#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <SFML/Graphics.hpp>

using f32 = float;

class Animation;

class Object {
 public:
  explicit Object() noexcept;
  explicit Object(sf::Texture const &texture, bool resetRect = false) noexcept;
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

    virtual Inner &operator=(Inner const &rhs) {
      if (this == &rhs) { return *this; }
      this->sprite = rhs.sprite;
      this->animation = rhs.animation;
      this->z = rhs.z;
      this->visible = rhs.visible;
      return *this;
    }
  } *ownership;

 private:
  virtual void ownershipCheck() const;

}; // Object

#endif
