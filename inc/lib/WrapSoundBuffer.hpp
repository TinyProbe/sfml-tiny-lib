#ifndef WRAP_SOUND_BUFFER_HPP_
#define WRAP_SOUND_BUFFER_HPP_

#include <SFML/Audio/SoundBuffer.hpp>

using u32 = unsigned int;

class WrapSoundBuffer {
 public:
  explicit WrapSoundBuffer();
  explicit WrapSoundBuffer(std::string const &filename);
  explicit WrapSoundBuffer(void const *data, std::size_t sizeInBytes);
  explicit WrapSoundBuffer(sf::InputStream &stream);
  explicit WrapSoundBuffer(sf::Int16 const *samples,
                           sf::Uint64 sampleCount,
                           u32 channelCount,
                           u32 sampleRate);
  explicit WrapSoundBuffer(sf::SoundBuffer const &sound_buffer);
  explicit WrapSoundBuffer(WrapSoundBuffer const &rhs) noexcept;
  virtual WrapSoundBuffer &operator=(WrapSoundBuffer const &rhs) noexcept;
  virtual ~WrapSoundBuffer() noexcept;

  virtual WrapSoundBuffer clone() const;

  virtual sf::SoundBuffer &getSoundBuffer();
  virtual sf::SoundBuffer const &getSoundBuffer() const;

  virtual void loadFromFile(std::string const &filename);
  virtual void loadFromMemory(void const *data, std::size_t sizeInBytes);
  virtual void loadFromStream(sf::InputStream &stream);
  virtual void loadFromSamples(sf::Int16 const *samples,
                               sf::Uint64 sampleCount,
                               u32 channelCount,
                               u32 sampleRate);
  virtual void saveToFile(std::string const &filename) const;

  virtual sf::Int16 const *getSamples() const;
  virtual sf::Uint64 getSampleCount() const;
  virtual u32 getSampleRate() const;
  virtual u32 getChannelCount() const;
  virtual sf::Time getDuration() const;

 protected:
  struct Inner {
    sf::SoundBuffer sound_buffer;

    explicit Inner();
    explicit Inner(Inner const &rhs);
    virtual Inner &operator=(Inner const &rhs);
  } *ownership;

 private:
  explicit WrapSoundBuffer(WrapSoundBuffer::Inner *const &ownership) noexcept;
  virtual void ownershipCheck() const;
};

#endif
