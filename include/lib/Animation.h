#ifndef SFML_LIB_ANIMATION_H_
#define SFML_LIB_ANIMATION_H_

#include <vector>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

using usize = unsigned long;
using Motion = std::pair<sf::IntRect, sf::Time>;
using Anime = std::vector<Motion>;
using AnimeStore = std::vector<Anime>;

class Animation {
 public:
  explicit Animation();
  explicit Animation(AnimeStore const &animes);
  explicit Animation(usize const &anime_count);
  explicit Animation(Animation const &rhs);
  virtual Animation &operator=(Animation const &rhs) noexcept;
  virtual ~Animation() noexcept;

  virtual Animation clone() const;

  virtual usize getAnimeCount() const;
  virtual void setAnimeCount(usize const &anime_count);

  virtual usize getMotionCount(usize const &anime_code) const;
  virtual void setMotionCount(usize const &anime_code,
                              usize const &motion_count);

  virtual AnimeStore const &getAnimes() const;
  virtual void setAnimes(AnimeStore const &animes);

  virtual Anime const &getAnime(usize const &anime_code) const;
  virtual void setAnime(usize const &anime_code, Anime const &anime);

  virtual Motion const &getMotion(usize const &anime_code,
                                  usize const &motion_code) const;
  virtual void setMotion(usize const &anime_code,
                         usize const &motion_code,
                         Motion const &motion);

 protected:
  struct Inner {
    AnimeStore animes_;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit Animation(Animation::Inner *const &ownership) noexcept;
  virtual void ownershipCheck() const;
  virtual void codeCheck(usize const &anime_code,
                         usize const &motion_code = -1) const;

}; // Animation

#endif // SFML_ANIMATION_H_
