#include "dev/object/Background.hpp"

Background::Background()
    : ownership(new Background::Inner()),
      Object() {
}

Background::Background(sf::Texture const &texture, bool resetRect = false)
    : ownership(new Background::Inner()),
      Object(texture, resetRect) {
}

Background::Background(Background const &rhs)
    : ownership() {
  *this = rhs;
}

Background &Background::operator=(Background const &rhs) {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<Background &>(rhs).ownership = nullptr;
  return *this;
}

Background::~Background() {
  if (ownership != nullptr) { delete ownership; }
}

Background Background::clone() const {
  Background result(*this);
  if (result.ownership != nullptr) {
    ownership = new Background::Inner();
    *ownership = *result.ownership;
  }
  return Background(result);
}

void Background::initialize() {
}

void Background::update() {
}

void Background::render() {
}

void Background::release() {
}

void Background::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: Background");
  }
}
