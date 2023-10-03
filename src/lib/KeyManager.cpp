#include "KeyManager.hpp"

// KeyMap
KeyManager::KeyMap::KeyMap() :
  is_linked()
{}
KeyManager::KeyMap::KeyMap(usize const &key_count) :
  is_linked()
{
  this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
  this->can_repeat.resize(key_count);
}
KeyManager::KeyMap::KeyMap(KeyManager::KeyMap const &rhs) :
  is_linked()
{
  *this = rhs;
}
KeyManager::KeyMap &KeyManager::KeyMap::operator=(
  KeyManager::KeyMap const &rhs
) {
  if (this == &rhs) { return *this; }
  this->callbacks.assign(rhs.callbacks.begin(), rhs.callbacks.end());
  this->can_repeat.assign(rhs.can_repeat.begin(), rhs.can_repeat.end());
  return *this;
}
KeyManager::KeyMap::~KeyMap() {
  if (is_linked) {
    KeyManager::unlink();
  }
}

KeyManager::KeyMap::operator KeyManager::KeyMap const *() const {
  return this;
}

usize KeyManager::KeyMap::getKeyCount() const {
  return this->callbacks.size();
}
void KeyManager::KeyMap::setKeyCount(usize const &key_count) {
  this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
  this->can_repeat.resize(key_count);
  if (is_linked) {
    KeyManager::key_state.resize(key_count);
  }
}

CallbackStore const &KeyManager::KeyMap::getCallbacks() const {
  return this->callbacks;
}

Callback const &KeyManager::KeyMap::getCallback(
  usize const &key_code,
  usize const &kind_code
) const {
  this->codeCheck(key_code, usize(-1), kind_code);
  return this->callbacks[key_code][kind_code];
}
void KeyManager::KeyMap::setCallback(
  usize const &key_code,
  usize const &kind_code,
  Callback callback,
  bool const &can_repeat
) {
  this->codeCheck(key_code, usize(-1), kind_code);
  this->callbacks[key_code][kind_code] = std::move(callback);
  if (kind_code == KeyManager::Press) {
    this->can_repeat[key_code] = can_repeat;
  }
}
std::pair<Callback, bool> KeyManager::KeyMap::popCallback(
  usize const &key_code,
  usize const &kind_code
) {
  this->codeCheck(key_code, usize(-1), kind_code);
  std::pair<Callback, bool> tmp({
    std::move(this->callbacks[key_code][kind_code]),
    bool(),
  });
  this->callbacks[key_code][kind_code] = Callback();
  if (kind_code == KeyManager::Press) {
    tmp.second = this->can_repeat[key_code];
    this->can_repeat[key_code] = bool();
  }
  return std::move(tmp);
}
void KeyManager::KeyMap::copyCallback(
  usize const &key_code_from,
  usize const &key_code_to,
  usize const &kind_code
) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  this->callbacks[key_code_to][kind_code]
    = this->callbacks[key_code_from][kind_code];
  if (kind_code == KeyManager::Press) {
    this->can_repeat[key_code_to] = this->can_repeat[key_code_from];
  }
}
void KeyManager::KeyMap::moveCallback(
  usize const &key_code_from,
  usize const &key_code_to,
  usize const &kind_code
) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  this->callbacks[key_code_to][kind_code]
    = std::move(this->callbacks[key_code_from][kind_code]);
  this->callbacks[key_code_from][kind_code] = Callback();
  if (kind_code == KeyManager::Press) {
    this->can_repeat[key_code_to] = this->can_repeat[key_code_from];
    this->can_repeat[key_code_from] = bool();
  }
}
void KeyManager::KeyMap::swapCallback(
  usize const &key_code_from,
  usize const &key_code_to,
  usize const &kind_code
) {
  this->codeCheck(key_code_from, key_code_to, kind_code);
  Callback tmp = std::move(this->callbacks[key_code_to][kind_code]);
  this->callbacks[key_code_to][kind_code]
    = std::move(this->callbacks[key_code_from][kind_code]);
  this->callbacks[key_code_from][kind_code]
    = std::move(tmp);
  if (kind_code == KeyManager::Press) {
    bool tmp = this->can_repeat[key_code_to];
    this->can_repeat[key_code_to] = this->can_repeat[key_code_from];
    this->can_repeat[key_code_from] = tmp;
  }
}

bool KeyManager::KeyMap::canRepeat(usize const &key_code) const {
  this->codeCheck(key_code);
  return this->can_repeat[key_code];
}

void KeyManager::KeyMap::link() {
  this->is_linked = true;
}
void KeyManager::KeyMap::unlink() {
  this->is_linked = false;
}

void KeyManager::KeyMap::codeCheck(
  usize const &key_code_from,
  usize const &key_code_to,
  usize const &kind_code
) const {
  if (key_code_from >= this->callbacks.size()) {
    throw std::runtime_error("No exist key_code_from.");
  } 
  if (key_code_to != usize(-1) && key_code_to >= this->callbacks.size()) {
    throw std::runtime_error("No exist key_code_to.");
  }
  if (kind_code != usize(-1) && kind_code >= KeyManager::KindCount) {
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
    if (!KeyManager::key_state[key_code]
      || KeyManager::key_map->canRepeat(key_code))
    {
      Callback const &callback = KeyManager::key_map->getCallback(
        key_code, KeyManager::Press
      );
      if (callback) { callback(); }
    }
  }
  KeyManager::key_state[key_code] = true;
}
void KeyManager::keyRelease(usize const &key_code) {
  if (key_code == usize(-1)) { return; }
  KeyManager::codeCheck(key_code);
  if (KeyManager::key_map != nullptr) {
    Callback const &callback = KeyManager::key_map->getCallback(
      key_code, KeyManager::Release
    );
    if (callback) { callback(); }
  }
  KeyManager::key_state[key_code] = false;
}

usize KeyManager::getKeyCount() {
  return KeyManager::key_state.size();
}

bool KeyManager::getKeyState(usize const &key_code) {
  KeyManager::codeCheck(key_code);
  return KeyManager::key_state[key_code];
}

KeyManager::KeyMap const *const &KeyManager::getKeyMap() {
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

void KeyManager::keyFramework() {
  if (KeyManager::key_map != nullptr) {
    CallbackStore const &callbacks = KeyManager::key_map->getCallbacks();
    for (usize i = KeyManager::key_state.size(); i--; ) {
      if (KeyManager::key_state[i] && callbacks[i][KeyManager::Pressed]) {
        callbacks[i][KeyManager::Pressed]();
      }
    }
  }
}

void KeyManager::link(KeyManager::KeyMap const *key_map) {
  KeyManager::key_map = key_map;
}
void KeyManager::unlink() {
  KeyManager::key_map = nullptr;
}

void KeyManager::codeCheck(
  usize const &key_code,
  usize const &kind_code
) {
  if (key_code >= KeyManager::key_state.size()) {
    throw std::runtime_error("No exist key_code.");
  }
  if (kind_code != usize(-1) && kind_code >= KeyManager::KindCount) {
    throw std::runtime_error("No exist kind_code.");
  }
}
