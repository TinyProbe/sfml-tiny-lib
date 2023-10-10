#ifndef SFML_DEV_OBJECT_VISIBLE_H_
#define SFML_DEV_OBJECT_VISIBLE_H_

#include <dev/object/Object.h>

class Visible : virtual public Object {
 public:
  explicit Visible() = delete;
  explicit Visible(Visible const &rhs) = delete;
  Visible &operator=(Visible const &rhs) = delete;
  ~Visible() = delete;

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
  explicit Visible(Visible::Inner *const &ownership) = delete;
  virtual void ownershipCheck() const = 0;
}; // Visible

#endif // SFML_DEV_OBJECT_VISIBLE_H_
