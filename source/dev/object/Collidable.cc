#include <dev/object/Collidable.h>

Collidable::Inner::Inner()
    : Object::Inner() {
}

Collidable::Inner::Inner(Collidable::Inner const &rhs) {
  *this = rhs;
}

Collidable::Inner &Collidable::Inner::operator=(Collidable::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  dynamic_cast<Object::Inner &>(*this) =
      dynamic_cast<Object::Inner const &>(rhs);

  return *this;
}
