#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "dev/object/Object.hpp"

class Background : public Object {
 public:
  explicit Background();
  explicit Background(sf::Texture const &texture, bool resetRect = false);
  explicit Background(Background const &rhs);
  virtual Background &operator=(Background const &rhs);
  virtual ~Background();

  virtual Background clone() const;

  virtual void initialize() override;
  virtual void update() override;
  virtual void render() override;
  virtual void release() override;

 protected:
  struct Inner : public Object::Inner {
    virtual Inner &operator=(Inner const &rhs) {
      if (this == &rhs) { return *this; }
      dynamic_cast<Object::Inner &>(*this) =
          dynamic_cast<Object::Inner const &>(rhs);
      return *this;
    }
  };

 private:
  virtual void ownershipCheck() const override;

};

#endif
