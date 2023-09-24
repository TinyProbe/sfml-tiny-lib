#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#define WIDTH  1200
#define HEIGHT 800

#define MOVE_UNIT 5

#include <stdexcept>
#include <iostream>
#include <cmath>
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

using DurationNano = std::chrono::duration<i64, std::nano>;
using TimePointNano = std::chrono::time_point<std::chrono::steady_clock, DurationNano>;

class FPSManager {
  usize framerate_limit;
  DurationNano term;
  std::list<TimePointNano> time_per_frame;

public:
  FPSManager() :
    framerate_limit(usize(-1)),
    term(DurationNano(1000000000 / this->framerate_limit)) {}
  FPSManager(usize const &framerate_limit) :
    framerate_limit(framerate_limit),
    term(DurationNano(1000000000 / this->framerate_limit)) {}
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
    this->term = DurationNano(1000000000 / this->framerate_limit);
  }
  virtual DurationNano const &getTerm() const {
    return this->term;
  }
  virtual usize getCurrentFPS() const {
    return this->time_per_frame.size();
  }
  virtual TimePointNano getFrameTime() {
    return (!this->time_per_frame.empty()
    ? this->time_per_frame.back()
    : std::chrono::steady_clock::now());
  }
  virtual void framePulse() {
    using namespace std::chrono_literals;
    TimePointNano now = std::chrono::steady_clock::now();
    DurationNano diff = (!time_per_frame.empty() ? now - time_per_frame.back() : 0ms);
    if (this->term > diff) {
      diff = this->term - diff;
      std::this_thread::sleep_for(diff);
      now += diff;
    }
    this->time_per_frame.push_back(now);
    TimePointNano cutline = now - 1000ms;
    while (this->time_per_frame.size() && this->time_per_frame.front() <= cutline) {
      this->time_per_frame.pop_front();
    }
  }

};

using Callback = std::function<void()>;
using CallbackElement = std::vector<Callback>;
using CallbackStore = std::vector<CallbackElement>;

class KeyManager {
public:
  enum Kind {
    Press,
    Release,
    Pressed,
    KindCount,
  };
  class KeyMap {
    CallbackStore callbacks;
    std::vector<bool> can_repeat;
    KeyManager const *key_manager;

  public:
    KeyMap() : key_manager() {}
    KeyMap(usize const &key_count) : key_manager() {
      this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
      this->can_repeat.resize(key_count);
    }
    KeyMap(KeyMap const &rhs) : key_manager() { *this = rhs; }
    virtual KeyMap &operator=(KeyMap const &rhs) {
      if (this == &rhs) { return *this; }
      this->callbacks.assign(rhs.callbacks.begin(), rhs.callbacks.end());
      this->can_repeat.assign(rhs.can_repeat.begin(), rhs.can_repeat.end());
      return *this;
    }
    virtual ~KeyMap() {
      if (this->key_manager != nullptr) {
        const_cast<KeyManager *>(this->key_manager)->unlink();
      }
    }

    virtual usize getKeyCount() const {
      return this->callbacks.size();
    }
    virtual void setKeyCount(usize const &key_count) {
      if (this->key_manager != nullptr) {
        throw std::runtime_error("Already linked KeyManager.");
      }
      this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
      this->can_repeat.resize(key_count);
    }

