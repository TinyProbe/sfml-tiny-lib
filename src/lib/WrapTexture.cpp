#include "WrapTexture.hpp"

WrapTexture::WrapTexture() {}
WrapTexture::WrapTexture(
  std::string const &filename,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromFile(filename, area)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}
WrapTexture::WrapTexture(
  void const *data,
  usize size,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromMemory(data, size, area)) {
    throw std::runtime_error("load from memory failed");
  }
}
WrapTexture::WrapTexture(
  sf::InputStream &stream,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromStream(stream, area)) {
    throw std::runtime_error("load from stream failed");
  }
}
WrapTexture::WrapTexture(
  sf::Image const &image,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromImage(image, area)) {
    throw std::runtime_error("load from image failed");
  }
}
WrapTexture::WrapTexture(WrapTexture const &rhs) {
  *this = rhs;
}
WrapTexture &WrapTexture::operator=(WrapTexture const &rhs) {
  if (this == &rhs) { return *this; }
  this->texture = rhs.texture;
  return *this;
}
WrapTexture::~WrapTexture() {}

WrapTexture::operator sf::Texture &() {
  return this->texture;
}
WrapTexture::operator sf::Texture *() {
  return &(this->texture);
}
WrapTexture::operator sf::Texture const &() const {
  return this->texture;
}
WrapTexture::operator sf::Texture const *() const {
  return &(this->texture);
}

sf::Vector2u WrapTexture::getSize() const {
  return this->texture.getSize();
}

bool WrapTexture::isSmooth() const {
  return this->texture.isSmooth();
}
void WrapTexture::setSmooth(bool smooth) {
  this->texture.setSmooth(smooth);
}

bool WrapTexture::isSrgb() const {
  return this->texture.isSrgb();
}
void WrapTexture::setSrgb(bool sRgb) {
  this->texture.setSrgb(sRgb);
}

bool WrapTexture::isRepeated() const {
  return this->texture.isRepeated();
}
void WrapTexture::setRepeated(bool repeated) {
  this->texture.setRepeated(repeated);
}

u32 WrapTexture::getNativeHandle() const {
  return this->texture.getNativeHandle();
}

void WrapTexture::create(u32 width, u32 height) {
  this->texture.create(width, height);
}

void WrapTexture::loadFromFile(
  std::string const &filename,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromFile(filename, area)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}
void WrapTexture::loadFromMemory(
  void const *data,
  usize size,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromMemory(data, size, area)) {
    throw std::runtime_error("load from memory failed");
  }
}
void WrapTexture::loadFromStream(
  sf::InputStream &stream,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromStream(stream, area)) {
    throw std::runtime_error("load from stream failed");
  }
}
void WrapTexture::loadFromImage(
  sf::Image const &image,
  sf::IntRect const &area
) {
  if (!this->texture.loadFromImage(image, area)) {
    throw std::runtime_error("load from image failed");
  }
}

sf::Image WrapTexture::copyToImage() const {
  return std::move(this->texture.copyToImage());
}

void WrapTexture::swap(sf::Texture &right) {
  this->texture.swap(right);
}
void WrapTexture::generateMipmap() {
  if (!this->texture.generateMipmap()) {
    throw std::runtime_error("generate mipmap failed");
  }
}

void WrapTexture::update(sf::Uint8 const *pixels) {
  this->texture.update(pixels);
}
void WrapTexture::update(
  sf::Uint8 const *pixels, u32 width, u32 height, u32 x, u32 y
) {
  this->texture.update(pixels, width, height, x, y);
}
void WrapTexture::update(sf::Texture const &texture) {
  this->texture.update(texture);
}
void WrapTexture::update(sf::Texture const &texture, u32 x, u32 y) {
  this->texture.update(texture, x, y);
}
void WrapTexture::update(sf::Image const &image) {
  this->texture.update(image);
}
void WrapTexture::update(sf::Image const &image, u32 x, u32 y) {
  this->texture.update(image, x, y);
}
void WrapTexture::update(sf::Window const &window) {
  this->texture.update(window);
}
void WrapTexture::update(sf::Window const &window, u32 x, u32 y) {
  this->texture.update(window, x, y);
}
