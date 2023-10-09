#ifndef WRAP_IMAGE_HPP_
#define WRAP_IMAGE_HPP_

#include <SFML/Graphics.hpp>

using usize = unsigned long;
using u32 = unsigned int;

class WrapImage {
 public:
  explicit WrapImage();
  explicit WrapImage(std::string const &filename);
  explicit WrapImage(void const *data, usize size);
  explicit WrapImage(sf::InputStream &stream);
  explicit WrapImage(sf::Image const &image);
  explicit WrapImage(WrapImage const &rhs) noexcept;
  virtual WrapImage &operator=(WrapImage const &rhs) noexcept;
  virtual ~WrapImage() noexcept;

  virtual WrapImage clone() const;

  virtual sf::Image &getImage();
  virtual sf::Image const &getImage() const;

  virtual sf::Vector2u getSize() const;

  virtual sf::Color getPixel(u32 x, u32 y) const;
  virtual void setPixel(u32 x, u32 y, sf::Color const &color);

  virtual sf::Uint8 const *getPixelsPtr() const;

  virtual void create(usize const &width,
                      usize const &height,
                      sf::Color const &color = sf::Color(0, 0, 0));
  virtual void create(usize const &width,
                      usize const &height,
                      sf::Uint8 const *pixels);
  virtual void copy(sf::Image const &source,
                    u32 destX = 0,
                    u32 destY = 0,
                    sf::IntRect const &sourceRect = sf::IntRect(0, 0, 0, 0),
                    bool applyAlpha = false);

  virtual void loadFromFile(std::string const &filename);
  virtual void loadFromMemory(void const *data, usize size);
  virtual void loadFromStream(sf::InputStream &stream);

  virtual void saveToFile(std::string const &filename) const;
  virtual void saveToMemory(std::vector<sf::Uint8> &output,
                            std::string const &format) const;

  virtual void createMaskFromColor(sf::Color const &color,
                                   sf::Uint8 alpha = 0);

  virtual void flipHorizontally();
  virtual void flipVertically();

 protected:
  struct Inner {
    sf::Image image_;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit WrapImage(WrapImage::Inner *const &ownership) noexcept;
  virtual void ownershipCheck() const;
}; // WrapImage

#endif
