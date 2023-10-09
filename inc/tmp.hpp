// KeyManager.hpp (old)
#pragma once
#ifndef FPS_MANAGER_HPP
#define FPS_MANAGER_HPP

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <thread>
#include <list>

using usize = unsigned long;
using u64 = unsigned long long;
using DurationNano = std::chrono::duration<u64, std::nano>;

// A high precision FPS manager.
class FPSManager {
  usize framerate_limit;
  sf::Clock clock;
  sf::Time term;
  std::list<sf::Time> time_per_frame;

public:
  FPSManager() :
    framerate_limit(usize(-1)),
    term(sf::microseconds(u64(1000000) / this->framerate_limit)),
    time_per_frame({ clock.getElapsedTime() }) {}
  FPSManager(usize const &framerate_limit) :
    framerate_limit(framerate_limit != 0 ? framerate_limit : usize(-1)),
    term(sf::microseconds(u64(1000000) / this->framerate_limit)),
    time_per_frame({ clock.getElapsedTime() }) {}
  FPSManager(FPSManager const &rhs) { *this = rhs; }
  virtual FPSManager &operator=(FPSManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->framerate_limit = rhs.framerate_limit;
    this->clock = rhs.clock;
    this->term = rhs.term;
    this->time_per_frame.assign(rhs.time_per_frame.begin(), rhs.time_per_frame.end());
    return *this;
  }
  virtual ~FPSManager() {}

  virtual usize const &getFramerateLimit() const {
    return this->framerate_limit;
  }
  virtual void setFramerateLimit(usize const &framerate_limit) {
    this->framerate_limit = (framerate_limit != 0 ? framerate_limit : usize(-1));
    this->term = sf::microseconds(u64(1000000) / this->framerate_limit);
  }
  virtual sf::Time const &getTerm() const {
    return this->term;
  }
  virtual usize getCurrentFPS() const {
    return this->time_per_frame.size();
  }
  virtual sf::Time getFrameTime() {
    return this->time_per_frame.back();
  }
  virtual void framePulse() {
    sf::Time now = clock.getElapsedTime();
    sf::Time diff = now - this->time_per_frame.back();
    if (this->term > diff) {
      diff = this->term - diff;
      std::this_thread::sleep_for(DurationNano(u64(diff.asMicroseconds()) * 1000));
      now += diff;
    }
    this->time_per_frame.push_back(now);
    sf::Time cutline = now - sf::milliseconds(999);
    while (this->time_per_frame.front() <= cutline) {
      this->time_per_frame.pop_front();
    }
  }

};

// // Example code
// int main() {
//   // 
//   // create window ...
//   //
//
//   // create FPSManager instance
//   FPSManager fpsmng;
//   fpsmng.setFramerateLimit(60);
//
//   // start event loop
//   while (true) {
//     fpsmng.framePulse();
//     std::cout << fpsmng.getCurrentFPS() << '\n';
//
//     //
//     // do something ...
//     //
//   }
// }

#endif

// ScrManager.hpp
#pragma once
#ifndef SCRMANAGER_HPP
#define SCRMANAGER_HPP

# if _WIN32
#  include <Windows.h>
# elif __APPLE__
#  include <CoreGraphics/CoreGraphics.h>
# elif __linux__
#  include <X11/Xlib.h>
# endif

using usize = unsigned long;

class ScrManager {

public:
  ScrManager() {}
  ScrManager(ScrManager const &rhs) { *this = rhs; }
  virtual ScrManager &operator=(ScrManager const &rhs) {
    if (this == &rhs) { return *this; }
    return *this;
  }
  virtual ~ScrManager() {}

