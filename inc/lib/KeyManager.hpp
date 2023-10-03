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
  enum {
    Press,
    Release,
    Pressed,
    KindCount,
  };

  class KeyMap {
    CallbackStore callbacks;
    std::vector<bool> can_repeat;
    bool is_linked;

  public:
    KeyMap();
    KeyMap(usize const &key_count);
    KeyMap(KeyMap const &rhs);
    virtual KeyMap &operator=(KeyMap const &rhs);
    virtual ~KeyMap();

    virtual operator KeyMap const *() const;

    virtual usize getKeyCount() const;
    virtual void setKeyCount(usize const &key_count);

    virtual CallbackStore const &getCallbacks() const;

    virtual Callback const &getCallback(
      usize const &key_code,
      usize const &kind_code
    ) const;
    virtual void setCallback(
      usize const &key_code,
      usize const &kind_code,
      Callback callback,
      bool const &can_repeat = false
    );
    virtual std::pair<Callback, bool> popCallback(
      usize const &key_code,
      usize const &kind_code
    );
    virtual void copyCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    );
    virtual void moveCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    );
    virtual void swapCallback(
      usize const &key_code_from,
      usize const &key_code_to,
      usize const &kind_code
    );

    virtual bool canRepeat(usize const &key_code) const;

  protected:
    friend class KeyManager;
    virtual void link();
    virtual void unlink();

    virtual void codeCheck(
      usize const &key_code_from,
      usize const &key_code_to = -1,
      usize const &kind_code = -1
    ) const;

  };

private:
  static std::vector<bool> key_state;
  static KeyMap const *key_map;

  KeyManager();
  KeyManager(KeyManager const &rhs);
  KeyManager &operator=(KeyManager const &rhs);
  ~KeyManager();

public:
  static void keyPress(usize const &key_code);
  static void keyRelease(usize const &key_code);

  static usize getKeyCount();

  static bool getKeyState(usize const &key_code);

  static KeyMap const *const &getKeyMap();
  static void setKeyMap(KeyMap const *const &key_map);

  static void keyFramework();

private:
  friend KeyMap::~KeyMap();
  friend void KeyMap::setKeyCount(usize const &);
  static void link(KeyMap const *key_map);
  static void unlink();

  static void codeCheck(
    usize const &key_code,
    usize const &kind_code = -1
  );

};

#endif
