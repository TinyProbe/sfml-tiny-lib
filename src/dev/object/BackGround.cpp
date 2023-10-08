#include "dev/object/Background.hpp"

Background::Background()
    : ownership(new Background::Inner()) {
}

Background::Background(sf::Texture const &texture, bool resetRect)
    : ownership(new Background::Inner()) {
  ownership->sprite.setTexture(texture, resetRect);
}

Background::Background(Background const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

Background &Background::operator=(Background const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<Background &>(rhs).ownership = nullptr;
  return *this;
}

Background::~Background() noexcept {
  if (ownership != nullptr) { delete ownership; }
}

Background Background::clone() const {
  this->ownershipCheck();
  return Background(new Background::Inner(*ownership));
}

void Background::initialize() {
}

void Background::update() {
}

void Background::render() {
}

void Background::release() {
}

Background::Inner::Inner()
    : Object::Inner() {
}

Background::Inner::Inner(Background::Inner const &rhs) {
  *this = rhs;
}

Background::Inner &Background::Inner::operator=(Background::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  dynamic_cast<Object::Inner &>(*this) =
      dynamic_cast<Object::Inner const &>(rhs);
  return *this;
}

Background::Background(Background::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void Background::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: Background");
  }
}
