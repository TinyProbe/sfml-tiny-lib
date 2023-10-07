#include "lib/KeyManager.hpp"

#include <stdexcept>

// KeyMap
KeyManager::KeyMap::KeyMap()
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->is_linked = bool();
}

KeyManager::KeyMap::KeyMap(usize const &key_count)
    : ownership(new KeyManager::KeyMap::Inner()) {
  ownership->callbacks.resize(key_count,
                              CallbackElement(KeyManager::kKindCount));
  ownership->can_repeat.resize(key_count);
  ownership->is_linked = bool();
}

KeyManager::KeyMap::KeyMap(KeyManager::KeyMap const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

KeyManager::KeyMap &KeyManager::KeyMap::operator=(KeyManager::KeyMap const &rhs
                                                  ) noexcept {
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
  KeyManager::KeyMap result;
  result.ownership->callbacks.assign(ownership->callbacks.begin(),
                                     ownership->callbacks.end());
  result.ownership->can_repeat.assign(ownership->can_repeat.begin(),
                                     ownership->can_repeat.end());
  result.ownership->is_linked = bool();
  return KeyManager::KeyMap(result);
}

usize KeyManager::KeyMap::getKeyCount() const {
  this->ownershipCheck();
  return ownership->callbacks.size();
}

void KeyManager::KeyMap::setKeyCount(usize const &key_count) {
  this->ownershipCheck();
  ownership->callbacks.resize(key_count,
                              CallbackElement(KeyManager::kKindCount));
  ownership->can_repeat.resize(key_count);
  if (ownership->is_linked) {
    KeyManager::key_state.resize(key_count);
  }
}

CallbackStore const &KeyManager::KeyMap::getCallbacks() const {
  this->ownershipCheck();
  return ownership->callbacks;
}

Callback const &KeyManager::KeyMap::getCallback(usize const &key_code,
                                                usize const &kind_code) const {
  this->codeCheck(key_code, usize(-1), kind_code);
  return ownership->callbacks[key_code][kind_code];
}

void KeyManager::KeyMap::setCallback(usize const &key_code,
                                     usize const &kind_code,
                                     Callback callback,
                                     bool const &can_repeat) {
  this->codeCheck(key_code, usize(-1), kind_code);
  ownership->callbacks[key_code][kind_code] = std::move(callback);
  if (kind_code == KeyManager::kPress) {
    ownership->can_repeat[key_code] = can_repeat;
  }
}

std::pair<Callback, bool> KeyManager::KeyMap::popCallback(usize const &key_code,
                                                          usize const &kind_code
                                                          ) {
  this->codeCheck(key_code, usize(-1), kind_code);
  std::pair<Callback, bool> tmp({
    std::move(ownership->callbacks[key_code][kind_code]),
    bool(),
  });
  ownership->callbacks[key_code][kind_code] = Callback();
  if (kind_code == KeyManager::kPress) {
    tmp.second = ownership->can_repeat[key_code];
    ownership->can_repeat[key_code] = bool();
  }
  return std::move(tmp);
}

void KeyManager::KeyMap::copyCallback(usize const &key_code_from,
                                      usize const &key_code_to,
                                      usize const &kind_code) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  ownership->callbacks[key_code_to][kind_code] =
      ownership->callbacks[key_code_from][kind_code];
  if (kind_code == KeyManager::kPress) {
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
  }
}

void KeyManager::KeyMap::moveCallback(usize const &key_code_from,
                                      usize const &key_code_to,
                                      usize const &kind_code) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  ownership->callbacks[key_code_to][kind_code] =
      std::move(ownership->callbacks[key_code_from][kind_code]);
  ownership->callbacks[key_code_from][kind_code] = Callback();
  if (kind_code == KeyManager::kPress) {
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
    ownership->can_repeat[key_code_from] = bool();
  }
}

void KeyManager::KeyMap::swapCallback(usize const &key_code_from,
                                      usize const &key_code_to,
                                      usize const &kind_code) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  Callback tmp = std::move(ownership->callbacks[key_code_to][kind_code]);
  ownership->callbacks[key_code_to][kind_code] =
      std::move(ownership->callbacks[key_code_from][kind_code]);
  ownership->callbacks[key_code_from][kind_code] = std::move(tmp);
  if (kind_code == KeyManager::kPress) {
    bool tmp = ownership->can_repeat[key_code_to];
    ownership->can_repeat[key_code_to] = ownership->can_repeat[key_code_from];
    ownership->can_repeat[key_code_from] = tmp;
  }
}

bool KeyManager::KeyMap::canRepeat(usize const &key_code) const {
  this->codeCheck(key_code);
  return ownership->can_repeat[key_code];
}

void KeyManager::KeyMap::link() {
  this->ownershipCheck();
  ownership->is_linked = true;
}

void KeyManager::KeyMap::unlink() {
  this->ownershipCheck();
  ownership->is_linked = false;
}

void KeyManager::KeyMap::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error(
        "No ownership rights whatsoever: KeyManager::KeyMap");
  }
}

void KeyManager::KeyMap::codeCheck(usize const &key_code_from,
                                   usize const &key_code_to,
                                   usize const &kind_code) const {
  this->ownershipCheck();
  if (key_code_from >= ownership->callbacks.size()) {
    throw std::runtime_error("No exist key_code_from.");
  } 
  if (key_code_to != usize(-1) && key_code_to >= ownership->callbacks.size()) {
    throw std::runtime_error("No exist key_code_to.");
  }
  if (kind_code != usize(-1) && kind_code >= KeyManager::kKindCount) {
    throw std::runtime_error("No exist kind_code.");
  }
}

// KeyManaer
std::vector<bool> KeyManager::key_state;
KeyManager::KeyMap const *KeyManager::key_map = nullptr;

void KeyManager::keyPress(usize const &key_code) {
  if (key_code == usize(-1)) { return; }
  KeyManager::codeCheck(key_code);
  if (KeyManager::key_map != nullptr) {
    if (!KeyManager::key_state[key_code] ||
        KeyManager::key_map->canRepeat(key_code)) {
      Callback const &callback =
          KeyManager::key_map->getCallback(key_code, KeyManager::kPress);
      if (callback) { callback(); }
    }
  }
  KeyManager::key_state[key_code] = true;
}

void KeyManager::keyRelease(usize const &key_code) {
  if (key_code == usize(-1)) { return; }
  KeyManager::codeCheck(key_code);
  if (KeyManager::key_map != nullptr) {
    Callback const &callback =
        KeyManager::key_map->getCallback(key_code, KeyManager::kRelease);
    if (callback) { callback(); }
  }
  KeyManager::key_state[key_code] = false;
}

void KeyManager::keyFramework() noexcept {
  if (KeyManager::key_map != nullptr) {
    CallbackStore const &callbacks = KeyManager::key_map->getCallbacks();
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
  KeyManager::codeCheck(key_code);
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

void KeyManager::link(KeyManager::KeyMap const *key_map) noexcept {
  KeyManager::key_map = key_map;
}

void KeyManager::unlink() noexcept {
  KeyManager::key_map = nullptr;
}

void KeyManager::codeCheck(usize const &key_code,
                           usize const &kind_code) {
  if (key_code >= KeyManager::key_state.size()) {
    throw std::runtime_error("No exist key_code.");
  }
  if (kind_code != usize(-1) && kind_code >= KeyManager::kKindCount) {
    throw std::runtime_error("No exist kind_code.");
  }
}
