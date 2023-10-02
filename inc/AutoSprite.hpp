#pragma once
#ifndef AUTO_SPRITE_HPP
#define AUTO_SPRITE_HPP

#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "SpriteTexture.hpp"

using usize = unsigned long;

class AutoSprite {
  sf::Clock clock;
  sf::Sprite sprite;
  usize current_anime;
  usize current_motion;
  SpriteTexture const *sprite_texture;

public:
  AutoSprite() :
    current_anime(),
    current_motion(),
    sprite_texture()
  {}
  AutoSprite(SpriteTexture const *const &sprite_texture) :
    current_anime(),
    current_motion(),
    sprite_texture(sprite_texture)
  {
    if (sprite_texture != nullptr) {
      this->sprite.setTexture(*sprite_texture);
    }
  }
  AutoSprite(AutoSprite const &rhs) { *this = rhs; }
  virtual AutoSprite &operator=(AutoSprite const &rhs) {
    if (this == &rhs) { return *this; }
    this->clock = rhs.clock;
    this->sprite = rhs.sprite;
    this->current_anime = rhs.current_anime;
    this->current_motion = rhs.current_motion;
    this->sprite_texture = rhs.sprite_texture;
    return *this;
  }
  virtual ~AutoSprite() {}

  virtual operator sf::Sprite const &() {
    if (this->sprite_texture != nullptr) {
      AnimeStore const &animes = this->sprite_texture->getAnimes();
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

  virtual SpriteTexture const *const &getSpriteTexture() const {
    return this->sprite_texture;
  }
  virtual void setSpriteTexture(SpriteTexture const *const &sprite_texture) {
    this->sprite_texture = sprite_texture;
    if (sprite_texture != nullptr) {
      this->sprite.setTexture(*sprite_texture);
    } else {
      this->sprite = sf::Sprite();
    }
  }
  virtual usize const &getCurrentAnime() const {
    return this->current_anime;
  }
  virtual void setCurrentAnime(usize const &anime_code) {
    if (this->sprite_texture == nullptr) {
      throw std::runtime_error("Nothing set a SpriteTexture.");
    }
    this->clock.restart();
    this->current_anime = anime_code;
    this->current_motion = 0;
    Motion const &motion = this->sprite_texture->getMotion(
      this->current_anime,
      this->current_motion
    );
    this->sprite.setTextureRect(motion.first);
  }
  virtual usize const &getCurrentMotion() const {
    return this->current_motion;
  }
  virtual void setCurrentMotion(usize const &motion_code) {
    if (this->sprite_texture == nullptr) {
      throw std::runtime_error("Nothing set a SpriteTexture.");
    }
    this->clock.restart();
    this->current_motion = motion_code;
    Motion const &motion = this->sprite_texture->getMotion(
      this->current_anime,
      this->current_motion
    );
    this->sprite.setTextureRect(motion.first);
  }

  virtual void setColor(sf::Color const &color) {
    this->sprite.setColor(color);
  }
  virtual sf::Color const &getColor() const {
    return this->sprite.getColor();
  }
  virtual sf::FloatRect getLocalBounds() const {
    return this->sprite.getLocalBounds();
  }
  virtual sf::FloatRect getGlobalBounds() const {
    return this->sprite.getGlobalBounds();
  }
  virtual void setPosition(float x, float y) {
    this->sprite.setPosition(x, y);
  }
  virtual void setPosition(sf::Vector2f const &position) {
    this->sprite.setPosition(position);
  }
  virtual void setRotation(float angle) {
    this->sprite.setRotation(angle);
  }
  virtual void setScale(float factorX, float factorY) {
    this->sprite.setScale(factorX, factorY);
  }
  virtual void setScale(sf::Vector2f const &factors) {
    this->sprite.setScale(factors);
  }
  virtual void setOrigin(float x, float y) {
    this->sprite.setOrigin(x, y);
  }
  virtual void setOrigin(sf::Vector2f const &origin) {
    this->sprite.setOrigin(origin);
  }
  virtual sf::Vector2f const &getPosition() const {
    return this->sprite.getPosition();
  }
  virtual float getRotation() const {
    return this->sprite.getRotation();
  }
  virtual sf::Vector2f const &getScale() const {
    return this->sprite.getScale();
  }
  virtual sf::Vector2f const &getOrigin() const {
    return this->sprite.getOrigin();
  }
  virtual void move(float offsetX, float offsetY) {
    this->sprite.move(offsetX, offsetY);
  }
  virtual void move(sf::Vector2f const &offset) {
    this->sprite.move(offset);
  }
  virtual void rotate(float angle) {
    this->sprite.rotate(angle);
  }
  virtual void scale(float factorX, float factorY) {
    this->sprite.scale(factorX, factorY);
  }
  virtual void scale(sf::Vector2f const &factor) {
    this->sprite.scale(factor);
  }
  virtual sf::Transform const &getTransform() const {
    return this->sprite.getTransform();
  }
  virtual sf::Transform const &getInverseTransform() const {
    return this->sprite.getInverseTransform();
  }

};

#endif
