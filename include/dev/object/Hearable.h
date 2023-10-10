#ifndef SFML_DEV_OBJECT_HEARABLE_H_
#define SFML_DEV_OBJECT_HEARABLE_H_

#include <dev/object/Object.h>

class Hearable : virtual public Object {
 public:
  explicit Hearable() = delete;
  explicit Hearable(Hearable const &rhs) = delete;
  Hearable &operator=(Hearable const &rhs) = delete;
  ~Hearable() = delete;

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
  explicit Hearable(Hearable::Inner *const &ownership) = delete;
  virtual void ownershipCheck() const = 0;
}; // Hearable

#endif // SFML_DEV_OBJECT_HEARABLE_H_
