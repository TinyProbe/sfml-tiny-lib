#pragma once
#ifndef SPRITE_TEXTURE_HPP
#define SPRITE_TEXTURE_HPP

#include "ImageTexture.hpp"
#include "Animation.hpp"

using usize = unsigned long;

class SpriteTexture : public ImageTexture, public Animation {

public:
  SpriteTexture() : ImageTexture(), Animation() {}
  SpriteTexture(usize const &anime_count) : ImageTexture(), Animation(anime_count) {}
  SpriteTexture(SpriteTexture const &rhs) { *this = rhs; }
  virtual SpriteTexture &operator=(SpriteTexture const &rhs) {
    if (this == &rhs) { return *this; }
    dynamic_cast<ImageTexture &>(*this) = dynamic_cast<ImageTexture const &>(rhs);
    dynamic_cast<Animation &>(*this) = dynamic_cast<Animation const &>(rhs);
    return *this;
  }
  virtual ~SpriteTexture() {}

};

#endif
