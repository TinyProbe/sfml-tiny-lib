#pragma once
#ifndef SPRITE_ATLAS_HPP
#define SPRITE_ATLAS_HPP

#include <stdexcept>
#include <vector>
#include <utility>
#include <algorithm>

#include "SpriteTexture.hpp"

using i32 = int;
using f32 = float;
using usize = unsigned long;
using ImageTextures = std::vector<ImageTexture>;
using ImageTexturesStore = std::vector<ImageTextures>;

class SpriteAtlas {
  ImageTexturesStore image_textures_store;
  sf::Time holding_time;

public:
  SpriteAtlas() : holding_time(sf::milliseconds(100)) {}
  SpriteAtlas(usize const &image_textures_count) :
    holding_time(sf::milliseconds(100))
  {
    this->image_textures_store.resize(image_textures_count);
  }
  SpriteAtlas(ImageTexturesStore const &image_textures_store) :
    holding_time(sf::milliseconds(100))
  {
    this->image_textures_store.assign(
      image_textures_store.begin(),
      image_textures_store.end()
    );
  }
  SpriteAtlas(SpriteAtlas const &rhs) { *this = rhs; }
  virtual SpriteAtlas &operator=(SpriteAtlas const &rhs) {
    if (this == &rhs) { return *this; }
    this->image_textures_store.assign(
      rhs.image_textures_store.begin(),
      rhs.image_textures_store.end()
    );
    this->holding_time = rhs.holding_time;
    return *this;
  }
  virtual ~SpriteAtlas() {}

  virtual SpriteTexture generateSpriteTexture() const {
    AnimeStore animes;
    std::vector<usize> heights;
    usize total_width = 0, total_height = 0;
    for (ImageTextures const &image_textures: this->image_textures_store) {
      animes.push_back(Anime());
      heights.push_back(total_height);
      usize current_width = 0, current_height = 0;
      for (ImageTexture const &image_texture: image_textures) {
        sf::Vector2u image_size = image_texture.getImageSize();
        animes.back().push_back(
          Motion({
            sf::IntRect({
              i32(current_width), i32(total_height),
              i32(image_size.x), i32(image_size.y),
            }),
            this->holding_time,
          })
        );
        current_width += image_size.x;
        current_height = std::max(current_height, total_height + usize(image_size.y));
      }
      total_width = std::max(total_width, current_width);
      total_height = current_height;
    }
    SpriteTexture sprite_texture;
    sf::RenderTexture render_texture;
    render_texture.create(total_width, total_height);
    for (usize i = 0; i < this->image_textures_store.size(); ++i) {
      for (usize j = 0; j < this->image_textures_store[i].size(); ++j) {
        sf::RectangleShape rectangle_shape;
        sf::IntRect const &int_rect = animes[i][j].first;
        rectangle_shape.setTexture(this->image_textures_store[i][j]);
        rectangle_shape.setPosition(int_rect.left, heights[i]);
        rectangle_shape.setSize({ f32(int_rect.width), f32(int_rect.height) });
        render_texture.draw(rectangle_shape);
      }
    }
    sprite_texture.copy(render_texture.getTexture().copyToImage());
    sprite_texture.flipVertically();
    sprite_texture.loadFromImage();
    sprite_texture.setAnimes(std::move(animes));
    return std::move(sprite_texture);
  }

