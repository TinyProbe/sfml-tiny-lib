#include "SpriteAtlas.hpp"

SpriteAtlas::SpriteAtlas() :
  WrapTexture(),
  Animation()
{}
SpriteAtlas::SpriteAtlas(usize const &anime_count) :
  WrapTexture(),
  Animation(anime_count)
{}
SpriteAtlas::SpriteAtlas(SpriteAtlas const &rhs) {
  *this = rhs;
}
SpriteAtlas &SpriteAtlas::operator=(SpriteAtlas const &rhs) {
  if (this == &rhs) { return *this; }
  dynamic_cast<WrapTexture &>(*this) = dynamic_cast<WrapTexture const &>(rhs);
  dynamic_cast<Animation &>(*this) = dynamic_cast<Animation const &>(rhs);
  return *this;
}
SpriteAtlas::~SpriteAtlas() {}

SpriteAtlas::operator SpriteAtlas const *() const {
  return this;
}
