#ifndef KEY_MANAGER_HPP_
#define KEY_MANAGER_HPP_

#include <functional>
#include <utility>
#include <vector>

using usize = unsigned long;
using Callback = std::function<void()>;
using CallbackElement = std::vector<Callback>;
using CallbackStore = std::vector<CallbackElement>;

// A powerful convenient key manager.
class KeyManager {
 public:
  enum {
    kPress = 0,
    kRelease,
    kPressed,
    kKindCount,
  };
  class KeyMap {
   public:
    explicit KeyMap();
    explicit KeyMap(usize const &key_count);
    explicit KeyMap(KeyMap const &rhs) noexcept;
    virtual KeyMap &operator=(KeyMap const &rhs) noexcept;
    virtual ~KeyMap() noexcept;

    virtual KeyManager::KeyMap clone() const;

    virtual usize getKeyCount() const;
    virtual void setKeyCount(usize const &key_count);

    virtual CallbackStore const &getCallbacks() const;

    virtual Callback const &getCallback(usize const &key_code,
                                        usize const &kind_code) const;
    virtual void setCallback(usize const &key_code,
                             usize const &kind_code,
                             Callback callback,
                             bool const &can_repeat = false);
    virtual void copyCallback(usize const &key_code_from,
                              usize const &key_code_to,
                              usize const &kind_code);
    virtual void moveCallback(usize const &key_code_from,
                              usize const &key_code_to,
                              usize const &kind_code);
    virtual void swapCallback(usize const &key_code_from,
                              usize const &key_code_to,
                              usize const &kind_code);
    virtual std::pair<Callback, bool> popCallback(usize const &key_code,
                                                  usize const &kind_code);

    virtual bool canRepeat(usize const &key_code) const;

   private:
    friend class KeyManager;
    virtual void link();
    virtual void unlink();

    virtual void ownershipCheck() const;
    virtual void codeCheck(usize const &key_code_from,
                           usize const &key_code_to = -1,
                           usize const &kind_code = -1) const;

    struct Inner {
      CallbackStore callbacks;
      std::vector<bool> can_repeat;
      bool is_linked;
    } *ownership;
  }; // KeyMap

  static void keyPress(usize const &key_code);
  static void keyRelease(usize const &key_code);
  static void keyFramework() noexcept;

  static usize getKeyCount() noexcept;

  static bool getKeyState(usize const &key_code);

  static KeyMap const *const &getKeyMap() noexcept;
  static void setKeyMap(KeyMap const *const &key_map);

 private:
  KeyManager() = delete;
  KeyManager(KeyManager const &rhs) = delete;
  KeyManager &operator=(KeyManager const &rhs) = delete;
  ~KeyManager() = delete;

  friend KeyMap &KeyMap::operator=(KeyMap const &) noexcept;
  friend KeyMap::~KeyMap() noexcept;
  friend void KeyMap::setKeyCount(usize const &);
  static void link(KeyMap const *key_map) noexcept;
  static void unlink() noexcept;

  static void codeCheck(usize const &key_code,
                        usize const &kind_code = -1);

  static std::vector<bool> key_state;
  static KeyMap const *key_map;
}; // KeyManager

#endif