  virtual void changeResolution(usize const &wid, usize const &hei) {
# if _WIN32
    DEVMODE dm({
      .dmSize = sizeof(DEVMODE),
      .dmPelsWidth = wid,
      .dmPelsHeight = hei,
      .dmBitsPerPel = 32,
      .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
    });
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
    }
# elif __APPLE__
    CGDisplayConfigRef config;
    CGBeginDisplayConfiguration(&config);
    CGDirectDisplayID displayID = kCGDirectMainDisplay;
    CFDictionaryRef resolutionMode
      = CGDisplayBestModeForParameters(displayID, 32, wid, hei, NULL);
    CGConfigureDisplayMode(config, displayID, resolutionMode);
    CGCompleteDisplayConfiguration(config, kCGConfigureForSession);
# elif __linux__
    XRRScreenConfiguration* screenConfig
      = XRRGetScreenInfo(display, RootWindow(display, 0));
    if (screenConfig) {
      XRRSetScreenConfig(
        display, screenConfig, RootWindow(display, 1), wid, hei, CurrentTime);
      XRRFreeScreenConfigInfo(screenConfig);
    }
# endif
  }

};

#endif

// AutoSpriteMulti.hpp
#pragma once
#ifndef AUTO_SPRITE_MULTI_HPP
#define AUTO_SPRITE_MULTI_HPP

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "SpriteTextureMulti.hpp"

using usize = unsigned long;
using SpriteMulti = std::vector<sf::RectangleShape>;

class AutoSpriteMulti {
  sf::Clock clock;
  SpriteMulti sprite_multi;
  usize current_motion;
  SpriteTextureMulti const *sprite_texture_multi;

public:
  AutoSpriteMulti() :
    current_motion(),
    sprite_texture_multi()
  {}
  AutoSpriteMulti(SpriteTextureMulti const *const &sprite_texture_multi) :
    current_motion(),
    sprite_texture_multi(sprite_texture_multi)
  {
    if (sprite_texture_multi != nullptr) {
      AnimeMulti const &anime_multi = sprite_texture_multi->getAnimeMulti();
      this->sprite_multi.resize(anime_multi.size());
      for (usize i = 0; i < anime_multi.size(); ++i) {
        this->sprite_multi[i].setTexture(anime_multi[i].first);
      }
    }
  }
  AutoSpriteMulti(AutoSpriteMulti const &rhs) { *this = rhs; }
  virtual AutoSpriteMulti &operator=(AutoSpriteMulti const &rhs) {
    if (this == &rhs) { return *this; }
    this->clock = rhs.clock;
    this->sprite_multi.assign(rhs.sprite_multi.begin(). rhs.sprite_multi.end());
    this->current_motion = rhs.current_motion;
    this->sprite_texture_multi = rhs.sprite_texture_multi;
    return *this;
  }
  virtual ~AutoSpriteMulti() {}

};

#endif

// ImageTexture.hpp
#pragma once
#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include <stdexcept>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

using usize = unsigned long;
using u32 = unsigned int;
using i32 = int;

class ImageTexture {
  sf::Image image;
  sf::Texture texture;

public:
  ImageTexture() {}
  ImageTexture(std::string const &filename) {
    if (!this->image.loadFromFile(filename)) {
      throw std::runtime_error(std::string("load from file failed: ") + filename);
    }
  }
  ImageTexture(void const *data, std::size_t size) {
    if (!this->image.loadFromMemory(data, size)) {
      throw std::runtime_error("load from memory failed");
    }
  }
  ImageTexture(sf::InputStream &stream) {
    if (!this->image.loadFromStream(stream)) {
      throw std::runtime_error("load from stream failed");
    }
  }
  ImageTexture(ImageTexture const &rhs) { *this = rhs; }
  virtual ImageTexture &operator=(ImageTexture const &rhs) {
    if (this == &rhs) { return *this; }
    this->image = rhs.image;
    this->texture = rhs.texture;
    return *this;
  }
  virtual ~ImageTexture() {}

  virtual operator sf::Image const &() const { return this->image; }
  virtual operator sf::Texture const &() const { return this->texture; }
  virtual operator sf::Image const *() const { return &(this->image); }
  virtual operator sf::Texture const *() const { return &(this->texture); }

