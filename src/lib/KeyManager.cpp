#include "lib/KeyManager.hpp"

#include <stdexcept>

// KeyMap
KeyManager::KeyMap::KeyMap()
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->is_linked = bool();
}

KeyManager::KeyMap::KeyMap(usize const &key_count)
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->callbacks.resize(
      key_count, KeyCallbackElement(KeyManager::kKeyEventCount));
  ownership->can_repeat.resize(key_count);
  ownership->is_linked = bool();
}

KeyManager::KeyMap::KeyMap(KeyManager::KeyMap const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

KeyManager::KeyMap &KeyManager::KeyMap::operator=(
    KeyManager::KeyMap const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) {
    if (ownership->is_linked) { KeyManager::unlink(); }
    delete ownership;
  }
  ownership = rhs.ownership;
  const_cast<KeyManager::KeyMap &>(rhs).ownership = nullptr;
  if (ownership != nullptr) {
    if (ownership->is_linked) { KeyManager::link(this); }
  }
  return *this;
}

KeyManager::KeyMap::~KeyMap() noexcept {
  if (ownership != nullptr) {
    if (ownership->is_linked) { KeyManager::unlink(); }
    delete ownership;
  }
}

KeyManager::KeyMap KeyManager::KeyMap::clone() const {
  this->ownershipCheck();
  return KeyManager::KeyMap(new KeyManager::KeyMap::Inner(*ownership));
}

usize KeyManager::KeyMap::getKeyCount() const {
  this->ownershipCheck();
  return ownership->callbacks.size();
}

void KeyManager::KeyMap::setKeyCount(usize const &key_count) {
  this->ownershipCheck();
  ownership->callbacks.resize(
      key_count, KeyCallbackElement(KeyManager::kKeyEventCount));
  ownership->can_repeat.resize(key_count);
  if (ownership->is_linked) {
    KeyManager::key_state.resize(key_count);
  }
}

KeyCallbackStore const &KeyManager::KeyMap::getKeyCallbacks() const {
  this->ownershipCheck();
  return ownership->callbacks;
}

KeyCallback const &KeyManager::KeyMap::getKeyCallback(
    usize const &key_code,
    usize const &key_event_code) const {
  this->codeCheck(key_code, usize(-1), key_event_code);
  return ownership->callbacks[key_code][key_event_code];
}

void KeyManager::KeyMap::setKeyCallback(usize const &key_code,
                                        usize const &key_event_code,
                                        KeyCallback const &callback,
                                        bool const &can_repeat) {
  this->codeCheck(key_code, usize(-1), key_event_code);
  ownership->callbacks[key_code][key_event_code] = callback;
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat[key_code] = can_repeat;
  }
}

std::pair<KeyCallback, bool> KeyManager::KeyMap::popKeyCallback(
    usize const &key_code,
    usize const &key_event_code) {
  this->codeCheck(key_code, usize(-1), key_event_code);
  std::pair<KeyCallback, bool> tmp({
    std::move(ownership->callbacks[key_code][key_event_code]),
    bool(),
  });
  ownership->callbacks[key_code][key_event_code] = KeyCallback();
  if (key_event_code == KeyManager::kPress) {
    tmp.second = ownership->can_repeat[key_code];
    ownership->can_repeat[key_code] = bool();
  }
  return std::pair<KeyCallback, bool>(std::move(tmp));
}

void KeyManager::KeyMap::copyKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  ownership->callbacks[key_code_to][key_event_code] =
      ownership->callbacks[key_code_from][key_event_code];
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
  }
}

void KeyManager::KeyMap::moveKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  ownership->callbacks[key_code_to][key_event_code] =
      std::move(ownership->callbacks[key_code_from][key_event_code]);
  ownership->callbacks[key_code_from][key_event_code] = KeyCallback();
  if (key_event_code == KeyManager::kPress) {
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
    ownership->can_repeat[key_code_from] = bool();
  }
}

void KeyManager::KeyMap::swapKeyCallback(usize const &key_code_from,
                                         usize const &key_code_to,
                                         usize const &key_event_code) {
  this->codeCheck(key_code_from, key_code_to, key_event_code);
  KeyCallback tmp =
      std::move(ownership->callbacks[key_code_to][key_event_code]);
  ownership->callbacks[key_code_to][key_event_code] =
      std::move(ownership->callbacks[key_code_from][key_event_code]);
  ownership->callbacks[key_code_from][key_event_code] = std::move(tmp);
  if (key_event_code == KeyManager::kPress) {
    bool tmp = ownership->can_repeat[key_code_to];
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
    ownership->can_repeat[key_code_from] = tmp;
  }
}

