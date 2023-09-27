#pragma once
#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include <stdexcept>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

using usize = unsigned long;
using u32 = unsigned int;
using i32 = int;

class ImageTexture {
  sf::Image image;
  sf::Texture texture;

public:
  ImageTexture() {}
  ImageTexture(ImageTexture const &rhs) { *this = rhs; }
  virtual ImageTexture &operator=(ImageTexture const &rhs) {
    if (this == &rhs) { return *this; }
    this->image = rhs.texture.copyToImage();
    this->texture = rhs.texture;
    return *this;
  }
  virtual ~ImageTexture() {}

  virtual sf::Image const &getImage() const { return this->image; }
  virtual sf::Texture const &getTexture() const { return this->texture; }

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
  virtual void loadFromStream(sf::InputStream &stream) {
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
  virtual void createMaskFromColor(sf::Color const &color, sf::Uint8 alpha = 0) {
    this->image.createMaskFromColor(color, alpha);
    this->loadFromImage();
  }
  virtual void copy(
    sf::Image const &source,
    u32 destX,
    u32 destY,
    sf::IntRect const &sourceRect = sf::IntRect(0, 0, 0, 0),
    bool applyAlpha = false
  ) {
    this->image.copy(source, destX, destY, sourceRect, applyAlpha);
    this->loadFromImage();
  }
  virtual sf::Image copyToImage() const {
    return std::move(this->texture.copyToImage());
  }
  virtual void setPixel(u32 x, u32 y, sf::Color const &color) {
    this->image.setPixel(x, y, color);
    this->loadFromImage();
  }
  virtual sf::Color getPixel(u32 x, u32 y) const {
    return this->image.getPixel(x, y);
  }
  virtual sf::Uint8 const *getPixelsPtr() const {
    return this->image.getPixelsPtr();
  }
  virtual void flipHorizontally() {
    this->image.flipHorizontally();
    this->loadFromImage();
  }
  virtual void flipVertically() {
    this->image.flipVertically();
    this->loadFromImage();
  }
  virtual void update(sf::Uint8 const *pixels) {
    this->texture.update(pixels);
  }
  virtual void update(sf::Uint8 const *pixels, u32 width, u32 height, u32 x, u32 y) {
    this->texture.update(pixels, width, height, x, y);
  }
  virtual void update(sf::Texture const &texture) {
    this->texture.update(texture);
  }
  virtual void update(sf::Texture const &texture, u32 x, u32 y) {
    this->texture.update(texture, x, y);
  }
  virtual void update(sf::Image const &image) {
    this->texture.update(image);
  }
  virtual void update(sf::Image const &image, u32 x, u32 y) {
    this->texture.update(image, x, y);
  }
  virtual void update(sf::Window const &window) {
    this->texture.update(window);
  }
  virtual void update(sf::Window const &window, u32 x, u32 y) {
    this->texture.update(window, x, y);
  }
  virtual void setSmooth(bool smooth) {
    this->texture.setSmooth(smooth);
  }
  virtual bool isSmooth() const {
    return this->texture.isSmooth();
  }
  virtual void setSrgb(bool sRgb) {
    this->texture.setSrgb(sRgb);
  }
  virtual bool isSrgb() const {
    return this->texture.isSrgb();
  }
  virtual void setRepeated(bool repeated) {
    this->texture.setRepeated(repeated);
  }
  virtual bool isRepeated() const {
    return this->texture.isRepeated();
  }
  virtual void generateMipmap() {
    if (!this->texture.generateMipmap()) {
      throw std::runtime_error("generate mipmap failed");
    }
  }
  virtual sf::Texture &operator=(sf::Texture const &right) {
    return this->texture = right;
  }
  virtual void swap(sf::Texture &right) {
    this->texture.swap(right);
  }
  virtual u32 getNativeHandle() const {
    return this->texture.getNativeHandle();
  }

private:
  virtual void loadFromImage() {
    sf::Vector2u size = this->image.getSize();
    if (!this->texture.loadFromImage(this->image, { 0, 0, i32(size.x), i32(size.y) })) {
      throw std::runtime_error("texture load failed");
    }
  }

};

#endif
