#pragma once
#ifndef SPRITE_TEXTURE_MULTI_HPP
#define SPRITE_TEXTURE_MULTI_HPP

#include <stdexcept>
#include <vector>
#include <utility>

#include <SFML/System/Time.hpp>

#include "ImageTexture.hpp"

using usize = unsigned long;
using MotionMulti = std::pair<ImageTexture, sf::Time>;
using AnimeMulti = std::vector<MotionMulti>;

class SpriteTextureMulti {
  AnimeMulti anime_multi;

public:
  SpriteTextureMulti() {}
  SpriteTextureMulti(usize const &motion_multi_count) {
    this->anime_multi.resize(motion_multi_count);
  }
  SpriteTextureMulti(AnimeMulti const &anime_multi) {
    this->anime_multi.assign(anime_multi.begin(), anime_multi.end());
  }
  SpriteTextureMulti(SpriteTextureMulti const &rhs) { *this = rhs; }
  virtual SpriteTextureMulti &operator=(SpriteTextureMulti const &rhs) {
    if (this == &rhs) { return *this; }
    this->anime_multi.assign(rhs.anime_multi.begin(), rhs.anime_multi.end());
    return *this;
  }
  virtual ~SpriteTextureMulti() {}

  virtual operator SpriteTextureMulti const *() { return this; }

  virtual AnimeMulti const &getAnimeMulti() const {
    return this->anime_multi;
  }
  virtual void setAnimeMulti(AnimeMulti const &anime_multi) {
    this->anime_multi = anime_multi;
  }
  virtual usize getMotionMultiCount() const {
    return this->anime_multi.size();
  }
  virtual void setMotionMultiCount(usize const &motion_multi_count) {
    this->anime_multi.resize(motion_multi_count);
  }
  virtual MotionMulti const &getMotionMulti(usize const &motion_multi_code) const {
    this->codeCheck(motion_multi_code);
    return this->anime_multi[motion_multi_code];
  }
  virtual MotionMulti &getMotionMulti(usize const &motion_multi_code) {
    this->codeCheck(motion_multi_code);
    return this->anime_multi[motion_multi_code];
  }
  virtual void setMotionMulti(
    usize const &motion_multi_code,
    MotionMulti const &motion_multi
  ) {
    this->codeCheck(motion_multi_code);
    this->anime_multi[motion_multi_code] = motion_multi;
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  ) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.create(width, height, color);
    }
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  ) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.create(width, height, pixels);
    }
  }
  virtual void createMaskFromColor(sf::Color const &color, sf::Uint8 alpha = 0) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.createMaskFromColor(color, alpha);
    }
  }
  virtual void flipHorizontally() {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.flipHorizontally();
    }
  }
  virtual void flipVertically() {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.flipVertically();
    }
  }
  virtual void setSmooth(bool smooth) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.setSmooth(smooth);
    }
  }
  virtual void setSrgb(bool sRgb) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.setSrgb(sRgb);
    }
  }
  virtual void setRepeated(bool repeated) {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.setRepeated(repeated);
    }
  }
  virtual void generateMipmap() {
    for (MotionMulti &motion_multi: this->anime_multi) {
      motion_multi.first.generateMipmap();
    }
  }

private:
  virtual void codeCheck(usize const &motion_multi_code) {
    if (motion_multi_code >= this->getMotionMultiCount()) {
      throw std::runtime_error("No exist MotionMultiCode.");
    }
  }

};

#endif
