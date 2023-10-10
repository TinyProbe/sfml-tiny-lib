#include <dev/object/Visible.h>

Visible::Inner::Inner()
    : Object::Inner() {
}

Visible::Inner::Inner(Visible::Inner const &rhs) {
  *this = rhs;
}

Visible::Inner &Visible::Inner::operator=(Visible::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  dynamic_cast<Object::Inner &>(*this) =
      dynamic_cast<Object::Inner const &>(rhs);

  return *this;
}