  virtual void loadFromImage() {
    sf::Vector2u size = this->image.getSize();
    if (!this->texture.loadFromImage(this->image, { 0, 0, i32(size.x), i32(size.y) })) {
      throw std::runtime_error("ImageTexture load failed");
    }
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Color const &color = sf::Color(0, 0, 0)
  ) {
    this->image.create(width, height, color);
  }
  virtual void create(
    usize const &width,
    usize const &height,
    sf::Uint8 const *pixels
  ) {
    this->image.create(width, height, pixels);
  }
  virtual void loadFromFile(std::string const &filename) {
    if (!this->image.loadFromFile(filename)) {
      throw std::runtime_error(std::string("load from file failed: ") + filename);
    }
  }
  virtual void loadFromMemory(void const *data, std::size_t size) {
    if (!this->image.loadFromMemory(data, size)) {
      throw std::runtime_error("load from memory failed");
    }
  }
  virtual void loadFromStream(sf::InputStream &stream) {
    if (!this->image.loadFromStream(stream)) {
      throw std::runtime_error("load from stream failed");
    }
  }
  virtual void saveToFile(std::string const &filename) const {
    if (!this->image.saveToFile(filename)) {
      throw std::runtime_error("save to file failed");
    }
  }
  virtual void saveToMemory(
    std::vector<sf::Uint8> &output,
    std::string const &format
  ) const {
    if (this->image.saveToMemory(output, format)) {
      throw std::runtime_error("save to memory failed");
    }
  }
  virtual sf::Vector2u getImageSize() const {
    return this->image.getSize();
  }
  virtual sf::Vector2u getTextureSize() const {
    return this->texture.getSize();
  }
  virtual void createMaskFromColor(sf::Color const &color, sf::Uint8 alpha = 0) {
    this->image.createMaskFromColor(color, alpha);
  }
  virtual void copy(
    sf::Image const &source,
    u32 destX = 0,
    u32 destY = 0,
    sf::IntRect const &sourceRect = sf::IntRect(0, 0, 0, 0),
    bool applyAlpha = false
  ) {
    sf::Vector2u size = source.getSize();
    this->image.create(size.x, size.y, sf::Color::Black);
    this->image.copy(source, destX, destY, sourceRect, applyAlpha);
  }
  virtual sf::Image copyToImage() const {
    return std::move(this->texture.copyToImage());
  }
  virtual void setPixel(u32 x, u32 y, sf::Color const &color) {
    this->image.setPixel(x, y, color);
  }
  virtual sf::Color getPixel(u32 x, u32 y) const {
    return this->image.getPixel(x, y);
  }
  virtual sf::Uint8 const *getPixelsPtr() const {
    return this->image.getPixelsPtr();
  }
  virtual void flipHorizontally() {
    this->image.flipHorizontally();
  }
  virtual void flipVertically() {
    this->image.flipVertically();
  }
  virtual void update(sf::Uint8 const *pixels) {
    this->texture.update(pixels);
  }
  virtual void update(sf::Uint8 const *pixels, u32 width, u32 height, u32 x, u32 y) {
    this->texture.update(pixels, width, height, x, y);
  }
  virtual void update(sf::Texture const &texture) {
    this->texture.update(texture);
  }
  virtual void update(sf::Texture const &texture, u32 x, u32 y) {
    this->texture.update(texture, x, y);
  }
  virtual void update(sf::Image const &image) {
    this->texture.update(image);
  }
  virtual void update(sf::Image const &image, u32 x, u32 y) {
    this->texture.update(image, x, y);
  }
  virtual void update(sf::Window const &window) {
    this->texture.update(window);
  }
  virtual void update(sf::Window const &window, u32 x, u32 y) {
    this->texture.update(window, x, y);
  }
  virtual void setSmooth(bool smooth) {
    this->texture.setSmooth(smooth);
  }
  virtual bool isSmooth() const {
    return this->texture.isSmooth();
  }
  virtual void setSrgb(bool sRgb) {
    this->texture.setSrgb(sRgb);
  }
  virtual bool isSrgb() const {
    return this->texture.isSrgb();
  }
  virtual void setRepeated(bool repeated) {
    this->texture.setRepeated(repeated);
  }
  virtual bool isRepeated() const {
    return this->texture.isRepeated();
  }
  virtual void generateMipmap() {
    if (!this->texture.generateMipmap()) {
      throw std::runtime_error("generate mipmap failed");
    }
  }
  virtual sf::Texture &operator=(sf::Texture const &right) {
    return this->texture = right;
  }
  virtual void swap(sf::Texture &right) {
    this->texture.swap(right);
  }
  virtual u32 getNativeHandle() const {
    return this->texture.getNativeHandle();
  }

};

