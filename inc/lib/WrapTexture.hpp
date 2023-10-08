#ifndef WRAP_TEXTURE_HPP_
#define WRAP_TEXTURE_HPP_

#include <SFML/Graphics.hpp>

#include "lib/WrapImage.hpp"

using usize = unsigned long;
using u32 = unsigned int;

class WrapTexture {
 public:
  explicit WrapTexture();
  explicit WrapTexture(std::string const &filename,
                       sf::IntRect const &area = sf::IntRect());
  explicit WrapTexture(void const *data, usize size,
                       sf::IntRect const &area = sf::IntRect());
  explicit WrapTexture(sf::InputStream &stream,
                       sf::IntRect const &area = sf::IntRect());
  explicit WrapTexture(sf::Image const &image,
                       sf::IntRect const &area = sf::IntRect());
  explicit WrapTexture(sf::Texture const &texture);
  explicit WrapTexture(WrapTexture const &rhs) noexcept;
  virtual WrapTexture &operator=(WrapTexture const &rhs) noexcept;
  virtual ~WrapTexture() noexcept;

  virtual WrapTexture clone() const;

  virtual sf::Texture &getTexture();
  virtual sf::Texture const &getTexture() const;

  virtual sf::Vector2u getSize() const;

  virtual bool isSmooth() const;
  virtual void setSmooth(bool smooth);

  virtual bool isSrgb() const;
  virtual void setSrgb(bool sRgb);

  virtual bool isRepeated() const;
  virtual void setRepeated(bool repeated);

  virtual u32 getNativeHandle() const;

  virtual void create(u32 width, u32 height);

  virtual void loadFromFile(std::string const &filename,
                            sf::IntRect const &area = sf::IntRect());
  virtual void loadFromMemory(void const *data, usize size,
                              sf::IntRect const &area = sf::IntRect());
  virtual void loadFromStream(sf::InputStream &stream,
                              sf::IntRect const &area = sf::IntRect());
  virtual void loadFromImage(sf::Image const &image,
                             sf::IntRect const &area = sf::IntRect());

  virtual WrapImage copyToImage() const;

  virtual void swap(sf::Texture &right);
  virtual void generateMipmap();

  virtual void update(sf::Uint8 const *pixels);
  virtual void update(sf::Uint8 const *pixels,
                      u32 width, u32 height, u32 x, u32 y);
  virtual void update(sf::Texture const &texture);
  virtual void update(sf::Texture const &texture, u32 x, u32 y);
  virtual void update(sf::Image const &image);
  virtual void update(sf::Image const &image, u32 x, u32 y);
  virtual void update(sf::Window const &window);
  virtual void update(sf::Window const &window, u32 x, u32 y);

 protected:
  struct Inner {
    sf::Texture texture;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit WrapTexture(WrapTexture::Inner *const &ownership) noexcept;
  virtual void ownershipCheck() const;

}; // WrapTexture

#endif
