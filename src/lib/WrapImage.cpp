#include "WrapImage.hpp"

WrapImage::WrapImage() {}
WrapImage::WrapImage(std::string const &filename) {
  if (!this->image.loadFromFile(filename)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}
WrapImage::WrapImage(void const *data, usize size) {
  if (!this->image.loadFromMemory(data, size)) {
    throw std::runtime_error("load from memory failed");
  }
}
WrapImage::WrapImage(sf::InputStream &stream) {
  if (!this->image.loadFromStream(stream)) {
    throw std::runtime_error("load from stream failed");
  }
}
WrapImage::WrapImage(WrapImage const &rhs) {
  *this = rhs;
}
WrapImage &WrapImage::operator=(WrapImage const &rhs) {
  if (this == &rhs) { return *this; }
  this->image = rhs.image;
  return *this;
}
WrapImage::~WrapImage() {}

WrapImage::operator sf::Image &() { return this->image; }
WrapImage::operator sf::Image *() { return &(this->image); }
WrapImage::operator sf::Image const &() const { return this->image; }
WrapImage::operator sf::Image const *() const { return &(this->image); }

sf::Vector2u WrapImage::getSize() const {
  return this->image.getSize();
}

sf::Color WrapImage::getPixel(u32 x, u32 y) const {
  return this->image.getPixel(x, y);
}
void WrapImage::setPixel(u32 x, u32 y, sf::Color const &color) {
  this->image.setPixel(x, y, color);
}

sf::Uint8 const *WrapImage::getPixelsPtr() const {
  return this->image.getPixelsPtr();
}

void WrapImage::create(
  usize const &width,
  usize const &height,
  sf::Color const &color
) {
  this->image.create(width, height, color);
}
void WrapImage::create(
  usize const &width,
  usize const &height,
  sf::Uint8 const *pixels
) {
  this->image.create(width, height, pixels);
}
void WrapImage::loadFromFile(std::string const &filename) {
  if (!this->image.loadFromFile(filename)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}
void WrapImage::loadFromMemory(void const *data, usize size) {
  if (!this->image.loadFromMemory(data, size)) {
    throw std::runtime_error("load from memory failed");
  }
}
void WrapImage::loadFromStream(sf::InputStream &stream) {
  if (!this->image.loadFromStream(stream)) {
    throw std::runtime_error("load from stream failed");
  }
}

void WrapImage::saveToFile(std::string const &filename) const {
  if (!this->image.saveToFile(filename)) {
    throw std::runtime_error("save to file failed");
  }
}
void WrapImage::saveToMemory(
  std::vector<sf::Uint8> &output,
  std::string const &format
) const {
  if (this->image.saveToMemory(output, format)) {
    throw std::runtime_error("save to memory failed");
  }
}

void WrapImage::createMaskFromColor(
  sf::Color const &color,
  sf::Uint8 alpha
) {
  this->image.createMaskFromColor(color, alpha);
}
void WrapImage::copy(
  sf::Image const &source,
  u32 destX,
  u32 destY,
  sf::IntRect const &sourceRect,
  bool applyAlpha
) {
  if (sourceRect == sf::IntRect(0, 0, 0, 0)) {
    sf::Vector2u size = source.getSize();
    this->image.create(destX + size.x, destY + size.y, sf::Color::Black);
  } else {
    this->image.create(
      destX + sourceRect.width, destY + sourceRect.height, sf::Color::Black
    );
  }
  this->image.copy(source, destX, destY, sourceRect, applyAlpha);
}

void WrapImage::flipHorizontally() {
  this->image.flipHorizontally();
}
void WrapImage::flipVertically() {
  this->image.flipVertically();
}
