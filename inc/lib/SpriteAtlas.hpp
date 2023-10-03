#pragma once
#ifndef SPRITE_ATLAS_HPP
#define SPRITE_ATLAS_HPP

#include "WrapTexture.hpp"
#include "Animation.hpp"

using usize = unsigned long;

class SpriteAtlas : public WrapTexture, public Animation {

public:
  SpriteAtlas();
  SpriteAtlas(usize const &anime_count);
  SpriteAtlas(SpriteAtlas const &rhs);
  virtual SpriteAtlas &operator=(SpriteAtlas const &rhs);
  virtual ~SpriteAtlas();

  virtual operator SpriteAtlas const *() const;

};

#endif
