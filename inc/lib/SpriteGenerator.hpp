#ifndef SPRITE_GENERATOR_HPP_
#define SPRITE_GENERATOR_HPP_

#include <vector>

#include "lib/WrapImage.hpp"
#include "lib/WrapTexture.hpp"

using i32 = int;
using f32 = float;
using usize = unsigned long;
using WrapImages = std::vector<WrapImage>;
using WrapImagesStore = std::vector<WrapImages>;

class SpriteGenerator {
 public:
  explicit SpriteGenerator();
  explicit SpriteGenerator(usize const &images_count);
  explicit SpriteGenerator(WrapImagesStore const &images_store);
  explicit SpriteGenerator(SpriteGenerator const &rhs) noexcept;
  virtual SpriteGenerator &operator=(SpriteGenerator const &rhs) noexcept;
  virtual ~SpriteGenerator() noexcept;

  virtual SpriteGenerator clone();

  virtual WrapTexture generateSpriteSheet() const;

  virtual WrapImagesStore const &getImagesStore() const;
  virtual void setImagesStore(WrapImagesStore const &images_store);

  virtual usize getImagesCount() const;
  virtual void setImagesCount(usize const &images_count);

  virtual usize getImageCount(usize const &images_code) const;
  virtual void setImageCount(usize const &images_code,
                             usize const &image_count);

  virtual WrapImages const &getImages(usize const &images_code) const;
  virtual void setImages(usize const &images_code,
                         WrapImages const &images);

  virtual WrapImage const &getImage(usize const &images_code,
                                    usize const &image_code) const;
  virtual WrapImage &getImage(usize const &images_code,
                              usize const &image_code);
  virtual void setImage(usize const &images_code,
                        usize const &image_code,
                        WrapImage const &image);

  virtual void pushBackImage(usize const &images_code,
                             WrapImage const &image);
  virtual WrapImage popBackImage(usize const &images_code);

  virtual void create(usize const &width,
                      usize const &height,
                      sf::Color const &color = sf::Color(0, 0, 0));
  virtual void create(usize const &width,
                      usize const &height,
                      sf::Uint8 const *pixels);

  virtual void createMaskFromColor(sf::Color const &color,
                                   sf::Uint8 alpha = 0);

  virtual void flipHorizontally();
  virtual void flipVertically();

 protected:
  struct Inner {
    WrapImagesStore images_store;

    virtual Inner &operator=(Inner const &rhs) {
      if (this == &rhs) { return *this; }
      this->images_store.assign(rhs.images_store.begin(),
                                rhs.images_store.end());
      return *this;
    }
  } *ownership;

 private:
  virtual void ownershipCheck() const;
  virtual void codeCheck(usize const &images_code,
                         usize const &image_code = -1) const;

}; // SpriteGenerator

#endif
