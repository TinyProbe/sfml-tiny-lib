#ifndef FPS_MANAGER_HPP_
#define FPS_MANAGER_HPP_

#include <list>
#include <SFML/System/Clock.hpp>

using u64 = unsigned long long;

class FPSManager {
 public:
  static u64 getCurrentFPS() noexcept;

  static u64 const &getFramerateLimit() noexcept;
  static void setFramerateLimit(u64 const &framerate_limit) noexcept;

  static void framePulse() noexcept;

 private:
  FPSManager() = delete;
  FPSManager(FPSManager const &rhs) = delete;
  FPSManager &operator=(FPSManager const &rhs) = delete;
  ~FPSManager() = delete;

  static std::list<u64> time_per_frame;
  static sf::Clock clock;
  static u64 framerate_limit;
  static u64 term;
  static u64 remain;
  static u64 add_per_frame;
  static u64 add;
  static u64 target;

}; // FPSManager

#endif