#endif

// AutoSprite(.hpp/.cpp)
#pragma once
#ifndef AUTO_SPRITE_HPP
#define AUTO_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

using usize = unsigned long;

class SpriteAtlas;

class AutoSprite {
  sf::Clock clock;
  sf::Sprite sprite;
  usize current_anime;
  usize current_motion;
  SpriteAtlas const *sprite_atlas;

public:
  AutoSprite();
  AutoSprite(SpriteAtlas const *const &sprite_atlas);
  AutoSprite(AutoSprite const &rhs);
  virtual AutoSprite &operator=(AutoSprite const &rhs);
  virtual ~AutoSprite();

  virtual operator sf::Sprite const &();

  virtual SpriteAtlas const *const &getSpriteAtlas() const;
  virtual void setSpriteAtlas(SpriteAtlas const *const &sprite_atlas);

  virtual usize const &getCurrentAnime() const;
  virtual void setCurrentAnime(usize const &anime_code);

  virtual usize const &getCurrentMotion() const;
  virtual void setCurrentMotion(usize const &motion_code);

  virtual sf::FloatRect getLocalBounds() const;
  virtual sf::FloatRect getGlobalBounds() const;

  virtual sf::Transform const &getTransform() const;
  virtual sf::Transform const &getInverseTransform() const;

  virtual sf::Color const &getColor() const;
  virtual void setColor(sf::Color const &color);

  virtual sf::Vector2f const &getPosition() const;
  virtual void setPosition(float x, float y);
  virtual void setPosition(sf::Vector2f const &position);

  virtual float getRotation() const;
  virtual void setRotation(float angle);

  virtual sf::Vector2f const &getScale() const;
  virtual void setScale(float factorX, float factorY);
  virtual void setScale(sf::Vector2f const &factors);

  virtual sf::Vector2f const &getOrigin() const;
  virtual void setOrigin(float x, float y);
  virtual void setOrigin(sf::Vector2f const &origin);

  virtual void move(float offsetX, float offsetY);
  virtual void move(sf::Vector2f const &offset);
  virtual void rotate(float angle);
  virtual void scale(float factorX, float factorY);
  virtual void scale(sf::Vector2f const &factor);

};

#endif

#include "AutoSprite.hpp"

#include <stdexcept>

#include "SpriteAtlas.hpp"

AutoSprite::AutoSprite() :
  current_anime(),
  current_motion(),
  sprite_atlas()
{}
AutoSprite::AutoSprite(SpriteAtlas const *const &sprite_atlas) :
  current_anime(),
  current_motion(),
  sprite_atlas(sprite_atlas)
{
  if (sprite_atlas != nullptr) {
    this->sprite.setTexture(*sprite_atlas);
  }
}
AutoSprite::AutoSprite(AutoSprite const &rhs) {
  *this = rhs;
}
AutoSprite &AutoSprite::operator=(AutoSprite const &rhs) {
  if (this == &rhs) { return *this; }
  this->clock = rhs.clock;
  this->sprite = rhs.sprite;
  this->current_anime = rhs.current_anime;
  this->current_motion = rhs.current_motion;
  this->sprite_atlas = rhs.sprite_atlas;
  return *this;
}
AutoSprite::~AutoSprite() {}

