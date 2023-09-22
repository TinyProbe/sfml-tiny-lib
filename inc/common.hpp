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
  virtual usize const &getCurrentFPS() const {
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
  std::vector<std::pair<bool, std::function<void()>>> key_state_func;

public:
  KeyManager() {}
  KeyManager(usize const &key_count) {
    this->key_state_func.resize(key_count);
  }
  KeyManager(KeyManager const &rhs) { *this = rhs; }
  virtual KeyManager &operator=(KeyManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->key_state_func.assign(rhs.key_state_func.begin(), rhs.key_state_func.end());
    return *this;
  }
  virtual ~KeyManager() {}

  virtual void keyPress(usize const &key_code) {
    if (key_code >= this->key_state_func.size()) { throw std::runtime_error("No exist KeyCode."); }
    this->key_state_func[key_code].first = true;
  }
  virtual void keyRelease(usize const &key_code) {
    if (key_code >= this->key_state_func.size()) { throw std::runtime_error("No exist KeyCode."); }
    this->key_state_func[key_code].first = false;
  }
  virtual usize const &getKeyCount() const {
    return this->key_state_func.size();
  }
  virtual void setKeyCount(usize const &key_count) {
    this->key_state_func.resize(key_count);
  }
  virtual void setCallback(usize const &key_code, std::function<void()> callback) {
    if (key_code >= this->key_state_func.size()) { throw std::runtime_error("No exist KeyCode."); }
    this->key_state_func[key_code].second = callback;
  }
  virtual void callbackAll() const {
    for (std::pair<bool, std::function<void()>> const &elem: this->key_state_func) {
      if (elem.first && elem.second) { elem.second(); }
    }
  }

};

#endif
