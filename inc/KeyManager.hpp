#pragma once
#ifndef KEY_MANAGER_HPP
#define KEY_MANAGER_HPP

#include <stdexcept>
#include <functional>
#include <vector>
#include <utility>

using usize = unsigned long;
using Callback = std::function<void()>;
using CallbackElement = std::vector<Callback>;
using CallbackStore = std::vector<CallbackElement>;

// A powerful convenient key manager.
class KeyManager {
public:
  enum Kind {
    Press,
    Release,
    Pressed,
    KindCount,
  };
  class KeyMap {
    CallbackStore callbacks;
    std::vector<bool> can_repeat;
    KeyManager const *key_manager;

  public:
    KeyMap() : key_manager() {}
    KeyMap(usize const &key_count) : key_manager() {
      this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
      this->can_repeat.resize(key_count);
    }
    KeyMap(KeyMap const &rhs) : key_manager() { *this = rhs; }
    virtual KeyMap &operator=(KeyMap const &rhs) {
      if (this == &rhs) { return *this; }
      this->callbacks.assign(rhs.callbacks.begin(), rhs.callbacks.end());
      this->can_repeat.assign(rhs.can_repeat.begin(), rhs.can_repeat.end());
      return *this;
    }
    virtual ~KeyMap() {
      if (this->key_manager != nullptr) {
        const_cast<KeyManager *>(this->key_manager)->unlink();
      }
    }

    virtual usize getKeyCount() const {
      return this->callbacks.size();
    }
    virtual void setKeyCount(usize const &key_count) {
      this->callbacks.resize(key_count, CallbackElement(KeyManager::KindCount));
      this->can_repeat.resize(key_count);
      if (this->key_manager != nullptr) {
        const_cast<KeyManager *>(this->key_manager)->key_state.resize(key_count);
      }
    }

