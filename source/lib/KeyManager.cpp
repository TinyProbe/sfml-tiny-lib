#include "lib/KeyManager.hpp"

#include <stdexcept>

// KeyMap
KeyManager::KeyMap::KeyMap()
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->is_linked_ = bool();
}

KeyManager::KeyMap::KeyMap(usize const &key_count)
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->callbacks_.resize(
      key_count, KeyCallbackElement(KeyManager::kKeyEventCount));
  ownership->can_repeat_.resize(key_count);
  ownership->is_linked_ = bool();
}

KeyManager::KeyMap::KeyMap(KeyManager::KeyMap const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

KeyManager::KeyMap &KeyManager::KeyMap::operator=(
    KeyManager::KeyMap const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) {
    if (ownership->is_linked_) { KeyManager::unlink(); }
    delete ownership;
  }
  ownership = rhs.ownership;
  const_cast<KeyManager::KeyMap &>(rhs).ownership = nullptr;
  if (ownership != nullptr) {
    if (ownership->is_linked_) { KeyManager::link(this); }
  }
  return *this;
}

KeyManager::KeyMap::~KeyMap() noexcept {
  if (ownership != nullptr) {
    if (ownership->is_linked_) { KeyManager::unlink(); }
    delete ownership;
  }
}

KeyManager::KeyMap KeyManager::KeyMap::clone() const {
  this->ownershipCheck();
  return KeyManager::KeyMap(new KeyManager::KeyMap::Inner(*ownership));
}

usize KeyManager::KeyMap::getKeyCount() const {
  this->ownershipCheck();
  return ownership->callbacks_.size();
}

void KeyManager::KeyMap::setKeyCount(usize const &key_count) {
  this->ownershipCheck();
  ownership->callbacks_.resize(
      key_count, KeyCallbackElement(KeyManager::kKeyEventCount));
  ownership->can_repeat_.resize(key_count);
  if (ownership->is_linked_) {
    KeyManager::key_state_.resize(key_count);
  }
}

KeyCallbackStore const &KeyManager::KeyMap::getKeyCallbacks() const {
  this->ownershipCheck();
  return ownership->callbacks_;
}

KeyCallback const &KeyManager::KeyMap::getKeyCallback(
    usize const &key_code,
    usize const &key_event_code) const {
  this->codeCheck(key_code, usize(-1), key_event_code);
  return ownership->callbacks_[key_code][key_event_code];
}

void KeyManager::KeyMap::setKeyCallback(usize const &key_code,
                                        usize const &key_event_code,
                                        KeyCallback const &callback,
                                        bool const &can_repeat) {
  this->codeCheck(key_code, usize(-1), key_event_code);
  ownership->callbacks_[key_code][key_event_code] = callback;
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat_[key_code] = can_repeat;
  }
}

std::pair<KeyCallback, bool> KeyManager::KeyMap::popKeyCallback(
    usize const &key_code,
    usize const &key_event_code) {
  this->codeCheck(key_code, usize(-1), key_event_code);
  std::pair<KeyCallback, bool> tmp({
    std::move(ownership->callbacks_[key_code][key_event_code]),
    bool(),
  });
  ownership->callbacks_[key_code][key_event_code] = KeyCallback();
  if (key_event_code == KeyManager::kPress) {
    tmp.second = ownership->can_repeat_[key_code];
    ownership->can_repeat_[key_code] = bool();
  }
  return std::pair<KeyCallback, bool>(std::move(tmp));
}

void KeyManager::KeyMap::copyKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  ownership->callbacks_[key_code_to][key_event_code] =
      ownership->callbacks_[key_code_from][key_event_code];
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat_[key_code_to] = ownership->can_repeat_[key_code_from];
  }
}

void KeyManager::KeyMap::moveKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  ownership->callbacks_[key_code_to][key_event_code] =
      std::move(ownership->callbacks_[key_code_from][key_event_code]);
  ownership->callbacks_[key_code_from][key_event_code] = KeyCallback();
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat_[key_code_to] = ownership->can_repeat_[key_code_from];
    ownership->can_repeat_[key_code_from] = bool();
  }
}

void KeyManager::KeyMap::swapKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  KeyCallback tmp =
      std::move(ownership->callbacks_[key_code_to][key_event_code]);
  ownership->callbacks_[key_code_to][key_event_code] =
      std::move(ownership->callbacks_[key_code_from][key_event_code]);
  ownership->callbacks_[key_code_from][key_event_code] = std::move(tmp);
  if (key_event_code == KeyManager::kPress) {
    bool tmp = ownership->can_repeat_[key_code_to];
    ownership->can_repeat_[key_code_to] = ownership->can_repeat_[key_code_from];
    ownership->can_repeat_[key_code_from] = tmp;
  }
}

