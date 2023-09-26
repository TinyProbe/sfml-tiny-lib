#pragma once
#ifndef SPRITE_TEXTURE_HPP
#define SPRITE_TEXTURE_HPP

#include "ImageTexture.hpp"
#include "Animation.hpp"

class SpriteTexture {
  ImageTexture image_texture;
  Animation animation;

public:
  SpriteTexture() {}
  SpriteTexture(SpriteTexture const &rhs) { *this = rhs; }
  virtual SpriteTexture &operator=(SpriteTexture const &rhs) {
    if (this == &rhs) { return *this; }
    this->image_texture = rhs.image_texture;
    this->animation = rhs.animation;
    return *this;
  }
  virtual ~SpriteTexture() {}



};

#endif
