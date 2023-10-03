#pragma once
#ifndef FPS_MANAGER_HPP
#define FPS_MANAGER_HPP

#include <list>
#include <SFML/System/Clock.hpp>

using u64 = unsigned long long;

class FPSManager {
  static std::list<u64> time_per_frame;
  static sf::Clock clock;
  static u64 framerate_limit;
  static u64 term;
  static u64 remain;
  static u64 add_per_frame;
  static u64 add;
  static u64 target;

  FPSManager();
  FPSManager(FPSManager const &rhs);
  FPSManager &operator=(FPSManager const &rhs);
  ~FPSManager();

public:
  static u64 getCurrentFPS();

  static u64 const &getFramerateLimit();
  static void setFramerateLimit(u64 const &framerate_limit);

  static void framePulse();

};

#endif
