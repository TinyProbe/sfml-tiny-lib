#include "lib/FPSManager.hpp"

#include <thread>

#include <SFML/System/Time.hpp>

using f64 = double;
using DurationNano = std::chrono::duration<u64, std::nano>;

static constexpr u64 kSecondUnit = 1000000; // us
static constexpr u64 kSleepUnit = 100000;   // ns

std::list<u64> FPSManager::time_per_frame_;
sf::Clock FPSManager::clock_;
u64 FPSManager::framerate_limit_  = u64(-1);
u64 FPSManager::term_             = kSecondUnit / FPSManager::framerate_limit_;
u64 FPSManager::remain_           = kSecondUnit % FPSManager::framerate_limit_;
u64 FPSManager::add_per_frame_    = f64(FPSManager::remain_) /
                                    FPSManager::framerate_limit_ * kSecondUnit;
u64 FPSManager::add_              = u64(0);
u64 FPSManager::target_           = u64(0);

u64 FPSManager::getCurrentFPS() noexcept {
  return FPSManager::time_per_frame_.size();
}

u64 const &FPSManager::getFramerateLimit() noexcept {
  return FPSManager::framerate_limit_;
}

void FPSManager::setFramerateLimit(u64 const &framerate_limit) noexcept {
  FPSManager::time_per_frame_.clear();
  FPSManager::clock_.restart();
  FPSManager::framerate_limit_  = (framerate_limit != 0 ?
                                   framerate_limit :
                                   u64(-1));
  FPSManager::term_             = kSecondUnit / FPSManager::framerate_limit_;
  FPSManager::remain_           = kSecondUnit % FPSManager::framerate_limit_;
  FPSManager::add_per_frame_    = f64(FPSManager::remain_) /
                                  FPSManager::framerate_limit_ * kSecondUnit;
  FPSManager::add_              = u64(0);
  FPSManager::target_           = u64(0);
}

void FPSManager::framePulse() noexcept {
  FPSManager::add_ += FPSManager::add_per_frame_;
  FPSManager::target_ += FPSManager::term_ + FPSManager::add_ / kSecondUnit;
  FPSManager::add_ %= kSecondUnit;
  u64 now;
  while (true) {
    now = u64(FPSManager::clock_.getElapsedTime().asMicroseconds());
    if (now >= FPSManager::target_) { break; }
    std::this_thread::sleep_for(DurationNano(kSleepUnit));
  }
  FPSManager::target_ = now;
  FPSManager::time_per_frame_.push_back(now);
  u64 cutline = (now >= kSecondUnit ? now - kSecondUnit : u64(0));
  while (FPSManager::time_per_frame_.front() <= cutline) {
    FPSManager::time_per_frame_.pop_front();
  }
}
