#include "lib/WrapTexture.hpp"

#include <stdexcept>

WrapTexture::WrapTexture()
    : ownership(new WrapTexture::Inner()) {
}

WrapTexture::WrapTexture(std::string const &filename, sf::IntRect const &area)
    : ownership(new WrapTexture::Inner()) {
  this->loadFromFile(filename, area);
}

WrapTexture::WrapTexture(void const *data, usize size, sf::IntRect const &area)
    : ownership(new WrapTexture::Inner()) {
  this->loadFromMemory(data, size, area);
}

WrapTexture::WrapTexture(sf::InputStream &stream, sf::IntRect const &area)
    : ownership(new WrapTexture::Inner()) {
  this->loadFromStream(stream, area);
}

WrapTexture::WrapTexture(sf::Image const &image, sf::IntRect const &area)
    : ownership(new WrapTexture::Inner()) {
  this->loadFromImage(image, area);
}

WrapTexture::WrapTexture(sf::Texture const &texture)
    : ownership(new WrapTexture::Inner()) {
  ownership->texture = texture;
}

WrapTexture::WrapTexture(WrapTexture const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

WrapTexture &WrapTexture::operator=(WrapTexture const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<WrapTexture &>(rhs).ownership = nullptr;
  return *this;
}

WrapTexture::~WrapTexture() noexcept {
  if (ownership != nullptr) { delete ownership; }
}

WrapTexture WrapTexture::clone() {
  WrapTexture result(*this);
  if (result.ownership != nullptr) {
    ownership = new WrapTexture::Inner();
    *ownership = *result.ownership;
  }
  return WrapTexture(result);
}

sf::Texture &WrapTexture::getTexture() {
  this->ownershipCheck();
  return ownership->texture;
}

sf::Texture const &WrapTexture::getTexture() const {
  this->ownershipCheck();
  return ownership->texture;
}

sf::Vector2u WrapTexture::getSize() const {
  this->ownershipCheck();
  return ownership->texture.getSize();
}

bool WrapTexture::isSmooth() const {
  this->ownershipCheck();
  return ownership->texture.isSmooth();
}

void WrapTexture::setSmooth(bool smooth) {
  this->ownershipCheck();
  ownership->texture.setSmooth(smooth);
}

bool WrapTexture::isSrgb() const {
  this->ownershipCheck();
  return ownership->texture.isSrgb();
}

void WrapTexture::setSrgb(bool sRgb) {
  this->ownershipCheck();
  ownership->texture.setSrgb(sRgb);
}

bool WrapTexture::isRepeated() const {
  this->ownershipCheck();
  return ownership->texture.isRepeated();
}

void WrapTexture::setRepeated(bool repeated) {
  this->ownershipCheck();
  ownership->texture.setRepeated(repeated);
}

u32 WrapTexture::getNativeHandle() const {
  this->ownershipCheck();
  return ownership->texture.getNativeHandle();
}

void WrapTexture::create(u32 width, u32 height) {
  this->ownershipCheck();
  ownership->texture.create(width, height);
}

void WrapTexture::loadFromFile(std::string const &filename,
                               sf::IntRect const &area) {
  this->ownershipCheck();
  if (!ownership->texture.loadFromFile(filename, area)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}

void WrapTexture::loadFromMemory(void const *data, usize size,
                                 sf::IntRect const &area) {
  this->ownershipCheck();
  if (!ownership->texture.loadFromMemory(data, size, area)) {
    throw std::runtime_error("load from memory failed");
  }
}

void WrapTexture::loadFromStream(sf::InputStream &stream,
                                 sf::IntRect const &area) {
  this->ownershipCheck();
  if (!ownership->texture.loadFromStream(stream, area)) {
    throw std::runtime_error("load from stream failed");
  }
}
void WrapTexture::loadFromImage(sf::Image const &image,
                                sf::IntRect const &area) {
  this->ownershipCheck();
  if (!ownership->texture.loadFromImage(image, area)) {
    throw std::runtime_error("load from image failed");
  }
}

WrapImage WrapTexture::copyToImage() const {
  this->ownershipCheck();
  return WrapImage(ownership->texture.copyToImage());
}

void WrapTexture::swap(sf::Texture &right) {
  this->ownershipCheck();
  ownership->texture.swap(right);
}

void WrapTexture::generateMipmap() {
  this->ownershipCheck();
  if (!ownership->texture.generateMipmap()) {
    throw std::runtime_error("generate mipmap failed");
  }
}

void WrapTexture::update(sf::Uint8 const *pixels) {
  this->ownershipCheck();
  ownership->texture.update(pixels);
}

void WrapTexture::update(sf::Uint8 const *pixels,
                         u32 width, u32 height, u32 x, u32 y) {
  this->ownershipCheck();
  ownership->texture.update(pixels, width, height, x, y);
}

void WrapTexture::update(sf::Texture const &texture) {
  this->ownershipCheck();
  ownership->texture.update(texture);
}

void WrapTexture::update(sf::Texture const &texture, u32 x, u32 y) {
  this->ownershipCheck();
  ownership->texture.update(texture, x, y);
}

void WrapTexture::update(sf::Image const &image) {
  this->ownershipCheck();
  ownership->texture.update(image);
}

void WrapTexture::update(sf::Image const &image, u32 x, u32 y) {
  this->ownershipCheck();
  ownership->texture.update(image, x, y);
}

void WrapTexture::update(sf::Window const &window) {
  this->ownershipCheck();
  ownership->texture.update(window);
}

void WrapTexture::update(sf::Window const &window, u32 x, u32 y) {
  this->ownershipCheck();
  ownership->texture.update(window, x, y);
}

void WrapTexture::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: WrapTexture");
  }
}