bool KeyManager::KeyMap::canRepeat(usize const &key_code) const {
  this->codeCheck(key_code);
  return ownership->can_repeat[key_code];
}

KeyManager::KeyMap::Inner::Inner() {
}

KeyManager::KeyMap::Inner::Inner(KeyManager::KeyMap::Inner const &rhs) {
  *this = rhs;
}

KeyManager::KeyMap::Inner &KeyManager::KeyMap::Inner::operator=(
    KeyManager::KeyMap::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->callbacks.assign(rhs.callbacks.begin(), rhs.callbacks.end());
  this->can_repeat.assign(rhs.can_repeat.begin(), rhs.can_repeat.end());
  this->is_linked = rhs.is_linked;
  return *this;
}

void KeyManager::KeyMap::link() {
  this->ownershipCheck();
  ownership->is_linked = true;
}

void KeyManager::KeyMap::unlink() {
  this->ownershipCheck();
  ownership->is_linked = false;
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
  if (key_code_from >= ownership->callbacks.size()) {
    throw std::runtime_error("No exist key_code_from.");
  } 
  if (key_code_to != usize(-1) &&
      key_code_to >= ownership->callbacks.size()) {
    throw std::runtime_error("No exist key_code_to.");
  }
  if (key_event_code != usize(-1) &&
      key_event_code >= KeyManager::kKeyEventCount) {
    throw std::runtime_error("No exist key_event_code.");
  }
}

// KeyManaer
std::vector<bool> KeyManager::key_state;
KeyManager::KeyMap const *KeyManager::key_map = nullptr;

void KeyManager::event(sf::Event const &event) {
  if (event.type == sf::Event::KeyPressed) {
    KeyManager::press(event.key);
  } else if (event.type == sf::Event::KeyReleased) {
    KeyManager::release(event.key);
  }
}

void KeyManager::framework() {
  if (KeyManager::key_map != nullptr) {
    KeyCallbackStore const &callbacks =
        KeyManager::key_map->getKeyCallbacks();
    for (usize i = KeyManager::key_state.size(); i--; ) {
      if (KeyManager::key_state[i] && callbacks[i][KeyManager::kPressed]) {
        callbacks[i][KeyManager::kPressed]();
      }
    }
  }
}

usize KeyManager::getKeyCount() noexcept {
  return KeyManager::key_state.size();
}

bool KeyManager::getKeyState(usize const &key_code) {
  KeyManager::key_map->codeCheck(key_code);
  return KeyManager::key_state[key_code];
}

KeyManager::KeyMap const *const &KeyManager::getKeyMap() noexcept {
  return KeyManager::key_map;
}

void KeyManager::setKeyMap(KeyManager::KeyMap const *const &key_map) {
  if (KeyManager::key_map != nullptr) {
    const_cast<KeyManager::KeyMap *>(KeyManager::key_map)->unlink();
  }
  KeyManager::link(key_map);
  if (KeyManager::key_map != nullptr) {
    KeyManager::key_state.resize(key_map->getKeyCount());
    const_cast<KeyManager::KeyMap *>(KeyManager::key_map)->link();
  }
}

void KeyManager::press(sf::Event::KeyEvent const &key_event) {
  if (key_event.code == usize(-1)) { return; }
  if (KeyManager::key_map != nullptr) {
    KeyManager::key_map->codeCheck(key_event.code);
    if (!KeyManager::key_state[key_event.code] ||
        KeyManager::key_map->canRepeat(key_event.code)) {
      KeyCallback const &callback =
          KeyManager::key_map->getKeyCallback(key_event.code,
                                              KeyManager::kPress);
      if (callback) { callback(); }
    }
  }
  KeyManager::key_state[key_event.code] = true;
}

void KeyManager::release(sf::Event::KeyEvent const &key_event) {
  if (key_event.code == usize(-1)) { return; }
  if (KeyManager::key_map != nullptr) {
    KeyManager::key_map->codeCheck(key_event.code);
    KeyCallback const &callback =
        KeyManager::key_map->getKeyCallback(key_event.code,
                                            KeyManager::kRelease);
    if (callback) { callback(); }
  }
  KeyManager::key_state[key_event.code] = false;
}

void KeyManager::link(KeyManager::KeyMap const *const &key_map) noexcept {
  KeyManager::key_map = key_map;
}

void KeyManager::unlink() noexcept {
  KeyManager::key_map = nullptr;
}
