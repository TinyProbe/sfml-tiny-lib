#include "AutoSprite.hpp"

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
