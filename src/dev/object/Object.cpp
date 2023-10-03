#include "Object.hpp"

Object::Object() :
  x(), y(), z(),
  width(), height(),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(f32 const &x, f32 const &y, f32 const &z) :
  x(x), y(y), z(z),
  width(), height(),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(f32 const &width, f32 const &height) :
  x(), y(), z(),
  width(width), height(height),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(
  f32 const &x,
  f32 const &y,
  f32 const &width,
  f32 const &height,
  f32 const &z
) :
  x(x), y(y), z(z),
  width(width), height(height),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(
  sf::Vector2f const &axis,
  sf::Vector2f const &size,
  f32 const &z
) :
  x(axis.x), y(axis.y), z(z),
  width(size.x), height(size.y),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(
  sf::Vector3f const &axis,
  sf::Vector2f const &size
) :
  x(axis.x), y(axis.y), z(axis.z),
  width(size.x), height(size.y),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(
  sf::FloatRect const &rect,
  f32 const &z
) :
  x(rect.left), y(rect.top), z(z),
  width(rect.width), height(rect.height),
  visible(true), colorate({ 255, 255, 255, 255 })
{}
Object::Object(Object const &rhs) {
  *this = rhs;
}
Object &Object::operator=(Object const &rhs) {
  if (this == &rhs) { return *this; }
  this->x = rhs.x;
  this->y = rhs.y;
  this->z = rhs.z;
  this->width = rhs.width;
  this->height = rhs.height;
  return *this;
}
Object::~Object() {}

f32 const &Object::getX() const {
  return this->x;
}
f32 const &Object::getY() const {
  return this->y;
}
f32 const &Object::getZ() const {
  return this->z;
}
sf::Vector2f Object::getAxis2() const {
  return { this->x, this->y };
}
sf::Vector3f Object::getAxis3() const {
  return { this->x, this->y, this->z };
}
f32 const &Object::getWidth() const {
  return this->width;
}
f32 const &Object::getHeight() const {
  return this->height;
}
sf::Vector2f Object::getSize() const {
  return { this->width, this->height };
}
sf::FloatRect Object::getRect() const {
  return { this->x, this->y, this->width, this->height };
}
void Object::setX(f32 const &x) {
  this->x = x;
}
void Object::setY(f32 const &y) {
  this->y = y;
}
void Object::setZ(f32 const &z) {
  this->z = z;
}
void Object::setAxis2(sf::Vector2f const &axis) {
  this->x = axis.x;
  this->y = axis.y;
}
void Object::setAxis3(sf::Vector3f const &axis) {
  this->x = axis.x;
  this->y = axis.y;
  this->z = axis.z;
}
void Object::setWidth(f32 const &width) {
  this->width = width;
}
void Object::setHeight(f32 const &height) {
  this->height = height;
}
void Object::setSize(sf::Vector2f const &size) {
  this->width = size.width;
  this->height = size.height;
}
void Object::setRect(sf::FloatRect const &rect) {
  this->x = rect.left;
  this->y = rect.top;
  this->width = rect.width;
  this->height = rect.height;
}

f32 const &Object::getCenterX() const {
  return this->x + this->width / 2;
}
f32 const &Object::getCenterY() const {
  return this->y + this->height / 2;
}
sf::Vector2f Object::getCenterAxis() const {
  return {
    this->x + this->width / 2,
    this->y + this->height / 2
  };
}
void Object::setCenterX(f32 const &x) {
  this->x = x - this->width / 2;
}
void Object::setCenterY(f32 const &y) {
  this->y = y - this->height / 2;
}
void Object::setCenterAxis(sf::Vector2f const &axis) {
  this->x = axis.x - this->width / 2;
  this->y = axis.y - this->height / 2;
}
void Object::setCenterRect(sf::FloatRect const &rect) {
  this->x = rect.left - rect.width / 2;
  this->y = rect.top - rect.height / 2;
  this->width = rect.width;
  this->height = rect.height;
}

bool const &Object::isVisible() const {
  return this->visible;
}
sf::Color const &Object::getColorate() const {
  return this->colorate;
}
void Object::setVisible(bool const &visible) {
  this->visible = visible;
}
void Object::setColorate(sf::Color const &colorate) {
  this->colorate = colorate;
}
void Object::setColorate(
  f32 const &red, f32 const &green,
  f32 const &blue, f32 const &alpha
) {
  this->colorate = sf::Color({
    u8(255 * red), u8(255 * green),
    u8(255 * blue), u8(255 * alpha),
  });
}
