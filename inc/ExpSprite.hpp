#pragma once
#ifndef EXP_SPRITE_HPP
#define EXP_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Animation.hpp"

class ExpSprite : public sf::Sprite {
  // member ...

public:
  ExpSprite() : sf::Sprite() {}
  ExpSprite(ExpSprite const &rhs) { *this = rhs; }
  virtual ExpSprite(ExpSprite const &rhs) {
    if (this == &rhs) { return *this; }
    dynamic_cast<Sprite &>(*this) = dynamic_cast<Sprite const &>(rhs);

    return *this;
  }
  virtual ~ExpSprite() {}

  // setAnimation, setAnime ... etc

};

#endif
