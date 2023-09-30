#pragma once
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>

using f32 = float;
using u8 = unsigned char;

class Object {
  f32 x;
  f32 y;
  f32 z;
  f32 width;
  f32 height;
  bool visible;
  sf::Color colorate;

public:
  Object() :
    x(), y(), z(),
    width(), height(),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(f32 const &x, f32 const &y, f32 const &z = 0) :
    x(x), y(y), z(z),
    width(), height(),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(f32 const &width, f32 const &height) :
    x(), y(), z(),
    width(width), height(height),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(
    f32 const &x,
    f32 const &y,
    f32 const &width,
    f32 const &height,
    f32 const &z = 0
  ) :
    x(x), y(y), z(z),
    width(width), height(height),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(
    sf::Vector2f const &axis,
    sf::Vector2f const &size,
    f32 const &z = 0
  ) :
    x(axis.x), y(axis.y), z(z),
    width(size.x), height(size.y),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(
    sf::Vector3f const &axis,
    sf::Vector2f const &size
  ) :
    x(axis.x), y(axis.y), z(axis.z),
    width(size.x), height(size.y),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(
    sf::FloatRect const &rect,
    f32 const &z = 0
  ) :
    x(rect.left), y(rect.top), z(z),
    width(rect.width), height(rect.height),
    visible(true), colorate({ 255, 255, 255, 255 })
  {}
  Object(Object const &rhs) { *this = rhs; }
  virtual Object &operator=(Object const &rhs) {
    if (this == &rhs) { return *this; }
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->width = rhs.width;
    this->height = rhs.height;
    return *this;
  }
  virtual ~Object() {}

  virtual f32 const &getX() const { return this->x; }
  virtual f32 const &getY() const { return this->y; }
  virtual f32 const &getZ() const { return this->z; }
  virtual sf::Vector2f getAxis2() const { return { this->x, this->y }; }
  virtual sf::Vector3f getAxis3() const { return { this->x, this->y, this->z }; }
  virtual f32 const &getWidth() const { return this->width; }
  virtual f32 const &getHeight() const { return this->height; }
  virtual sf::Vector2f getSize() const { return { this->width, this->height }; }
  virtual sf::FloatRect getRect() const {
    return { this->x, this->y, this->width, this->height };
  }
  virtual void setX(f32 const &x) { this->x = x; }
  virtual void setY(f32 const &y) { this->y = y; }
  virtual void setZ(f32 const &z) { this->z = z; }
  virtual void setAxis2(sf::Vector2f const &axis) {
    this->x = axis.x;
    this->y = axis.y;
  }
  virtual void setAxis3(sf::Vector3f const &axis) {
    this->x = axis.x;
    this->y = axis.y;
    this->z = axis.z;
  }
  virtual void setWidth(f32 const &width) { this->width = width; }
  virtual void setHeight(f32 const &height) { this->height = height; }
  virtual void setSize(sf::Vector2f const &size) {
    this->width = size.width;
    this->height = size.height;
  }
  virtual void setRect(sf::FloatRect const &rect) {
    this->x = rect.left;
    this->y = rect.top;
    this->width = rect.width;
    this->height = rect.height;
  }

  virtual f32 const &getCenterX() const { return this->x + this->width / 2; }
  virtual f32 const &getCenterY() const { return this->y + this->height / 2; }
  virtual sf::Vector2f getCenterAxis() const {
    return {
      this->x + this->width / 2,
      this->y + this->height / 2
    };
  }
  virtual void setCenterX(f32 const &x) { this->x = x - this->width / 2; }
  virtual void setCenterY(f32 const &y) { this->y = y - this->height / 2; }
  virtual void setCenterAxis(sf::Vector2f const &axis) {
    this->x = axis.x - this->width / 2;
    this->y = axis.y - this->height / 2;
  }
  virtual void setCenterRect(sf::FloatRect const &rect) {
    this->x = rect.left - rect.width / 2;
    this->y = rect.top - rect.height / 2;
    this->width = rect.width;
    this->height = rect.height;
  }

  virtual bool const &isVisible() const { return this->visible; }
  virtual sf::Color const &getColorate() const { return this->colorate; }
  virtual void setVisible(bool const &visible) { this->visible = visible; }
  virtual void setColorate(sf::Color const &colorate) { this->colorate = colorate; }
  virtual void setColorate(
    f32 const &red, f32 const &green,
    f32 const &blue, f32 const &alpha = 1
  ) {
    this->colorate = sf::Color({
      u8(255 * red), u8(255 * green),
      u8(255 * blue), u8(255 * alpha),
    });
  }

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void release() = 0;

};

#endif