bool KeyManager::KeyMap::canRepeat(usize const &key_code) const {
  this->codeCheck(key_code);
  return ownership->can_repeat_[key_code];
}

KeyManager::KeyMap::Inner::Inner() {
}

KeyManager::KeyMap::Inner::Inner(KeyManager::KeyMap::Inner const &rhs) {
  *this = rhs;
}

KeyManager::KeyMap::Inner &KeyManager::KeyMap::Inner::operator=(
    KeyManager::KeyMap::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->callbacks_.assign(rhs.callbacks_.begin(), rhs.callbacks_.end());
  this->can_repeat_.assign(rhs.can_repeat_.begin(), rhs.can_repeat_.end());
  this->is_linked_ = rhs.is_linked_;
  return *this;
}

void KeyManager::KeyMap::link() {
  this->ownershipCheck();
  ownership->is_linked_ = true;
}

void KeyManager::KeyMap::unlink() {
  this->ownershipCheck();
  ownership->is_linked_ = false;
}

KeyManager::KeyMap::KeyMap(KeyMap::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void KeyManager::KeyMap::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error(
        "No ownership rights whatsoever: KeyManager::KeyMap");
  }
}

void KeyManager::KeyMap::codeCheck(usize const &key_code_from,
                                   usize const &key_code_to,
                                   usize const &key_event_code) const {
  this->ownershipCheck();
  if (key_code_from >= ownership->callbacks_.size()) {
    throw std::runtime_error("No exist key_code_from.");
  } 
  if (key_code_to != usize(-1) &&
      key_code_to >= ownership->callbacks_.size()) {
    throw std::runtime_error("No exist key_code_to.");
  }
  if (key_event_code != usize(-1) &&
      key_event_code >= KeyManager::kKeyEventCount) {
    throw std::runtime_error("No exist key_event_code.");
  }
}

// KeyManaer
std::vector<bool> KeyManager::key_state_;
KeyManager::KeyMap const *KeyManager::key_map_ = nullptr;

void KeyManager::eventProcess(sf::Event const &event) {
  if (event.type == sf::Event::KeyPressed) {
    KeyManager::press(event.key);
  } else if (event.type == sf::Event::KeyReleased) {
    KeyManager::release(event.key);
  }
}

void KeyManager::framework() {
  if (KeyManager::key_map_ != nullptr) {
    KeyCallbackStore const &callbacks =
        KeyManager::key_map_->getKeyCallbacks();
    for (usize i = KeyManager::key_state_.size(); i--; ) {
      if (KeyManager::key_state_[i] && callbacks[i][KeyManager::kPressed]) {
        callbacks[i][KeyManager::kPressed]();
      }
    }
  }
}

usize KeyManager::getKeyCount() noexcept {
  return KeyManager::key_state_.size();
}

bool KeyManager::getKeyState(usize const &key_code) {
  KeyManager::key_map_->codeCheck(key_code);
  return KeyManager::key_state_[key_code];
}

KeyManager::KeyMap const *const &KeyManager::getKeyMap() noexcept {
  return KeyManager::key_map_;
}

void KeyManager::setKeyMap(KeyManager::KeyMap const *const &key_map) {
  if (KeyManager::key_map_ != nullptr) {
    const_cast<KeyManager::KeyMap *>(KeyManager::key_map_)->unlink();
  }
  KeyManager::link(key_map);
  if (KeyManager::key_map_ != nullptr) {
    KeyManager::key_state_.resize(key_map->getKeyCount());
    const_cast<KeyManager::KeyMap *>(KeyManager::key_map_)->link();
  }
}

void KeyManager::press(sf::Event::KeyEvent const &key_event) {
  if (key_event.code == usize(-1)) { return; }
  if (KeyManager::key_map_ != nullptr) {
    KeyManager::key_map_->codeCheck(key_event.code);
    if (!KeyManager::key_state_[key_event.code] ||
        KeyManager::key_map_->canRepeat(key_event.code)) {
      KeyCallback const &callback =
          KeyManager::key_map_->getKeyCallback(key_event.code,
                                               KeyManager::kPress);
      if (callback) { callback(); }
    }
  }
  KeyManager::key_state_[key_event.code] = true;
}

void KeyManager::release(sf::Event::KeyEvent const &key_event) {
  if (key_event.code == usize(-1)) { return; }
  if (KeyManager::key_map_ != nullptr) {
    KeyManager::key_map_->codeCheck(key_event.code);
    KeyCallback const &callback =
        KeyManager::key_map_->getKeyCallback(key_event.code,
                                             KeyManager::kRelease);
    if (callback) { callback(); }
  }
  KeyManager::key_state_[key_event.code] = false;
}

void KeyManager::link(KeyManager::KeyMap const *const &key_map) noexcept {
  KeyManager::key_map_ = key_map;
}

void KeyManager::unlink() noexcept {
  KeyManager::key_map_ = nullptr;
}
