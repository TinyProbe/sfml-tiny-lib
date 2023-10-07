#include "dev/object/Object.hpp"

Object::Object() noexcept {
  ownership->z = 1.0f;
  ownership->visible = true;
  ownership->animation = nullptr;
}

Object::Object(sf::Texture const &texture, bool resetRect) noexcept {
  ownership->sprite.setTexture(texture, resetRect);
  ownership->z = 1.0f;
  ownership->visible = true;
  ownership->animation = nullptr;
}

Object::~Object() noexcept {
}

sf::Sprite &Object::getSprite() {
  this->ownershipCheck();
  return ownership->sprite;
}

sf::Sprite const &Object::getSprite() const {
  this->ownershipCheck();
  return ownership->sprite;
}

Animation const *const &Object::getAnimation() const {
  this->ownershipCheck();
  return ownership->animation;
}

void Object::setAnimation(Animation const *const &animation) {
  this->ownershipCheck();
  ownership->animation = animation;
}

f32 const &Object::getZ() const {
  this->ownershipCheck();
  return ownership->z;
}

void Object::setZ(f32 const &z) {
  this->ownershipCheck();
  ownership->z = z;
}

bool const &Object::isVisible() const {
  this->ownershipCheck();
  return ownership->visible;
}

void Object::setVisible(bool const &visible) {
  this->ownershipCheck();
  ownership->visible = visible;
}

void Object::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: Object");
  }
}
