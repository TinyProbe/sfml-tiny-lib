#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "dev/object/Object.hpp"

class Background : public Object {
 public:
  explicit Background();
  explicit Background(sf::Texture const &texture, bool resetRect = false);
  explicit Background(Background const &rhs) noexcept;
  virtual Background &operator=(Background const &rhs) noexcept;
  virtual ~Background() noexcept;

  virtual Background clone() const;

  virtual void initialize() override;
  virtual void update() override;
  virtual void render() override;
  virtual void release() override;

 protected:
  struct Inner : public Object::Inner {

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  };

 private:
  explicit Background(Background::Inner *const &ownership) noexcept;
  virtual void ownershipCheck() const override;
};

#endif
