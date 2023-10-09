#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <SFML/Graphics.hpp>

using f32 = float;

class Object {
 public:
  explicit Object() = delete;
  explicit Object(Object const &rhs) = delete;
  Object &operator=(Object const &rhs) = delete;
  ~Object() = delete;

  virtual sf::Vector2f const &getPosition() const;
  virtual f32 const &getZDepth() const;
  virtual f32 const &getRotation() const;
  virtual sf::Vector2f const &getScale() const;
  virtual sf::Vector2f const &getOrigin() const;
  virtual void setPosition(f32 x, f32 y);
  virtual void setPosition(sf::Vector2f const &position);
  virtual void setZDepth(f32 const &z_depth);
  virtual void setRotation(f32 const &angle);
  virtual void setScale(f32 factor_x, f32 factor_y);
  virtual void setScale(sf::Vector2f const &factor);
  virtual void setOrigin(f32 x, f32 y);
  virtual void setOrigin(sf::Vector2f const &origin);

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void release() = 0;

 protected:
  struct Inner {
    sf::Vector2f position_;
    f32 z_depth_;
    f32 rotation_;
    sf::Vector2f scale_;
    sf::Vector2f origin_;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit Object(Object::Inner *const &ownership) = delete;
  virtual void ownershipCheck() const = 0;
}; // Object

#endif
