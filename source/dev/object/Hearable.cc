#include <dev/object/Hearable.h>

Hearable::Inner::Inner()
    : Object::Inner() {
}

Hearable::Inner::Inner(Hearable::Inner const &rhs) {
  *this = rhs;
}

Hearable::Inner &Hearable::Inner::operator=(Hearable::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  dynamic_cast<Object::Inner &>(*this) =
      dynamic_cast<Object::Inner const &>(rhs);

  return *this;
}