AutoSprite::operator sf::Sprite const &() {
  if (this->sprite_atlas != nullptr) {
    AnimeStore const &animes = this->sprite_atlas->getAnimes();
    if (this->current_anime >= animes.size()) {
      throw std::runtime_error("No exist current_anime in animes.");
    }
    Anime const &anime = animes[this->current_anime];
    if (this->current_motion >= anime.size()) {
      throw std::runtime_error("No exist current_motion in anime.");
    }
    Motion const &motion = anime[this->current_motion];
    if (motion.second.asMilliseconds() != 0
      && this->clock.getElapsedTime() >= motion.second)
    {
      this->clock.restart();
      this->current_motion += 1;
      this->current_motion %= anime.size();
      this->sprite.setTextureRect(motion.first);
    }
  }
  return this->sprite;
}

SpriteAtlas const *const &AutoSprite::getSpriteAtlas() const {
  return this->sprite_atlas;
}
void AutoSprite::setSpriteAtlas(SpriteAtlas const *const &sprite_atlas) {
  this->sprite_atlas = sprite_atlas;
  if (sprite_atlas != nullptr) {
    this->sprite.setTexture(*sprite_atlas);
  } else {
    this->sprite = sf::Sprite();
  }
}
usize const &AutoSprite::getCurrentAnime() const {
  return this->current_anime;
}
void AutoSprite::setCurrentAnime(usize const &anime_code) {
  if (this->sprite_atlas == nullptr) {
    throw std::runtime_error("Nothing set a SpriteAtlas.");
  }
  this->clock.restart();
  this->current_anime = anime_code;
  this->current_motion = 0;
  Motion const &motion = this->sprite_atlas->getMotion(
    this->current_anime, this->current_motion
  );
  this->sprite.setTextureRect(motion.first);
}
usize const &AutoSprite::getCurrentMotion() const {
  return this->current_motion;
}
void AutoSprite::setCurrentMotion(usize const &motion_code) {
  if (this->sprite_atlas == nullptr) {
    throw std::runtime_error("Nothing set a SpriteAtlas.");
  }
  this->clock.restart();
  this->current_motion = motion_code;
  Motion const &motion = this->sprite_atlas->getMotion(
    this->current_anime, this->current_motion
  );
  this->sprite.setTextureRect(motion.first);
}

sf::FloatRect AutoSprite::getLocalBounds() const {
  return this->sprite.getLocalBounds();
}
sf::FloatRect AutoSprite::getGlobalBounds() const {
  return this->sprite.getGlobalBounds();
}

sf::Transform const &AutoSprite::getTransform() const {
  return this->sprite.getTransform();
}
sf::Transform const &AutoSprite::getInverseTransform() const {
  return this->sprite.getInverseTransform();
}

sf::Color const &AutoSprite::getColor() const {
  return this->sprite.getColor();
}
void AutoSprite::setColor(sf::Color const &color) {
  this->sprite.setColor(color);
}

sf::Vector2f const &AutoSprite::getPosition() const {
  return this->sprite.getPosition();
}
void AutoSprite::setPosition(float x, float y) {
  this->sprite.setPosition(x, y);
}
void AutoSprite::setPosition(sf::Vector2f const &position) {
  this->sprite.setPosition(position);
}

float AutoSprite::getRotation() const {
  return this->sprite.getRotation();
}
void AutoSprite::setRotation(float angle) {
  this->sprite.setRotation(angle);
}

sf::Vector2f const &AutoSprite::getScale() const {
  return this->sprite.getScale();
}
void AutoSprite::setScale(float factorX, float factorY) {
  this->sprite.setScale(factorX, factorY);
}
void AutoSprite::setScale(sf::Vector2f const &factors) {
  this->sprite.setScale(factors);
}

