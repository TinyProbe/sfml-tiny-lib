#pragma once
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

using usize = unsigned long;
using Motion = std::pair<sf::IntRect, sf::Time>;
using Anime = std::vector<Motion>;
using AnimeStore = std::vector<Anime>;

class Animation {
  AnimeStore animes;

public:
  Animation() {}
  Animation(usize const &anime_count) { this->animes.resize(anime_count); }
  Animation(Animation const &rhs) { *this = rhs; }
  virtual Animation &operator=(Animation const &rhs) {
    if (this == &rhs) { return *this; }
    this->animes.assign(rhs.animes.begin(), rhs.animes.end());
    return *this;
  }
  virtual ~Animation() {}

  virtual usize getAnimeCount() const {
    return this->animes.size();
  }
  virtual void setAnimeCount(usize const &anime_count) {
    this->animes.resize(anime_count);
  }
  virtual usize getMotionCount(usize const &anime_code) const {
    this->codeCheck(anime_code, usize(-1));
    return this->animes[anime_code].size();
  }
  virtual void setMotionCount(usize const &anime_code, usize const &motion_count) {
    this->codeCheck(anime_code, usize(-1));
    this->animes[anime_code].resize(motion_count);
  }
  virtual AnimeStore const &getAnimeStore() const {
    return this->animes;
  }
  virtual Anime const &getAnime(usize const &anime_code) const {
    this->codeCheck(anime_code, usize(-1));
    return this->animes[anime_code];
  }
  virtual void setAnime(usize const &anime_code, Anime anime) {
    this->codeCheck(anime_code, usize(-1));
    this->animes[anime_code] = std::move(anime);
  }
  virtual Motion const &getMotion(
    usize const &anime_code,
    usize const &motion_code
  ) const {
    this->codeCheck(anime_code, motion_code);
    return this->animes[anime_code][motion_code];
  }
  virtual void setMotion(
    usize const &anime_code,
    usize const &motion_code,
    Motion motion
  ) {
    this->codeCheck(anime_code, motion_code);
    this->animes[anime_code][motion_code] = std::move(motion);
  }

private:
  virtual void codeCheck(
    usize const &anime_code,
    usize const &motion_code
  ) const {
    if (anime_code != usize(-1) && anime_code >= this->getAnimeCount()) {
      throw std::runtime_error("No exist AnimeCode.");
    } 
    if (motion_code != usize(-1) && motion_code >= this->getMotionCount(anime_code)) {
      throw std::runtime_error("No exist MotionCode.");
    }
  }

};

#endif
