#include "FPSManager.hpp"

#include <thread>
#include <SFML/System/Time.hpp>

#define TIME_UNIT__ 1000000 // us
#define SLEEP_UNIT__ 100000 // ns

using f64 = double;
using DurationNano = std::chrono::duration<u64, std::nano>;

std::list<u64> FPSManager::time_per_frame;
sf::Clock FPSManager::clock;
u64 FPSManager::framerate_limit = -1;
u64 FPSManager::term            = TIME_UNIT__ / FPSManager::framerate_limit;
u64 FPSManager::remain          = TIME_UNIT__ % FPSManager::framerate_limit;
u64 FPSManager::add_per_frame   = f64(FPSManager::remain)
                                  / FPSManager::framerate_limit * TIME_UNIT__;
u64 FPSManager::add             = 0;
u64 FPSManager::target          = 0;

u64 FPSManager::getCurrentFPS() {
  return FPSManager::time_per_frame.size();
}

u64 const &FPSManager::getFramerateLimit() {
  return FPSManager::framerate_limit;
}
void FPSManager::setFramerateLimit(u64 const &framerate_limit) {
  u64 now = u64(FPSManager::clock.getElapsedTime().asMicroseconds());
  FPSManager::framerate_limit
    = (framerate_limit != 0 ? framerate_limit : u64(-1));
  FPSManager::term = TIME_UNIT__ / FPSManager::framerate_limit;
  FPSManager::remain = TIME_UNIT__ % FPSManager::framerate_limit;
  FPSManager::add_per_frame
    = f64(FPSManager::remain) / FPSManager::framerate_limit * TIME_UNIT__;
  FPSManager::add = now;
  FPSManager::target = now;
}

void FPSManager::framePulse() {
  FPSManager::add += FPSManager::add_per_frame;
  FPSManager::target += FPSManager::term + FPSManager::add / TIME_UNIT__;
  FPSManager::add %= TIME_UNIT__;
  u64 now;
  while (true) {
    now = u64(FPSManager::clock.getElapsedTime().asMicroseconds());
    if (now >= FPSManager::target) {
      FPSManager::time_per_frame.push_back(now);
      break;
    }
    std::this_thread::sleep_for(DurationNano(SLEEP_UNIT__));
  }
  u64 cutline = (now >= TIME_UNIT__ ? now - TIME_UNIT__ : u64());
  while (FPSManager::time_per_frame.front() <= cutline) {
    FPSManager::time_per_frame.pop_front();
  }
}