sf::Vector2f const &AutoSprite::getOrigin() const {
  return this->sprite.getOrigin();
}
void AutoSprite::setOrigin(float x, float y) {
  this->sprite.setOrigin(x, y);
}
void AutoSprite::setOrigin(sf::Vector2f const &origin) {
  this->sprite.setOrigin(origin);
}

void AutoSprite::move(float offsetX, float offsetY) {
  this->sprite.move(offsetX, offsetY);
}
void AutoSprite::move(sf::Vector2f const &offset) {
  this->sprite.move(offset);
}
void AutoSprite::rotate(float angle) {
  this->sprite.rotate(angle);
}
void AutoSprite::scale(float factorX, float factorY) {
  this->sprite.scale(factorX, factorY);
}
void AutoSprite::scale(sf::Vector2f const &factor) {
  this->sprite.scale(factor);
}

// SpriteAtlas.hpp
#ifndef SPRITE_ATLAS_HPP_
#define SPRITE_ATLAS_HPP_

#include "lib/WrapTexture.hpp"
#include "lib/Animation.hpp"

using usize = unsigned long;

class SpriteAtlas : public WrapTexture, public Animation {

public:
  explicit SpriteAtlas() noexcept;
  explicit SpriteAtlas(std::string const &filename,
              sf::IntRect const &area = sf::IntRect()) noexcept;
  explicit SpriteAtlas(void const *data,
              usize const &size,
              sf::IntRect const &area = sf::IntRect()) noexcept;
  explicit SpriteAtlas(sf::InputStream &stream,
              sf::IntRect const &area = sf::IntRect()) noexcept;
  explicit SpriteAtlas(sf::Image const &image,
              sf::IntRect const &area = sf::IntRect()) noexcept;
  explicit SpriteAtlas(AnimeStore animes) noexcept;
  explicit SpriteAtlas(usize const &anime_count) noexcept;
  explicit SpriteAtlas(SpriteAtlas const &rhs) noexcept;
  virtual SpriteAtlas &operator=(SpriteAtlas const &rhs) noexcept;
  virtual ~SpriteAtlas() noexcept;

}; // SpriteAtlas

#endif
// SpriteAtlas.cpp
#include "lib/SpriteAtlas.hpp"

SpriteAtlas::SpriteAtlas() noexcept
    : WrapTexture(), Animation() {
}

SpriteAtlas::SpriteAtlas(std::string const &filename,
                         sf::IntRect const &area) noexcept
    : WrapTexture(filename, area), Animation() {
}

SpriteAtlas::SpriteAtlas(void const *data,
                         usize const &size,
                         sf::IntRect const &area) noexcept
    : WrapTexture(data, size, area), Animation() {
}

SpriteAtlas::SpriteAtlas(sf::InputStream &stream,
                         sf::IntRect const &area) noexcept
    : WrapTexture(stream, area), Animation() {
}

SpriteAtlas::SpriteAtlas(sf::Image const &image,
                         sf::IntRect const &area) noexcept
    : WrapTexture(image, area), Animation() {
}

SpriteAtlas::SpriteAtlas(AnimeStore animes) noexcept
    : WrapTexture(), Animation(std::move(animes)) {
}

SpriteAtlas::SpriteAtlas(usize const &anime_count) noexcept
    : WrapTexture(), Animation(anime_count) {
}

SpriteAtlas::SpriteAtlas(SpriteAtlas const &rhs) noexcept {
  *this = rhs;
}

SpriteAtlas &SpriteAtlas::operator=(SpriteAtlas const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  dynamic_cast<WrapTexture &>(*this) = dynamic_cast<WrapTexture const &>(rhs);
  dynamic_cast<Animation &>(*this) = dynamic_cast<Animation const &>(rhs);
  return *this;
}

SpriteAtlas::~SpriteAtlas() noexcept {}
