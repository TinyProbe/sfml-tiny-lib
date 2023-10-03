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
  Object();
  Object(f32 const &x, f32 const &y, f32 const &z = 0);
  Object(f32 const &width, f32 const &height);
  Object(
    f32 const &x,
    f32 const &y,
    f32 const &width,
    f32 const &height,
    f32 const &z = 0
  );
  Object(
    sf::Vector2f const &axis,
    sf::Vector2f const &size,
    f32 const &z = 0
  );
  Object(
    sf::Vector3f const &axis,
    sf::Vector2f const &size
  );
  Object(
    sf::FloatRect const &rect,
    f32 const &z = 0
  );
  Object(Object const &rhs);
  virtual Object &operator=(Object const &rhs);
  virtual ~Object();

  virtual f32 const &getX() const;
  virtual f32 const &getY() const;
  virtual f32 const &getZ() const;
  virtual sf::Vector2f getAxis2() const;
  virtual sf::Vector3f getAxis3() const;
  virtual f32 const &getWidth() const;
  virtual f32 const &getHeight() const;
  virtual sf::Vector2f getSize() const;
  virtual sf::FloatRect getRect() const;
  virtual void setX(f32 const &x);
  virtual void setY(f32 const &y);
  virtual void setZ(f32 const &z);
  virtual void setAxis2(sf::Vector2f const &axis);
  virtual void setAxis3(sf::Vector3f const &axis);
  virtual void setWidth(f32 const &width);
  virtual void setHeight(f32 const &height);
  virtual void setSize(sf::Vector2f const &size);
  virtual void setRect(sf::FloatRect const &rect);

  virtual f32 const &getCenterX() const;
  virtual f32 const &getCenterY() const;
  virtual sf::Vector2f getCenterAxis() const;
  virtual void setCenterX(f32 const &x);
  virtual void setCenterY(f32 const &y);
  virtual void setCenterAxis(sf::Vector2f const &axis);
  virtual void setCenterRect(sf::FloatRect const &rect);

  virtual bool const &isVisible() const;
  virtual sf::Color const &getColorate() const;
  virtual void setVisible(bool const &visible);
  virtual void setColorate(sf::Color const &colorate);
  virtual void setColorate(
    f32 const &red, f32 const &green,
    f32 const &blue, f32 const &alpha = 1
  );

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void release() = 0;

};

#endif