    virtual CallbackStore const &getCallbacks() const {
      return this->callbacks;
    }
    virtual Callback const &getCallback(
      usize const &key_code,
      usize const &kind_code
    ) const {
      this->codeCheck(key_code, usize(-1), kind_code);
      return this->callbacks[key_code][kind_code];
    }
    virtual void setCallback(
      usize const &key_code,
      usize const &kind_code,
      Callback callback,
      bool const &can_repeat = false
    ) {
      this->codeCheck(key_code, usize(-1), kind_code);
      this->callbacks[key_code][kind_code] = std::move(callback);
      if (kind_code == Press) {
        this->can_repeat[key_code] = can_repeat;
      }
    }
    virtual std::pair<Callback, bool> popCallback(
      usize const &key_code,
      usize const &kind_code
    ) {
      this->codeCheck(key_code, usize(-1), kind_code);
      std::pair<Callback, bool> tmp({
        std::move(this->callbacks[key_code][kind_code]),
        bool()
      });
      this->callbacks[key_code][kind_code] = Callback();
      if (kind_code == KeyManager::Press) {
        tmp.second = this->can_repeat[key_code];
        this->can_repeat[key_code] = bool();
      }
      return std::move(tmp);
    }
    virtual void copyCallback(
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
    virtual void moveCallback(
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
    virtual void swapCallback(
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

    virtual bool canRepeat(usize const &key_code) const {
      this->codeCheck(key_code, usize(-1), usize(-1));
      return this->can_repeat[key_code];
    }

  protected:
    friend class KeyManager;
    virtual void link(KeyManager const *key_manager) { this->key_manager = key_manager; }
    virtual void unlink() { this->key_manager = nullptr; }

    virtual void codeCheck(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    ) const {
      if (key_code_from != usize(-1) && key_code_from >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      } 
      if (key_code_to != usize(-1) && key_code_to >= this->getKeyCount()) {
        throw std::runtime_error("No exist KeyCode.");
      }
      if (kind_code != usize(-1) && kind_code >= KeyManager::KindCount) {
        throw std::runtime_error("No exist KindCode.");
      }
    }

  };

private:
  std::vector<bool> key_state;
  KeyMap const *key_map;

public:
  KeyManager() : key_map() {}
  KeyManager(KeyMap const *const &key_map) : key_map(key_map) {
    this->key_state.resize(key_map->getKeyCount());
    const_cast<KeyMap *>(this->key_map)->link(this);
  }
  KeyManager(KeyManager const &rhs) { *this = rhs; }
  virtual KeyManager &operator=(KeyManager const &rhs) {
    if (this == &rhs) { return *this; }
    this->key_state.assign(rhs.key_state.begin(), rhs.key_state.end());
    this->key_map = rhs.key_map;
    return *this;
  }
  virtual ~KeyManager() {
    if (this->key_map != nullptr) {
      const_cast<KeyMap *>(this->key_map)->unlink();
    }
  }

  virtual void keyPress(usize const &key_code) {
    if (key_code == usize(-1)) { return; }
    this->codeCheck(key_code, usize(-1));
    if (this->key_map != nullptr) {
      if (!this->key_state[key_code] || this->key_map->canRepeat(key_code)) {
        Callback const &callback = this->key_map->getCallback(key_code, KeyManager::Press);
        if (callback) { callback(); }
      }
    }
    this->key_state[key_code] = true;
  }
  virtual void keyRelease(usize const &key_code) {
    if (key_code == usize(-1)) { return; }
    this->codeCheck(key_code, usize(-1));
    if (this->key_map != nullptr) {
      Callback const &callback = this->key_map->getCallback(key_code, KeyManager::Release);
      if (callback) { callback(); }
    }
    this->key_state[key_code] = false;
  }

  virtual usize getKeyCount() const {
    return this->key_state.size();
  }
  virtual void setKeyCount(usize const &key_count) = delete;
  // virtual void setKeyCount(usize const &key_count) {
  //   if (this->key_map != nullptr) {
  //     throw std::runtime_error("Already linked KeyMap.");
  //   }
  //   this->key_state.resize(key_count);
  // }

  virtual bool getKeyState(usize const &key_code) const {
    this->codeCheck(key_code, usize(-1));
    return this->key_state[key_code];
  }

  virtual KeyMap const *const &getKeyMap() const {
    return this->key_map;
  }
  virtual void setKeyMap(KeyMap const *const &key_map) {
    if (this->key_map != nullptr) { const_cast<KeyMap *>(this->key_map)->unlink(); }
    this->link(key_map);
    this->key_state.resize(key_map->getKeyCount());
    if (this->key_map != nullptr) { const_cast<KeyMap *>(this->key_map)->link(this); }
  }
  virtual void clearKeyMap() = delete;
  // virtual void clearKeyMap() {
  //   if (this->key_map != nullptr) { const_cast<KeyMap *>(this->key_map)->unlink(); }
  //   this->unlink();
  // }

  virtual void keyFramework() const {
    if (this->key_map != nullptr) {
      CallbackStore const &callbacks = this->key_map->getCallbacks();
      for (usize i = this->getKeyCount(); i--; ) {
        if (this->key_state[i] && callbacks[i][KeyManager::Pressed]) {
          callbacks[i][KeyManager::Pressed]();
        }
      }
    }
  }

private:
  friend KeyMap::~KeyMap();
  friend void KeyMap::setKeyCount(usize const &);
  virtual void link(KeyMap const *key_map) { this->key_map = key_map; }
  virtual void unlink() { this->key_map = nullptr; }

  virtual void codeCheck(
    usize const &key_code,
    usize const &kind_code
  ) const {
    if (key_code != usize(-1) && key_code >= this->getKeyCount()) {
      throw std::runtime_error("No exist KeyCode.");
    }
    if (kind_code != usize(-1) && kind_code >= KeyManager::KindCount) {
      throw std::runtime_error("No exist KindCode.");
    }
  }

};

// // Example code (pseudo)
// int main() {
//   // 
//   // create window ...
//   //
//
//   // create KeyManager & KeyMap instance
//   KeyManager::KeyMap keymap;
//   KeyManager keymng(&keymap);
//   keymap.setKeyCount((sf::Keyboard::KeyCount));
//   // keymng'll check the Up key every each frame, call it if pressed
//   keymap.setCallback(sf::Keyboard::Up, KeyManager::Pressed, [&]() {
//     monster.move(0, -5);
//   });
//   keymng.setKeyMap(&keymap);
//
//   // start event loop
//   while (true) {
//     // event process
//     while (!event.empty()) {
//       if (event.type == KeyPressed) {
//         keymng.keyPress(event.keycode);
//       } else if (event.type == KeyReleased) {
//         keymng.keyRelease(event.keycode);
//       }
//     }
//
//     keymng.keyFramework(); // call this before render
//
//     //
//     // do something ...
//     //
//   }
// }

#endif
