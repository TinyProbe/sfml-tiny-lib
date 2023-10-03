#pragma once
#ifndef SCULPTURE_HPP
#define SCULPTURE_HPP

#include "Object.hpp"

class Sculpture : public Object {

public:
  Sculpture() {}
  Sculpture(Sculpture const &rhs) { *this = rhs; }
  virtual Sculpture &operator=(Sculpture const &rhs) {
    if (this == &rhs) { return *this; }
    return *this;
  }
  virtual ~Sculpture() {}

};

#endif
