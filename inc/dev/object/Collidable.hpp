#ifndef COLLIDABLE_HPP_
#define COLLIDABLE_HPP_

#include "dev/object/Object.hpp"

class Collidable : virtual public Object {
 public:
  explicit Collidable() = delete;
  explicit Collidable(Collidable const &rhs) = delete;
  Collidable &operator=(Collidable const &rhs) = delete;
  ~Collidable() = delete;

  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void release() = 0;

 protected:
  struct Inner : virtual public Object::Inner {

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  };

 private:
  explicit Collidable(Collidable::Inner *const &ownership) = delete;
  virtual void ownershipCheck() const = 0;
}; // Collidable

#endif
