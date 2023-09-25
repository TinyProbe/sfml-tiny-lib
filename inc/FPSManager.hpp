#pragma once
#ifndef FPS_MANAGER_HPP
#define FPS_MANAGER_HPP

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <thread>
#include <list>

using usize = unsigned long;
using u64 = unsigned long long;
using DurationNano = std::chrono::duration<u64, std::nano>;

// A high precision FPS manager.
class FPSManager {
  usize framerate_limit;
  sf::Clock clock;
  sf::Time term;
  std::list<sf::Time> time_per_frame;

public:
  FPSManager() :
    framerate_limit(usize(-1)),
    term(sf::microseconds(u64(1000000) / this->framerate_limit)),
    time_per_frame({ clock.getElapsedTime() }) {}
  FPSManager(usize const &framerate_limit) :
    framerate_limit(framerate_limit != 0 ? framerate_limit : usize(-1)),
    term(sf::microseconds(u64(1000000) / this->framerate_limit)),
    time_per_frame({ clock.getElapsedTime() }) {}
  FPSManager(FPSManager const &rhs) { *this = rhs; }
  virtual FPSManager &operator=(FPSManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->framerate_limit = rhs.framerate_limit;
    this->clock = rhs.clock;
    this->term = rhs.term;
    this->time_per_frame.assign(rhs.time_per_frame.begin(), rhs.time_per_frame.end());
    return *this;
  }
  virtual ~FPSManager() {}

  virtual usize const &getFramerateLimit() const {
    return this->framerate_limit;
  }
  virtual void setFramerateLimit(usize const &framerate_limit) {
    this->framerate_limit = (framerate_limit != 0 ? framerate_limit : usize(-1));
    this->term = sf::microseconds(u64(1000000) / this->framerate_limit);
  }
  virtual sf::Time const &getTerm() const {
    return this->term;
  }
  virtual usize getCurrentFPS() const {
    return this->time_per_frame.size();
  }
  virtual sf::Time getFrameTime() {
    return this->time_per_frame.back();
  }
  virtual void framePulse() {
    sf::Time now = clock.getElapsedTime();
    sf::Time diff = now - time_per_frame.back();
    if (this->term > diff) {
      diff = this->term - diff;
      std::this_thread::sleep_for(DurationNano(u64(diff.asMicroseconds()) * 1000));
      now += diff;
    }
    this->time_per_frame.push_back(now);
    sf::Time cutline = now - sf::milliseconds(1000);
    while (this->time_per_frame.front() <= cutline) {
      this->time_per_frame.pop_front();
    }
  }

};

// // Example code
// int main() {
//   // 
//   // create window ...
//   //
//
//   // create FPSManager instance
//   FPSManager fpsmng;
//   fpsmng.setFramerateLimit(60);
//
//   // start event loop
//   while (true) {
//     fpsmng.framePulse();
//     std::cout << fpsmng.getCurrentFPS() << '\n';
//
//     //
//     // do something ...
//     //
//   }
// }

#endif
