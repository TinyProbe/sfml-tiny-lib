#include "lib/WrapImage.hpp"

#include <stdexcept>

WrapImage::WrapImage()
    : ownership(new WrapImage::Inner()) {
}

WrapImage::WrapImage(std::string const &filename)
    : ownership(new WrapImage::Inner()) {
  this->loadFromFile(filename);
}

WrapImage::WrapImage(void const *data, usize size)
    : ownership(new WrapImage::Inner()) {
  this->loadFromMemory(data, size);
}

WrapImage::WrapImage(sf::InputStream &stream)
    : ownership(new WrapImage::Inner()) {
  this->loadFromStream(stream);
}

WrapImage::WrapImage(sf::Image const &image)
    : ownership(new WrapImage::Inner()) {
  ownership->image_ = image;
}

WrapImage::WrapImage(WrapImage const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

WrapImage &WrapImage::operator=(WrapImage const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<WrapImage &>(rhs).ownership = nullptr;
  return *this;
}

WrapImage::~WrapImage() noexcept {
  if (ownership != nullptr) { delete ownership; }
}

WrapImage WrapImage::clone() const {
  this->ownershipCheck();
  return WrapImage(new WrapImage::Inner(*ownership));
}

sf::Image &WrapImage::getImage() {
  this->ownershipCheck();
  return ownership->image_;
}

sf::Image const &WrapImage::getImage() const {
  this->ownershipCheck();
  return ownership->image_;
}

sf::Vector2u WrapImage::getSize() const {
  this->ownershipCheck();
  return ownership->image_.getSize();
}

sf::Color WrapImage::getPixel(u32 x, u32 y) const {
  this->ownershipCheck();
  return ownership->image_.getPixel(x, y);
}

void WrapImage::setPixel(u32 x, u32 y, sf::Color const &color) {
  this->ownershipCheck();
  ownership->image_.setPixel(x, y, color);
}

sf::Uint8 const *WrapImage::getPixelsPtr() const {
  this->ownershipCheck();
  return ownership->image_.getPixelsPtr();
}

void WrapImage::create(usize const &width,
                       usize const &height,
                       sf::Color const &color) {
  this->ownershipCheck();
  ownership->image_.create(width, height, color);
}

void WrapImage::create(usize const &width,
                       usize const &height,
                       sf::Uint8 const *pixels) {
  this->ownershipCheck();
  ownership->image_.create(width, height, pixels);
}

void WrapImage::copy(sf::Image const &source,
                     u32 destX,
                     u32 destY,
                     sf::IntRect const &sourceRect,
                     bool applyAlpha) {
  this->ownershipCheck();
  if (sourceRect == sf::IntRect(0, 0, 0, 0)) {
    sf::Vector2u size = source.getSize();
    ownership->image_.create(destX + size.x, destY + size.y, sf::Color::Black);
  } else {
    ownership->image_.create(destX + sourceRect.width,
                             destY + sourceRect.height,
                             sf::Color::Black);
  }
  ownership->image_.copy(source, destX, destY, sourceRect, applyAlpha);
}

void WrapImage::loadFromFile(std::string const &filename) {
  this->ownershipCheck();
  if (!ownership->image_.loadFromFile(filename)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}

void WrapImage::loadFromMemory(void const *data, usize size) {
  this->ownershipCheck();
  if (!ownership->image_.loadFromMemory(data, size)) {
    throw std::runtime_error("load from memory failed");
  }
}

void WrapImage::loadFromStream(sf::InputStream &stream) {
  this->ownershipCheck();
  if (!ownership->image_.loadFromStream(stream)) {
    throw std::runtime_error("load from stream failed");
  }
}

void WrapImage::saveToFile(std::string const &filename) const {
  this->ownershipCheck();
  if (!ownership->image_.saveToFile(filename)) {
    throw std::runtime_error("save to file failed");
  }
}

void WrapImage::saveToMemory(std::vector<sf::Uint8> &output,
                             std::string const &format) const {
  this->ownershipCheck();
  if (!ownership->image_.saveToMemory(output, format)) {
    throw std::runtime_error("save to memory failed");
  }
}

void WrapImage::createMaskFromColor(sf::Color const &color,
                                    sf::Uint8 alpha) {
  this->ownershipCheck();
  ownership->image_.createMaskFromColor(color, alpha);
}

void WrapImage::flipHorizontally() {
  this->ownershipCheck();
  ownership->image_.flipHorizontally();
}

void WrapImage::flipVertically() {
  this->ownershipCheck();
  ownership->image_.flipVertically();
}

WrapImage::Inner::Inner() {
}

WrapImage::Inner::Inner(WrapImage::Inner const &rhs) {
  *this = rhs;
}

WrapImage::Inner &WrapImage::Inner::operator=(WrapImage::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->image_ = rhs.image_;
  return *this;
}

WrapImage::WrapImage(WrapImage::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void WrapImage::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: WrapImage");
  }
}
