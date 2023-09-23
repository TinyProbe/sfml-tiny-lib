#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#define WIDTH  1280
#define HEIGHT 800

#define MOVE_UNIT 5

#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <functional>

using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;
using isize = long;
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using usize = unsigned long;
using f32 = float;
using f64 = double;

class FPSManager {
  usize framerate_limit;
  std::chrono::duration<i64, std::nano> term;
  std::list<std::chrono::time_point<std::chrono::steady_clock,
    std::chrono::duration<i64, std::nano>>> time_per_frame;

public:
  FPSManager() :
    framerate_limit(usize(-1)),
    term(std::chrono::duration<i64, std::nano>(1000000000 / this->framerate_limit)) {}
  FPSManager(usize const &framerate_limit) :
    framerate_limit(framerate_limit),
    term(std::chrono::duration<i64, std::nano>(1000000000 / this->framerate_limit)) {}
  FPSManager(FPSManager const &rhs) { *this = rhs; }
  virtual FPSManager &operator=(FPSManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->framerate_limit = rhs.framerate_limit;
    this->term = rhs.term;
    this->time_per_frame.assign(rhs.time_per_frame.begin(), rhs.time_per_frame.end());
    return *this;
  }
  virtual ~FPSManager() {}

  virtual usize const &getFramerateLimit() const {
    return this->framerate_limit;
  }
  virtual void setFramerateLimit(usize const &framerate_limit) {
    this->framerate_limit = (framerate_limit ? framerate_limit : usize(-1));
    this->term = std::chrono::duration<i64, std::nano>(1000000000 / this->framerate_limit);
  }
  virtual std::chrono::duration<i64, std::nano> const &getTerm() const {
    return this->term;
  }
  virtual usize getCurrentFPS() const {
    return this->time_per_frame.size();
  }
  virtual void framePulse() {
    using namespace std::chrono_literals;
    std::chrono::time_point<std::chrono::steady_clock,
      std::chrono::duration<i64, std::nano>> now = std::chrono::steady_clock::now();
    std::chrono::duration<i64, std::nano> diff
      = (time_per_frame.size() ? now - time_per_frame.back() : 0ms);
    if (this->term > diff) {
      diff = this->term - diff;
      std::this_thread::sleep_for(diff);
      now += diff;
    }
    this->time_per_frame.push_back(now);
    std::chrono::time_point<std::chrono::steady_clock,
      std::chrono::duration<i64, std::nano>> cutline = now - 1000ms;
    while (this->time_per_frame.size() && this->time_per_frame.front() <= cutline) {
      this->time_per_frame.pop_front();
    }
  }

};

class KeyManager {
public:
  enum Kind {
    Press,
    Release,
    Pressed,
    KindCount,
  };
  class KeyMap {
    std::vector<std::vector<std::function<void()>>> callbacks;
    std::vector<bool> can_repeat;
    bool is_linked;

  public:
    KeyMap() : is_linked() {}
    KeyMap(usize const &key_count) : is_linked() {
      this->callbacks.resize(key_count, std::vector<std::function<void()>>(KindCount));
      this->can_repeat.resize(key_count);
    }
    KeyMap(KeyMap const &rhs) : is_linked() { *this = rhs; }
    virtual KeyMap &operator=(KeyMap const &rhs) {
      if (this == &rhs) { return *this; }
      this->callbacks.assign(rhs.callbacks.begin(), rhs.callbacks.end());
      this->can_repeat.assign(rhs.can_repeat.begin(), rhs.can_repeat.end());
      return *this;
    }
    virtual ~KeyMap() {}

    virtual usize getKeyCount() const {
      return this->callbacks.size();
    }
    virtual void setKeyCount(usize const &key_count) {
      if (this->is_linked) {
        throw std::runtime_error("Already linked KeyManager.");
      }
      this->callbacks.resize(key_count, std::vector<std::function<void()>>(KindCount));
      this->can_repeat.resize(key_count);
    }

