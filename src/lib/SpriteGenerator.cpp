#include "SpriteGenerator.hpp"

SpriteGenerator::SpriteGenerator() :
  holding_time(sf::milliseconds(100))
{}
SpriteGenerator::SpriteGenerator(usize const &images_count) :
  holding_time(sf::milliseconds(100))
{
  this->images_store.resize(images_count);
}
SpriteGenerator::SpriteGenerator(WrapImagesStore const &images_store) :
  holding_time(sf::milliseconds(100))
{
  this->images_store.assign(images_store.begin(), images_store.end());
}
SpriteGenerator::SpriteGenerator(SpriteGenerator const &rhs) {
  *this = rhs;
}
SpriteGenerator &SpriteGenerator::operator=(SpriteGenerator const &rhs) {
  if (this == &rhs) { return *this; }
  this->images_store.assign(rhs.images_store.begin(), rhs.images_store.end());
  this->holding_time = rhs.holding_time;
  return *this;
}
SpriteGenerator::~SpriteGenerator() {}

SpriteAtlas SpriteGenerator::generateSpriteAtlas() const {
  AnimeStore animes;
  std::vector<usize> heights;
  usize total_width = 0, total_height = 0;
  for (WrapImages const &images: this->images_store) {
    animes.push_back(Anime());
    heights.push_back(total_height);
    usize current_width = 0, current_height = 0;
    for (WrapImage const &image: images) {
      sf::Vector2u const &size = image.getSize();
      animes.back().push_back(
        Motion({
          sf::IntRect({
            i32(current_width), i32(total_height),
            i32(size.x), i32(size.y),
          }),
          this->holding_time,
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
  for (usize i = 0; i < this->images_store.size(); ++i) {
    for (usize j = 0; j < this->images_store[i].size(); ++j) {
      WrapTexture texture(this->images_store[i][j]);
      sf::RectangleShape rectangle_shape;
      sf::IntRect const &irect = animes[i][j].first;
      rectangle_shape.setTexture(texture);
      rectangle_shape.setPosition(irect.left, heights[i]);
      rectangle_shape.setSize({ f32(irect.width), f32(irect.height) });
      render_texture.draw(rectangle_shape);
    }
  }
  WrapImage image;
  image.copy(render_texture.getTexture().copyToImage());
  image.flipVertically();
  SpriteAtlas sprite_atlas;
  sprite_atlas.loadFromImage(image);
  sprite_atlas.setAnimes(std::move(animes));
  return std::move(sprite_atlas);
}

WrapImagesStore const &SpriteGenerator::getImagesStore() const {
  return this->images_store;
}
void SpriteGenerator::setImagesStore(WrapImagesStore const &images_store) {
  this->images_store.assign(images_store.begin(), images_store.end());
}

usize SpriteGenerator::getImagesCount() const {
  return this->images_store.size();
}
void SpriteGenerator::setImagesCount(usize const &images_count) {
  this->images_store.resize(images_count);
}

usize SpriteGenerator::getImageCount(usize const &images_code) const {
  this->codeCheck(images_code);
  return this->images_store[images_code].size();
}
void SpriteGenerator::setImageCount(
  usize const &images_code,
  usize const &image_count
) {
  this->codeCheck(images_code);
  this->images_store[images_code].resize(image_count);
}

WrapImages const &SpriteGenerator::getImages(usize const &images_code) const {
  this->codeCheck(images_code);
  return this->images_store[images_code];
}
void SpriteGenerator::setImages(
  usize const &images_code,
  WrapImages const &images
) {
  this->codeCheck(images_code);
  this->images_store[images_code].assign(images.begin(), images.end());
}

WrapImage const &SpriteGenerator::getImage(
  usize const &images_code,
  usize const &image_code
) const {
  this->codeCheck(images_code, image_code);
  return this->images_store[images_code][image_code];
}
WrapImage &SpriteGenerator::getImage(
  usize const &images_code,
  usize const &image_code
) {
  this->codeCheck(images_code, image_code);
  return this->images_store[images_code][image_code];
}
void SpriteGenerator::setImage(
  usize const &images_code,
  usize const &image_code,
  WrapImage const &image
) {
  this->codeCheck(images_code, image_code);
  this->images_store[images_code][image_code] = image;
}

void SpriteGenerator::pushBackImage(
  usize const &images_code,
  WrapImage const &image
) {
  this->codeCheck(images_code);
  this->images_store[images_code].push_back(image);
}
WrapImage SpriteGenerator::popBackImage(usize const &images_code) {
  this->codeCheck(images_code, this->images_store.size() - 1);
  WrapImage tmp = std::move(this->images_store[images_code].back());
  this->images_store[images_code].pop_back();
  return std::move(tmp);
}

sf::Time SpriteGenerator::getHoldingTime() const {
  return this->holding_time;
}
void SpriteGenerator::setHoldingTime(sf::Time const &holding_time) {
  this->holding_time = holding_time;
}

void SpriteGenerator::create(
  usize const &width,
  usize const &height,
  sf::Color const &color
) {
  for (WrapImages &images: this->images_store) {
    for (WrapImage &image: images) {
      image.create(width, height, color);
    }
  }
}
void SpriteGenerator::create(
  usize const &width,
  usize const &height,
  sf::Uint8 const *pixels
) {
  for (WrapImages &images: this->images_store) {
    for (WrapImage &image: images) {
      image.create(width, height, pixels);
    }
  }
}
void SpriteGenerator::createMaskFromColor(
  sf::Color const &color,
  sf::Uint8 alpha
) {
  for (WrapImages &images: this->images_store) {
    for (WrapImage &image: images) {
      image.createMaskFromColor(color, alpha);
    }
  }
}
void SpriteGenerator::flipHorizontally() {
  for (WrapImages &images: this->images_store) {
    for (WrapImage &image: images) {
      image.flipHorizontally();
    }
  }
}
void SpriteGenerator::flipVertically() {
  for (WrapImages &images: this->images_store) {
    for (WrapImage &image: images) {
      image.flipVertically();
    }
  }
}

void SpriteGenerator::codeCheck(
  usize const &images_code,
  usize const &image_code
) const {
  if (images_code >= this->images_store.size()) {
    throw std::runtime_error("No exist images_code.");
  }
  if (image_code != -1
    && image_code >= this->images_store[images_code].size())
  {
    throw std::runtime_error("No exist image_code.");
  }
}
