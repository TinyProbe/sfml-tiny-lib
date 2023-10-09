#include "lib/WrapSoundBuffer.hpp"

#include <stdexcept>

WrapSoundBuffer::WrapSoundBuffer()
    : ownership(new WrapSoundBuffer::Inner()) {
}

WrapSoundBuffer::WrapSoundBuffer(std::string const &filename)
    : ownership(new WrapSoundBuffer::Inner()) {
  this->loadFromFile(filename);
}

WrapSoundBuffer::WrapSoundBuffer(void const *data, std::size_t sizeInBytes)
    : ownership(new WrapSoundBuffer::Inner()) {
  this->loadFromMemory(data, sizeInBytes);
}

WrapSoundBuffer::WrapSoundBuffer(sf::InputStream &stream)
    : ownership(new WrapSoundBuffer::Inner()) {
  this->loadFromStream(stream);
}

WrapSoundBuffer::WrapSoundBuffer(sf::Int16 const *samples,
                                 sf::Uint64 sampleCount,
                                 u32 channelCount,
                                 u32 sampleRate)
    : ownership(new WrapSoundBuffer::Inner()) {
  this->loadFromSamples(samples, sampleCount, channelCount, sampleRate);
}

WrapSoundBuffer::WrapSoundBuffer(sf::SoundBuffer const &sound_buffer)
    : ownership(new WrapSoundBuffer::Inner()) {
  ownership->sound_buffer = sound_buffer;
}

WrapSoundBuffer::WrapSoundBuffer(WrapSoundBuffer const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

WrapSoundBuffer &WrapSoundBuffer::operator=(WrapSoundBuffer const &rhs
                                            ) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) { delete ownership; }
  ownership = rhs.ownership;
  const_cast<WrapSoundBuffer &>(rhs).ownership = nullptr;
  return *this;
}

WrapSoundBuffer::~WrapSoundBuffer() noexcept {
  if (ownership != nullptr) { delete ownership; }
}

WrapSoundBuffer WrapSoundBuffer::clone() const {
  this->ownershipCheck();
  return WrapSoundBuffer(new WrapSoundBuffer::Inner(*ownership));
}

sf::SoundBuffer &WrapSoundBuffer::getSoundBuffer() {
  this->ownershipCheck();
  return ownership->sound_buffer;
}

sf::SoundBuffer const &WrapSoundBuffer::getSoundBuffer() const {
  this->ownershipCheck();
  return ownership->sound_buffer;
}

void WrapSoundBuffer::loadFromFile(std::string const &filename) {
  this->ownershipCheck();
  if (!ownership->sound_buffer.loadFromFile(filename)) {
    throw std::runtime_error(std::string("load from file failed: ") + filename);
  }
}

void WrapSoundBuffer::loadFromMemory(void const *data,
                                     std::size_t sizeInBytes) {
  this->ownershipCheck();
  if (!ownership->sound_buffer.loadFromMemory(data, sizeInBytes)) {
    throw std::runtime_error("load from memory failed");
  }
}

void WrapSoundBuffer::loadFromStream(sf::InputStream &stream) {
  this->ownershipCheck();
  if (!ownership->sound_buffer.loadFromStream(stream)) {
    throw std::runtime_error("load from stream failed");
  }
}

void WrapSoundBuffer::loadFromSamples(sf::Int16 const *samples,
                                      sf::Uint64 sampleCount,
                                      u32 channelCount,
                                      u32 sampleRate) {
  this->ownershipCheck();
  if (!ownership->sound_buffer.loadFromSamples(
        samples, sampleCount, channelCount, sampleRate)) {
    throw std::runtime_error("load from samples failed");
  }
}

void WrapSoundBuffer::saveToFile(std::string const &filename) const {
  this->ownershipCheck();
  if (!ownership->sound_buffer.saveToFile(filename)) {
    throw std::runtime_error("save to file failed");
  }
}

sf::Int16 const *WrapSoundBuffer::getSamples() const {
  this->ownershipCheck();
  return ownership->sound_buffer.getSamples();
}

sf::Uint64 WrapSoundBuffer::getSampleCount() const {
  this->ownershipCheck();
  return ownership->sound_buffer.getSampleCount();
}

u32 WrapSoundBuffer::getSampleRate() const {
  this->ownershipCheck();
  return ownership->sound_buffer.getSampleRate();
}

u32 WrapSoundBuffer::getChannelCount() const {
  this->ownershipCheck();
  return ownership->sound_buffer.getChannelCount();
}

sf::Time WrapSoundBuffer::getDuration() const {
  this->ownershipCheck();
  return ownership->sound_buffer.getDuration();
}

WrapSoundBuffer::Inner::Inner() {
}

WrapSoundBuffer::Inner::Inner(WrapSoundBuffer::Inner const &rhs) {
  *this = rhs;
}

WrapSoundBuffer::Inner &WrapSoundBuffer::Inner::operator=(
    WrapSoundBuffer::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->sound_buffer = rhs.sound_buffer;
  return *this;
}

WrapSoundBuffer::WrapSoundBuffer(
    WrapSoundBuffer::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void WrapSoundBuffer::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error("No ownership rights whatsoever: WrapSoundBuffer");
  }
}
