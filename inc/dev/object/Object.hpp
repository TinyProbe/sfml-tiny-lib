#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <SFML/Graphics.hpp>

using f32 = float;

class Animation;

class Object {
 public:
  explicit Object() noexcept;
  explicit Object(Object const &rhs) = delete;
  Object &operator=(Object const &rhs) = delete;
  virtual ~Object() noexcept;

  virtual sf::Sprite &getSprite();
  virtual sf::Sprite const &getSprite() const;

  virtual Animation const *const &getAnimation() const;
  virtual void setAnimation(Animation const *const &animation);

  virtual f32 const &getZ() const;
  virtual void setZ(f32 const &z);

  virtual bool const &isVisible() const;
  virtual void setVisible(bool const &visible);

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void release() = 0;

 protected:
  struct Inner {
    sf::Sprite sprite;
    Animation const *animation;
    f32 z;
    bool visible;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit Object(Object::Inner *const &ownership);
  virtual void ownershipCheck() const;
}; // Object

#endif
