#include "Animation.hpp"

Animation::Animation() {}
Animation::Animation(usize const &anime_count) {
  this->animes.resize(anime_count);
}
Animation::Animation(Animation const &rhs) {
  *this = rhs;
}
Animation &Animation::operator=(Animation const &rhs) {
  if (this == &rhs) { return *this; }
  this->animes.assign(rhs.animes.begin(), rhs.animes.end());
  return *this;
}
Animation::~Animation() {}

usize Animation::getAnimeCount() const {
  return this->animes.size();
}
void Animation::setAnimeCount(usize const &anime_count) {
  this->animes.resize(anime_count);
}

usize Animation::getMotionCount(usize const &anime_code) const {
  this->codeCheck(anime_code);
  return this->animes[anime_code].size();
}
void Animation::setMotionCount(
  usize const &anime_code,
  usize const &motion_count
) {
  this->codeCheck(anime_code);
  this->animes[anime_code].resize(motion_count);
}

AnimeStore const &Animation::getAnimes() const {
  return this->animes;
}
void Animation::setAnimes(AnimeStore animes) {
  this->animes = std::move(animes);
}

Anime const &Animation::getAnime(usize const &anime_code) const {
  this->codeCheck(anime_code);
  return this->animes[anime_code];
}
void Animation::setAnime(usize const &anime_code, Anime anime) {
  this->codeCheck(anime_code);
  this->animes[anime_code] = std::move(anime);
}

Motion const &Animation::getMotion(
  usize const &anime_code,
  usize const &motion_code
) const {
  this->codeCheck(anime_code, motion_code);
  return this->animes[anime_code][motion_code];
}
void Animation::setMotion(
  usize const &anime_code,
  usize const &motion_code,
  Motion const &motion
) {
  this->codeCheck(anime_code, motion_code);
  this->animes[anime_code][motion_code] = motion;
}

void Animation::codeCheck(
  usize const &anime_code,
  usize const &motion_code
) const {
  if (anime_code >= this->animes.size()) {
    throw std::runtime_error("No exist anime_code.");
  } 
  if (motion_code != usize(-1)
    && motion_code >= this->animes[anime_code].size())
  {
    throw std::runtime_error("No exist motion_code.");
  }
}