    virtual CallbackStore const &getCallbacks() const {
      return this->callbacks;
    }
    virtual Callback const &getCallback(
      usize const &key_code,
      usize const &kind_code
    ) const {
      this->codeCheck(key_code, usize(), kind_code);
      return this->callbacks[key_code][kind_code];
    }
    virtual void setCallback(
      usize const &key_code,
      usize const &kind_code,
      Callback callback,
      bool const &can_repeat = false
    ) {
      this->codeCheck(key_code, usize(), kind_code);
      this->callbacks[key_code][kind_code] = std::move(callback);
      if (kind_code == Press) {
        this->can_repeat[key_code] = can_repeat;
      }
    }
    virtual std::pair<Callback, bool> popCallback(
      usize const &key_code,
      usize const &kind_code
    ) {
      this->codeCheck(key_code, usize(), kind_code);
      std::pair<Callback, bool> tmp({
        std::move(this->callbacks[key_code][kind_code]),
        bool()
      });
      this->callbacks[key_code][kind_code] = Callback();
      if (kind_code == Press) {
        tmp.second = this->can_repeat[key_code];
        this->can_repeat[key_code] = bool();
      }
      return std::move(tmp);
    }
    virtual void copyCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    ) {
      this->codeCheck(key_code_from, key_code_to, kind_code);
      this->callbacks[key_code_to][kind_code]
        = this->callbacks[key_code_from][kind_code];
      if (kind_code == KeyManager::Press) {
        this->can_repeat[key_code_to] = this->can_repeat[key_code_from];
      }
    }
    virtual void moveCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    ) {
      this->codeCheck(key_code_from, key_code_to, kind_code);
      this->callbacks[key_code_to][kind_code]
        = std::move(this->callbacks[key_code_from][kind_code]);
      this->callbacks[key_code_from][kind_code] = Callback();
      if (kind_code == KeyManager::Press) {
        this->can_repeat[key_code_to] = this->can_repeat[key_code_from];
        this->can_repeat[key_code_from] = bool();
      }
    }
    virtual void swapCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    ) {
      this->codeCheck(key_code_from, key_code_to, kind_code);
      Callback tmp = std::move(this->callbacks[key_code_to][kind_code]);
      this->callbacks[key_code_to][kind_code]
        = std::move(this->callbacks[key_code_from][kind_code]);
      this->callbacks[key_code_from][kind_code]
        = std::move(tmp);
    }

    virtual bool canRepeat(usize const &key_code) const {
      this->codeCheck(key_code, usize(), usize());
      return this->can_repeat[key_code];
    }

  protected:
    friend class KeyManager;
    virtual void link(KeyManager const *key_manager) { this->key_manager = key_manager; }
    virtual void unlink() { this->key_manager = nullptr; }

    virtual void codeCheck(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    ) const {
      if (key_code_from >= this->getKeyCount()
        || key_code_to >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      } else if (kind_code >= KeyManager::KindCount) {
        throw std::runtime_error("No exist KindCode.");
      }
    }

  };

private:
  std::vector<bool> key_state;
  KeyMap const *key_map;

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
  virtual ~KeyManager() {
    if (this->key_map != nullptr) {
      const_cast<KeyMap *>(this->key_map)->unlink();
    }
  }

  virtual void keyPress(usize const &key_code) {
    if (key_code == usize(-1)) { return; }
    this->codeCheck(key_code, usize(), usize());
    if (this->key_map != nullptr) {
      if (!this->key_state[key_code] || this->key_map->canRepeat(key_code)) {
        Callback const &callback = this->key_map->getCallback(key_code, KeyManager::Press);
        if (callback) { callback(); }
      }
    }
    this->key_state[key_code] = true;
  }
  virtual void keyRelease(usize const &key_code) {
    if (key_code == usize(-1)) { return; }
    this->codeCheck(key_code, usize(), usize());
    if (this->key_map != nullptr) {
      Callback const &callback = this->key_map->getCallback(key_code, KeyManager::Release);
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
    this->codeCheck(key_code, usize(), usize());
    return this->key_state[key_code];
  }

  virtual KeyMap const &getKeyMap() const {
    return *this->key_map;
  }
  virtual void setKeyMap(KeyMap const &key_map) {
    if (this->getKeyCount() != key_map.getKeyCount()) {
      throw std::runtime_error("KeyCount doesn't match.");
    }
    if (this->key_map != nullptr) {
      const_cast<KeyMap *>(this->key_map)->unlink();
    }
    this->link(&key_map);
    const_cast<KeyMap *>(this->key_map)->link(this);
  }
  virtual void clearKeyMap() {
    if (this->key_map != nullptr) {
      const_cast<KeyMap *>(this->key_map)->unlink();
    }
    this->unlink();
  }

  virtual void keyFramework() const {
    if (this->key_map != nullptr) {
      CallbackStore const &callbacks = this->key_map->getCallbacks();
      for (usize i = this->getKeyCount(); i--; ) {
        if (this->key_state[i] && callbacks[i][KeyManager::Pressed]) {
          callbacks[i][KeyManager::Pressed]();
        }
      }
    }
  }

private:
  friend KeyMap::~KeyMap();
  virtual void link(KeyMap const *key_map) { this->key_map = key_map; }
  virtual void unlink() { this->key_map = nullptr; }

  virtual void codeCheck(
    usize const &key_code_from,
    usize const &key_code_to,
    usize const &kind_code
  ) const {
    if (key_code_from >= this->getKeyCount()
      || key_code_to >= this->getKeyCount()) {
      throw std::runtime_error("No exist KeyCode.");
    } else if (kind_code >= KeyManager::KindCount) {
      throw std::runtime_error("No exist KindCode.");
    }
  }

};

#endif
