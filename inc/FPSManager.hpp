#pragma once
#ifndef FPS_MANAGER_HPP
#define FPS_MANAGER_HPP

#define TIME_UNIT__ 1000000 // us
#define SLEEP_UNIT__ 100000 // ns

#include <thread>
#include <list>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

using u64 = unsigned long long;
using f64 = double;
using DurationNano = std::chrono::duration<u64, std::nano>;

// A high precision FPS manager.
class FPSManager {
  std::list<u64> time_per_frame;
  sf::Clock clock;
  u64 framerate_limit;
  u64 term;
  u64 remain;
  u64 add_per_frame;
  u64 add;
  u64 target;

public:
  FPSManager() :
    framerate_limit(u64(-1)),
    term(TIME_UNIT__ / this->framerate_limit),
    remain(TIME_UNIT__ % this->framerate_limit),
    add_per_frame(f64(this->remain) / this->framerate_limit * TIME_UNIT__),
    add(),
    target()
  {}
  FPSManager(u64 const &framerate_limit) :
    framerate_limit(framerate_limit != 0 ? framerate_limit : u64(-1)),
    term(TIME_UNIT__ / this->framerate_limit),
    remain(TIME_UNIT__ % this->framerate_limit),
    add_per_frame(f64(this->remain) / this->framerate_limit * TIME_UNIT__),
    add(),
    target()
  {}
  FPSManager(FPSManager const &rhs) { *this = rhs; }
  virtual FPSManager &operator=(FPSManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->time_per_frame.assign(rhs.time_per_frame.begin(), rhs.time_per_frame.end());
    this->clock = rhs.clock;
    this->framerate_limit = rhs.framerate_limit;
    this->term = rhs.term;
    this->remain = rhs.remain;
    this->add_per_frame = rhs.add_per_frame;
    this->add = rhs.add;
    this->target = rhs.target;
    return *this;
  }
  virtual ~FPSManager() {}

  virtual u64 getCurrentFPS() const { return this->time_per_frame.size(); }
  virtual u64 const &getFramerateLimit() const { return this->framerate_limit; }
  virtual void setFramerateLimit(u64 const &framerate_limit) {
    u64 now = u64(this->clock.getElapsedTime().asMicroseconds());
    this->framerate_limit = framerate_limit != 0 ? framerate_limit : u64(-1);
    this->term = TIME_UNIT__ / this->framerate_limit;
    this->remain = TIME_UNIT__ % this->framerate_limit;
    this->add_per_frame = f64(this->remain) / this->framerate_limit * TIME_UNIT__;
    this->add = now;
    this->target = now;
  }

  virtual void framePulse() {
    this->add += this->add_per_frame;
    this->target += this->term + this->add / TIME_UNIT__;
    this->add %= TIME_UNIT__;
    u64 now;
    while (true) {
      now = u64(this->clock.getElapsedTime().asMicroseconds());
      if (now >= this->target) {
        this->time_per_frame.push_back(now);
        break;
      }
      std::this_thread::sleep_for(DurationNano(SLEEP_UNIT__));
    }
    u64 cutline = (now >= TIME_UNIT__ ? now - TIME_UNIT__ : u64());
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
//     std::cout << fpsmng.getCurrentFPS() << '\n';
//
//     //
//     // do something ...
//     //
//     fpsmng.framePulse();
//   }
// }

#endif
