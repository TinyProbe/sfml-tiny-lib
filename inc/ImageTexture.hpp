#pragma once
#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <utility>

using usize = unsigned long;

class ImageTexture {
  sf::Image image;
  sf::Texture texture;

public:
  ImageTexture() {}
  ImageTexture(ImageTexture const &rhs) { *this = rhs; }
  virtual ImageTexture &operator=(ImageTexture const &rhs) {
    if (this == &rhs) { return *this; }
    this->image = rhs.image;
    this->texture = rhs.texture;
    return *this;
  }
  virtual ~ImageTexture() {}

  virtual operator sf::Image() const { return this->image; }
  virtual operator sf::Texture() const { return this->texture; }

  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  ) {
    this->image.create(width, height, color);
    this->loadFromImage();
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  ) {
    this->image.create(width, height, pixels);
    this->loadFromImage();
  }
  virtual void loadFromFile(std::string const &filename) {
    if (!this->image.loadFromFile(filename)) {
      throw std::runtime_error(std::string("load from file failed: ") + filename);
    }
    this->loadFromImage();
  }
  virtual void loadFromMemory(void const *data, std::size_t size) {
    if (!this->image.loadFromMemory(data, size)) {
      throw std::runtime_error("load from memory failed");
    }
    this->loadFromImage();
  }
  virtual void loadFromStream(InputStream &stream) {
    if (!this->image.loadFromStream(stream)) {
      throw std::runtime_error("load from stream failed");
    }
    this->loadFromImage();
  }
  virtual void saveToFile(std::string const &filename) const {
    if (!this->image.saveToFile(filename)) {
      throw std::runtime_error("save to file failed");
    }
  }
  virtual void saveToMemory(
    std::vector<sf::Uint8> &output,
    std::string const &format
  ) const {
    if (this->image.saveToMemory(output, format)) {
      throw std::runtime_error("save to memory failed");
    }
  }
  virtual sf::Vector2u getImageSize() const {
    return this->image.getSize();
  }
  virtual sf::Vector2u getTextureSize() const {
    return this->texture.getSize();
  }
  virtual void createMaskFromColor(const Color &color, Uint8 alpha = 0) {
    this->image.createMaskFromColor(color, alpha);
  }
  // sf::Image: copy ...

private:
  virtual void loadFromImage() {
    sf::vector2u size = this->image.getSize();
    if (!this->texture.loadFromImage(this->image, { 0, 0, size.x, size.y })) {
      throw std::runtime_error("texture load failed");
    }
  }

};

#endif
