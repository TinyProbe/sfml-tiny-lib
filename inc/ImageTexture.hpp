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
  ImageTexture(std::string const &filename) {
    if (!this->image.loadFromFile(filename)) {
      throw std::runtime_error(std::string("load from file failed: ") + filename);
    }
  }
  ImageTexture(void const *data, std::size_t size) {
    if (!this->image.loadFromMemory(data, size)) {
      throw std::runtime_error("load from memory failed");
    }
  }
  ImageTexture(sf::InputStream &stream) {
    if (!this->image.loadFromStream(stream)) {
      throw std::runtime_error("load from stream failed");
    }
  }
  ImageTexture(ImageTexture const &rhs) { *this = rhs; }
  virtual ImageTexture &operator=(ImageTexture const &rhs) {
    if (this == &rhs) { return *this; }
    this->image = rhs.image;
    this->texture = rhs.texture;
    return *this;
  }
  virtual ~ImageTexture() {}

  virtual operator sf::Image const &() const { return this->image; }
  virtual operator sf::Texture const &() const { return this->texture; }
  virtual operator sf::Image const *() const { return &(this->image); }
  virtual operator sf::Texture const *() const { return &(this->texture); }

  virtual void loadFromImage() {
    sf::Vector2u size = this->image.getSize();
    if (!this->texture.loadFromImage(this->image, { 0, 0, i32(size.x), i32(size.y) })) {
      throw std::runtime_error("ImageTexture load failed");
    }
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  ) {
    this->image.create(width, height, color);
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  ) {
    this->image.create(width, height, pixels);
  }
  virtual void loadFromFile(std::string const &filename) {
    if (!this->image.loadFromFile(filename)) {
      throw std::runtime_error(std::string("load from file failed: ") + filename);
    }
  }
  virtual void loadFromMemory(void const *data, std::size_t size) {
    if (!this->image.loadFromMemory(data, size)) {
      throw std::runtime_error("load from memory failed");
    }
  }
  virtual void loadFromStream(sf::InputStream &stream) {
    if (!this->image.loadFromStream(stream)) {
      throw std::runtime_error("load from stream failed");
    }
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
  }
  virtual void copy(
    sf::Image const &source,
    u32 destX = 0,
    u32 destY = 0,
    sf::IntRect const &sourceRect = sf::IntRect(0, 0, 0, 0),
    bool applyAlpha = false
  ) {
    sf::Vector2u size = source.getSize();
    this->image.create(size.x, size.y, sf::Color::Black);
    this->image.copy(source, destX, destY, sourceRect, applyAlpha);
  }
  virtual sf::Image copyToImage() const {
    return std::move(this->texture.copyToImage());
  }
  virtual void setPixel(u32 x, u32 y, sf::Color const &color) {
    this->image.setPixel(x, y, color);
  }
  virtual sf::Color getPixel(u32 x, u32 y) const {
    return this->image.getPixel(x, y);
  }
  virtual sf::Uint8 const *getPixelsPtr() const {
    return this->image.getPixelsPtr();
  }
  virtual void flipHorizontally() {
    this->image.flipHorizontally();
  }
  virtual void flipVertically() {
    this->image.flipVertically();
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

};

#endif
