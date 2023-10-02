#pragma once
#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Object.hpp"

class BackGround : public Object {

public:
  BackGround() {}
  BackGround(BackGround const &rhs) { *this = rhs; }
  virtual BackGround &operator=(BackGround const &rhs) {
    if (this == &rhs) { return *this; }
    return *this;
  }
  virtual ~BackGround() {}

};

#endif
