#include "lib/SpriteGenerator.hpp"

#include <stdexcept>
#include <algorithm>

SpriteGenerator::SpriteGenerator()
    : ownership(new SpriteGenerator::Inner()) {
}

SpriteGenerator::SpriteGenerator(usize const &images_count)
    : ownership(new SpriteGenerator::Inner()) {
  ownership->images_store_.resize(images_count);
}

SpriteGenerator::SpriteGenerator(WrapImagesStore const &images_store)
    : ownership(new SpriteGenerator::Inner()) {
  ownership->images_store_.assign(images_store.begin(), images_store.end());
}

SpriteGenerator::SpriteGenerator(SpriteGenerator const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

SpriteGenerator &SpriteGenerator::operator=(SpriteGenerator const &rhs
                                            ) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<SpriteGenerator &>(rhs).ownership = nullptr;
  return *this;
}

SpriteGenerator::~SpriteGenerator() noexcept {
  if (ownership != nullptr) { delete ownership; }
}

SpriteGenerator SpriteGenerator::clone() const {
  this->ownershipCheck();
  return SpriteGenerator(new SpriteGenerator::Inner(*ownership));
}

WrapTexture SpriteGenerator::generateSpriteSheet() const {
  this->ownershipCheck();
  std::vector<std::vector<sf::IntRect>> animes;
  std::vector<usize> heights;
  usize total_width = 0, total_height = 0;
  for (WrapImages const &images : ownership->images_store_) {
    animes.push_back(std::vector<sf::IntRect>());
    heights.push_back(total_height);
    usize current_width = 0, current_height = 0;
    for (WrapImage const &image : images) {
      sf::Vector2u const &size = image.getSize();
      animes.back().push_back(
        sf::IntRect({
          i32(current_width), i32(total_height), i32(size.x), i32(size.y),
        })
      );
      current_width += size.x;
      current_height = std::max(current_height, total_height + usize(size.y));
    }
    total_width = std::max(total_width, current_width);
    total_height = current_height;
  }
  sf::RenderTexture render_texture;
  render_texture.create(total_width, total_height);
  for (usize i = 0; i < ownership->images_store_.size(); ++i) {
    for (usize j = 0; j < ownership->images_store_[i].size(); ++j) {
      WrapTexture texture(ownership->images_store_[i][j].getImage());
      sf::IntRect const &int_rect = animes[i][j];
      sf::RectangleShape rectangle_shape;
      rectangle_shape.setTexture(&texture.getTexture());
      rectangle_shape.setPosition(int_rect.left, heights[i]);
      rectangle_shape.setSize({ f32(int_rect.width), f32(int_rect.height) });
      render_texture.draw(rectangle_shape);
    }
  }
  WrapImage image(render_texture.getTexture().copyToImage());
  image.flipVertically();
  return WrapTexture(image.getImage());
}

WrapImagesStore const &SpriteGenerator::getImagesStore() const {
  this->ownershipCheck();
  return ownership->images_store_;
}

void SpriteGenerator::setImagesStore(WrapImagesStore const &images_store) {
  this->ownershipCheck();
  ownership->images_store_.assign(images_store.begin(), images_store.end());
}

usize SpriteGenerator::getImagesCount() const {
  this->ownershipCheck();
  return ownership->images_store_.size();
}

void SpriteGenerator::setImagesCount(usize const &images_count) {
  this->ownershipCheck();
  ownership->images_store_.resize(images_count);
}

usize SpriteGenerator::getImageCount(usize const &images_code) const {
  this->codeCheck(images_code);
  return ownership->images_store_[images_code].size();
}

void SpriteGenerator::setImageCount(usize const &images_code,
                                    usize const &image_count) {
  this->codeCheck(images_code);
  ownership->images_store_[images_code].resize(image_count);
}

WrapImages const &SpriteGenerator::getImages(usize const &images_code) const {
  this->codeCheck(images_code);
  return ownership->images_store_[images_code];
}

void SpriteGenerator::setImages(usize const &images_code,
                                WrapImages const &images) {
  this->codeCheck(images_code);
  ownership->images_store_[images_code].assign(images.begin(), images.end());
}

WrapImage const &SpriteGenerator::getImage(usize const &images_code,
                                           usize const &image_code) const {
  this->codeCheck(images_code, image_code);
  return ownership->images_store_[images_code][image_code];
}

WrapImage &SpriteGenerator::getImage(usize const &images_code,
                                     usize const &image_code) {
  this->codeCheck(images_code, image_code);
  return ownership->images_store_[images_code][image_code];
}

void SpriteGenerator::setImage(usize const &images_code,
                               usize const &image_code,
                               WrapImage const &image) {
  this->codeCheck(images_code, image_code);
  ownership->images_store_[images_code][image_code] = image;
}

void SpriteGenerator::pushBackImage(usize const &images_code,
                                    WrapImage const &image) {
  this->codeCheck(images_code);
  ownership->images_store_[images_code].push_back(image);
}

WrapImage SpriteGenerator::popBackImage(usize const &images_code) {
  this->codeCheck(images_code, ownership->images_store_.size() - 1);
  WrapImage tmp(ownership->images_store_[images_code].back());
  ownership->images_store_[images_code].pop_back();
  return WrapImage(tmp);
}

void SpriteGenerator::create(usize const &width,
                             usize const &height,
                             sf::Color const &color) {
  this->ownershipCheck();
  for (WrapImages &images : ownership->images_store_) {
    for (WrapImage &image : images) {
      image.create(width, height, color);
    }
  }
}

void SpriteGenerator::create(usize const &width,
                             usize const &height,
                             sf::Uint8 const *pixels) {
  this->ownershipCheck();
  for (WrapImages &images : ownership->images_store_) {
    for (WrapImage &image : images) {
      image.create(width, height, pixels);
    }
  }
}

void SpriteGenerator::createMaskFromColor(sf::Color const &color,
                                          sf::Uint8 alpha) {
  this->ownershipCheck();
  for (WrapImages &images : ownership->images_store_) {
    for (WrapImage &image : images) {
      image.createMaskFromColor(color, alpha);
    }
  }
}

void SpriteGenerator::flipHorizontally() {
  this->ownershipCheck();
  for (WrapImages &images : ownership->images_store_) {
    for (WrapImage &image : images) {
      image.flipHorizontally();
    }
  }
}

void SpriteGenerator::flipVertically() {
  this->ownershipCheck();
  for (WrapImages &images : ownership->images_store_) {
    for (WrapImage &image : images) {
      image.flipVertically();
    }
  }
}

SpriteGenerator::Inner::Inner() {
}

SpriteGenerator::Inner::Inner(SpriteGenerator::Inner const &rhs) {
  *this = rhs;
}

SpriteGenerator::Inner &SpriteGenerator::Inner::operator=(
    SpriteGenerator::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->images_store_.assign(rhs.images_store_.begin(),
                            rhs.images_store_.end());
  return *this;
}

SpriteGenerator::SpriteGenerator(
    SpriteGenerator::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void SpriteGenerator::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error(
        "No ownership rights whatsoever: SpriteGenerator");
  }
}

void SpriteGenerator::codeCheck(usize const &images_code,
                                usize const &image_code) const {
  this->ownershipCheck();
  if (images_code >= ownership->images_store_.size()) {
    throw std::runtime_error("No exist images_code.");
  }
  if (image_code != -1 &&
      image_code >= ownership->images_store_[images_code].size()) {
    throw std::runtime_error("No exist image_code.");
  }
}
