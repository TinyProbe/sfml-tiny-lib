#pragma once
#ifndef SPRITE_GENERATOR_HPP
#define SPRITE_GENERATOR_HPP

#include <stdexcept>
#include <vector>
#include <algorithm>

#include "WrapImage.hpp"
#include "WrapTexture.hpp"
#include "SpriteAtlas.hpp"

using i32 = int;
using f32 = float;
using usize = unsigned long;
using WrapImages = std::vector<WrapImage>;
using WrapImagesStore = std::vector<WrapImages>;

class SpriteGenerator {
  WrapImagesStore images_store;
  sf::Time holding_time;

public:
  SpriteGenerator();
  SpriteGenerator(usize const &images_count);
  SpriteGenerator(WrapImagesStore const &images_store);
  SpriteGenerator(SpriteGenerator const &rhs);
  virtual SpriteGenerator &operator=(SpriteGenerator const &rhs);
  virtual ~SpriteGenerator();

  virtual SpriteAtlas generateSpriteAtlas() const;

  virtual WrapImagesStore const &getImagesStore() const;
  virtual void setImagesStore(WrapImagesStore const &images_store);
  virtual usize getImagesCount() const;
  virtual void setImagesCount(usize const &images_count);
  virtual usize getImageCount(usize const &images_code) const;
  virtual void setImageCount(
    usize const &images_code,
    usize const &image_count
  );
  virtual WrapImages const &getImages(usize const &images_code) const;
  virtual void setImages(
    usize const &images_code,
    WrapImages const &images
  );
  virtual WrapImage const &getImage(
    usize const &images_code,
    usize const &image_code
  ) const;
  virtual WrapImage &getImage(
    usize const &images_code,
    usize const &image_code
  );
  virtual void setImage(
    usize const &images_code,
    usize const &image_code,
    WrapImage const &image
  );
  virtual void pushBackImage(
    usize const &images_code,
    WrapImage const &image
  );
  virtual WrapImage popBackImage(usize const &images_code);
  virtual sf::Time getHoldingTime() const;
  virtual void setHoldingTime(sf::Time const &holding_time);
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  );
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  );
  virtual void createMaskFromColor(
    sf::Color const &color,
    sf::Uint8 alpha = 0
  );
  virtual void flipHorizontally();
  virtual void flipVertically();

private:
  virtual void codeCheck(
    usize const &images_code,
    usize const &image_code = -1
  ) const;

};

#endif
