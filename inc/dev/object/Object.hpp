#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <SFML/Graphics.hpp>

using f32 = float;

class Animation;

class Object {
 public:
  explicit Object();
  explicit Object(sf::Texture const &texture, bool resetRect = false);
  explicit Object(Object const &rhs) = delete;
  virtual Object &operator=(Object const &rhs) = delete;
  virtual ~Object();

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
  virtual void ownershipCheck() const;

 private:
  struct Inner {
    sf::Sprite sprite;
    Animation const *animation;
    f32 z;
    bool visible;
  } *ownership;
}; // Object

#endif