  virtual ImageTexturesStore const &getImageTexturesStore() const {
    return this->image_textures_store;
  }
  virtual void setImageTexturesStore(
    ImageTexturesStore const &image_textures_store
  ) {
    this->image_textures_store.assign(
      image_textures_store.begin(),
      image_textures_store.end()
    );
  }
  virtual usize getImageTexturesCount() const {
    return this->image_textures_store.size();
  }
  virtual void setImageTexturesCount(usize const &image_textures_count) {
    this->image_textures_store.resize(image_textures_count);
  }
  virtual usize getImageTextureCount(
    usize const &image_textures_code
  ) const {
    this->codeCheck(image_textures_code);
    return this->image_textures_store[image_textures_code].size();
  }
  virtual void setImageTextureCount(
    usize const &image_textures_code,
    usize const &image_texture_count
  ) {
    this->codeCheck(image_textures_code);
    this->image_textures_store[image_textures_code].resize(image_texture_count);
  }
  virtual ImageTextures const &getImageTextures(
    usize const &image_textures_code
  ) const {
    this->codeCheck(image_textures_code);
    return this->image_textures_store[image_textures_code];
  }
  virtual void setImageTextures(
    usize const &image_textures_code,
    ImageTextures const &image_textures
  ) {
    this->codeCheck(image_textures_code);
    this->image_textures_store[image_textures_code].assign(
      image_textures.begin(), 
      image_textures.end()
    );
  }
  virtual ImageTexture const &getImageTexture(
    usize const &image_textures_code,
    usize const &image_texture_code
  ) const {
    this->codeCheck(image_textures_code, image_texture_code);
    return this->image_textures_store[image_textures_code][image_texture_code];
  }
  virtual ImageTexture &getImageTexture(
    usize const &image_textures_code,
    usize const &image_texture_code
  ) {
    this->codeCheck(image_textures_code, image_texture_code);
    return this->image_textures_store[image_textures_code][image_texture_code];
  }
  virtual void setImageTexture(
    usize const &image_textures_code,
    usize const &image_texture_code,
    ImageTexture const &image_texture
  ) {
    this->codeCheck(image_textures_code, image_texture_code);
    this->image_textures_store[image_textures_code][image_texture_code] = image_texture;
  }
  virtual void pushBackImageTexture(
    usize const &image_textures_code,
    ImageTexture const &image_texture
  ) {
    this->codeCheck(image_textures_code);
    this->image_textures_store[image_textures_code].push_back(image_texture);
  }
  virtual ImageTexture popBackImageTexture(
    usize const &image_textures_code
  ) {
    this->codeCheck(image_textures_code, this->image_textures_store.size() - 1); // it will be fine.
    ImageTexture tmp = std::move(this->image_textures_store[image_textures_code].back());
    this->image_textures_store[image_textures_code].pop_back();
    return std::move(tmp);
  }
  virtual sf::Time getHoldingTime() const {
    return this->holding_time;
  }
  virtual void setHoldingTime(sf::Time const &holding_time) {
    this->holding_time = holding_time;
  }
  virtual void loadFromImage() {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.loadFromImage();
      }
    }
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  ) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.create(width, height, color);
      }
    }
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  ) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.create(width, height, pixels);
      }
    }
  }
  virtual void createMaskFromColor(sf::Color const &color, sf::Uint8 alpha = 0) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.createMaskFromColor(color, alpha);
      }
    }
  }
  virtual void flipHorizontally() {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.flipHorizontally();
      }
    }
  }
  virtual void flipVertically() {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.flipVertically();
      }
    }
  }
  virtual void setSmooth(bool smooth) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.setSmooth(smooth);
      }
    }
  }
  virtual void setSrgb(bool sRgb) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.setSrgb(sRgb);
      }
    }
  }
  virtual void setRepeated(bool repeated) {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.setRepeated(repeated);
      }
    }
  }
  virtual void generateMipmap() {
    for (ImageTextures &image_textures: this->image_textures_store) {
      for (ImageTexture &image_texture: image_textures) {
        image_texture.generateMipmap();
      }
    }
  }

private:
  virtual void codeCheck(
    usize const &image_textures_code,
    usize const &image_texture_code = -1
  ) const {
    if (image_textures_code >= this->image_textures_store.size()) {
      throw std::runtime_error("No exist image_textures_code.");
    }
    if (image_texture_code != -1
      && image_texture_code >= this->image_textures_store[image_textures_code].size())
    {
      throw std::runtime_error("No exist image_texture_code.");
    }
  }

};

#endif
