#pragma once
#ifndef AUTO_SPRITE_MULTI_HPP
#define AUTO_SPRITE_MULTI_HPP

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "SpriteTextureMulti.hpp"

using usize = unsigned long;
using SpriteMulti = std::vector<sf::RectangleShape>;

class AutoSpriteMulti {
  sf::Clock clock;
  SpriteMulti sprite_multi;
  usize current_motion;
  SpriteTextureMulti const *sprite_texture_multi;

public:
  AutoSpriteMulti() :
    current_motion(),
    sprite_texture_multi()
  {}
  AutoSpriteMulti(SpriteTextureMulti const *const &sprite_texture_multi) :
    current_motion(),
    sprite_texture_multi(sprite_texture_multi)
  {
    if (sprite_texture_multi != nullptr) {
      AnimeMulti const &anime_multi = sprite_texture_multi->getAnimeMulti();
      this->sprite_multi.resize(anime_multi.size());
      for (usize i = 0; i < anime_multi.size(); ++i) {
        this->sprite_multi[i].setTexture(anime_multi[i].first);
      }
    }
  }
  AutoSpriteMulti(AutoSpriteMulti const &rhs) { *this = rhs; }
  virtual AutoSpriteMulti &operator=(AutoSpriteMulti const &rhs) {
    if (this == &rhs) { return *this; }
    this->clock = rhs.clock;
    this->sprite_multi.assign(rhs.sprite_multi.begin(). rhs.sprite_multi.end());
    this->current_motion = rhs.current_motion;
    this->sprite_texture_multi = rhs.sprite_texture_multi;
    return *this;
  }
  virtual ~AutoSpriteMulti() {}

};

#endif
