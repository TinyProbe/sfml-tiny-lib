#pragma once
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "Object.hpp"

class Terrain : public Object {

public:
  Terrain() {}
  Terrain(Terrain const &rhs) { *this = rhs; }
  virtual Terrain &operator=(Terrain const &rhs) {
    if (this == &rhs) { return *this; }
    return *this;
  }
  virtual ~Terrain() {}

};

#endif
