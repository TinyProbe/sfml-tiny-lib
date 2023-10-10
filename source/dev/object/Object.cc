#include <dev/object/Object.h>

sf::Vector2f const &Object::getPosition() const {
  return ownership->position_;
}

f32 const &Object::getZDepth() const {
  return ownership->z_depth_;
}

f32 const &Object::getRotation() const {
  return ownership->rotation_;
}

sf::Vector2f const &Object::getScale() const {
  return ownership->scale_;
}

sf::Vector2f const &Object::getOrigin() const {
  return ownership->origin_;
}

void Object::setPosition(f32 x, f32 y) {
  ownership->position_ = sf::Vector2f(x, y);
}

void Object::setPosition(sf::Vector2f const &position) {
  ownership->position_ = position;
}

void Object::setZDepth(f32 const &z_depth) {
  ownership->z_depth_ = z_depth;
}

void Object::setRotation(f32 const &angle) {
  ownership->rotation_ = angle;
}

void Object::setScale(f32 factor_x, f32 factor_y) {
  ownership->scale_ = sf::Vector2f(factor_x, factor_y);
}

void Object::setScale(sf::Vector2f const &factor) {
  ownership->scale_ = factor;
}

void Object::setOrigin(f32 x, f32 y) {
  ownership->origin_ = sf::Vector2f(x, y);
}

void Object::setOrigin(sf::Vector2f const &origin) {
  ownership->origin_ = origin;
}

Object::Inner::Inner()
    : position_(),
      z_depth_(),
      rotation_(),
      scale_(),
      origin_() {
}

Object::Inner::Inner(Object::Inner const &rhs) {
  *this = rhs;
}

Object::Inner &Object::Inner::operator=(Object::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->position_ = rhs.position_;
  this->z_depth_ = rhs.z_depth_;
  this->rotation_ = rhs.rotation_;
  this->scale_ = rhs.scale_;
  this->origin_ = rhs.origin_;
  return *this;
}
