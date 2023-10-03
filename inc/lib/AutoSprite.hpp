#pragma once
#ifndef AUTO_SPRITE_HPP
#define AUTO_SPRITE_HPP

#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "SpriteAtlas.hpp"

using usize = unsigned long;

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
