#pragma once
#ifndef AUTO_IMAGE_HPP
#define AUTO_IMAGE_HPP

#include <stdexcept>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

using usize = unsigned long;
using u32 = unsigned int;

class WrapImage {
  sf::Image image;

public:
  WrapImage();
  WrapImage(std::string const &filename);
  WrapImage(void const *data, usize size);
  WrapImage(sf::InputStream &stream);
  WrapImage(WrapImage const &rhs);
  virtual WrapImage &operator=(WrapImage const &rhs);
  virtual ~WrapImage();

  virtual operator sf::Image &();
  virtual operator sf::Image *();
  virtual operator sf::Image const &() const;
  virtual operator sf::Image const *() const;

  virtual sf::Vector2u getSize() const;

  virtual sf::Color getPixel(u32 x, u32 y) const;
  virtual void setPixel(u32 x, u32 y, sf::Color const &color);

  virtual sf::Uint8 const *getPixelsPtr() const;

  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  );
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  );

  virtual void loadFromFile(std::string const &filename);
  virtual void loadFromMemory(void const *data, usize size);
  virtual void loadFromStream(sf::InputStream &stream);

  virtual void saveToFile(std::string const &filename) const;
  virtual void saveToMemory(
    std::vector<sf::Uint8> &output,
    std::string const &format
  ) const;

  virtual void createMaskFromColor(
    sf::Color const &color,
    sf::Uint8 alpha = 0
  );
  virtual void copy(
    sf::Image const &source,
    u32 destX = 0,
    u32 destY = 0,
    sf::IntRect const &sourceRect = sf::IntRect(0, 0, 0, 0),
    bool applyAlpha = false
  );

  virtual void flipHorizontally();
  virtual void flipVertically();

};

#endif