    virtual std::vector<std::vector<std::function<void()>>> const &getCallbacks() const {
      return this->callbacks;
    }
    virtual std::function<void()> const &getCallback(usize const &key_code, usize const &kind_code) const {
      if (key_code >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      } else if (kind_code >= KindCount) {
        throw std::runtime_error("No exist KindCode.");
      }
      return this->callbacks[key_code][kind_code];
    }
    virtual void setCallback(
      usize const &key_code,
      usize const &kind_code,
      std::function<void()> callback,
      bool const &can_repeat = false
    ) {
      if (key_code >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      } else if (kind_code >= KindCount) {
        throw std::runtime_error("No exist KindCode.");
      }
      this->callbacks[key_code][kind_code] = callback;
      if (kind_code == Press) {
        this->can_repeat[key_code] = can_repeat;
      }
    }
    
    virtual bool canRepeat(usize const &key_code) const {
      if (key_code >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      }
      return this->can_repeat[key_code];
    }

    virtual void setLink() { this->is_linked = true; }
    virtual void setUnlink() { this->is_linked = false; }

  };

private:
  std::vector<bool> key_state;
  KeyMap *key_map;

public:
  KeyManager() : key_map() {}
  KeyManager(usize const &key_count) : key_map() {
    this->key_state.resize(key_count);
  }
  KeyManager(KeyManager const &rhs) { *this = rhs; }
  virtual KeyManager &operator=(KeyManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->key_state.assign(rhs.key_state.begin(), rhs.key_state.end());
    this->key_map = rhs.key_map;
    return *this;
  }
  virtual ~KeyManager() {}

  virtual void keyPress(usize const &key_code) {
    if (key_code >= this->getKeyCount()) {
      throw std::runtime_error("No exist KeyCode.");
    }
    if (this->key_map != nullptr) {
      if (!this->key_state[key_code] || this->key_map->canRepeat(key_code)) {
        std::function<void()> const &callback
          = this->key_map->getCallback(key_code, KeyManager::Press);
        if (callback) { callback(); }
      }
    }
    this->key_state[key_code] = true;
  }
  virtual void keyRelease(usize const &key_code) {
    if (key_code >= this->getKeyCount()) {
      throw std::runtime_error("No exist KeyCode.");
    }
    if (this->key_map != nullptr) {
      std::function<void()> const &callback
        = this->key_map->getCallback(key_code, KeyManager::Release);
      if (callback) { callback(); }
    }
    this->key_state[key_code] = false;
  }

  virtual usize getKeyCount() const {
    return this->key_state.size();
  }
  virtual void setKeyCount(usize const &key_count) {
    if (this->key_map != nullptr) {
      throw std::runtime_error("Already linked KeyMap.");
    }
    this->key_state.resize(key_count);
  }

  virtual bool getKeyState(usize const &key_code) const {
    if (key_code >= this->getKeyCount()) {
      throw std::runtime_error("No exist KeyCode.");
    }
    return this->key_state[key_code];
  }

  virtual KeyMap const *const getKeyMap() const {
    return this->key_map;
  }
  virtual void setKeyMap(KeyMap *key_map) {
    if (key_map != nullptr && this->getKeyCount() != key_map->getKeyCount()) {
      throw std::runtime_error("KeyCount doesn't match.");
    }
    if (this->key_map != nullptr) { this->key_map->setUnlink(); }
    this->key_map = key_map;
    if (this->key_map != nullptr) { this->key_map->setLink(); }
  }

  virtual void keyFramework() const {
    if (this->key_map != nullptr) {
      std::vector<std::vector<std::function<void()>>> const &callbacks
        = this->key_map->getCallbacks();
      for (usize i = 0; i < callbacks.size(); ++i) {
        if (this->key_state[i] && callbacks[i][KeyManager::Pressed]) {
          callbacks[i][KeyManager::Pressed]();
        }
      }
    }
  }

};

#endif
