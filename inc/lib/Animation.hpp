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
  Animation();
  Animation(usize const &anime_count);
  Animation(Animation const &rhs);
  virtual Animation &operator=(Animation const &rhs);
  virtual ~Animation();

  virtual usize getAnimeCount() const;
  virtual void setAnimeCount(usize const &anime_count);

  virtual usize getMotionCount(usize const &anime_code) const;
  virtual void setMotionCount(
    usize const &anime_code,
    usize const &motion_count
  );

  virtual AnimeStore const &getAnimes() const;
  virtual void setAnimes(AnimeStore animes);

  virtual Anime const &getAnime(usize const &anime_code) const;
  virtual void setAnime(usize const &anime_code, Anime anime);

  virtual Motion const &getMotion(
    usize const &anime_code,
    usize const &motion_code
  ) const;
  virtual void setMotion(
    usize const &anime_code,
    usize const &motion_code,
    Motion const &motion
  );

private:
  virtual void codeCheck(
    usize const &anime_code,
    usize const &motion_code = -1
  ) const;

};

#endif
