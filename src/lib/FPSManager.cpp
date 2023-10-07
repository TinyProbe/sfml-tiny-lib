#include "lib/FPSManager.hpp"

#include <thread>

#include <SFML/System/Time.hpp>

using f64 = double;
using DurationNano = std::chrono::duration<u64, std::nano>;

static constexpr u64 kSecondUnit = 1000000; // us
static constexpr u64 kSleepUnit = 100000;   // ns

std::list<u64> FPSManager::time_per_frame;
sf::Clock FPSManager::clock;
u64 FPSManager::framerate_limit = u64(-1);
u64 FPSManager::term            = kSecondUnit / FPSManager::framerate_limit;
u64 FPSManager::remain          = kSecondUnit % FPSManager::framerate_limit;
u64 FPSManager::add_per_frame   = f64(FPSManager::remain) /
                                  FPSManager::framerate_limit * kSecondUnit;
u64 FPSManager::add             = 0;
u64 FPSManager::target          = 0;

u64 FPSManager::getCurrentFPS() noexcept {
  return FPSManager::time_per_frame.size();
}

u64 const &FPSManager::getFramerateLimit() noexcept {
  return FPSManager::framerate_limit;
}

void FPSManager::setFramerateLimit(u64 const &framerate_limit) noexcept {
  FPSManager::time_per_frame.clear();
  FPSManager::clock.restart();
  FPSManager::framerate_limit = (framerate_limit != 0 ?
                                 framerate_limit :
                                 u64(-1));
  FPSManager::term            = kSecondUnit / FPSManager::framerate_limit;
  FPSManager::remain          = kSecondUnit % FPSManager::framerate_limit;
  FPSManager::add_per_frame   = f64(FPSManager::remain) /
                                FPSManager::framerate_limit * kSecondUnit;
  FPSManager::add             = u64(0);
  FPSManager::target          = u64(0);
}

void FPSManager::framePulse() noexcept {
  FPSManager::add += FPSManager::add_per_frame;
  FPSManager::target += FPSManager::term + FPSManager::add / kSecondUnit;
  FPSManager::add %= kSecondUnit;
  u64 now;
  while (true) {
    now = u64(FPSManager::clock.getElapsedTime().asMicroseconds());
    if (now >= FPSManager::target) { break; }
    std::this_thread::sleep_for(DurationNano(kSleepUnit));
  }
  FPSManager::target = now;
  FPSManager::time_per_frame.push_back(now);
  u64 cutline = (now >= kSecondUnit ? now - kSecondUnit : u64(0));
  while (FPSManager::time_per_frame.front() <= cutline) {
    FPSManager::time_per_frame.pop_front();
  }
}
