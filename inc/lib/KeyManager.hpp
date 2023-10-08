#ifndef KEY_MANAGER_HPP_
#define KEY_MANAGER_HPP_

#include <functional>
#include <utility>
#include <vector>

#include <SFML/Window/Event.hpp>

using usize = unsigned long;
using KeyCallback = std::function<void()>;
using KeyCallbackElement = std::vector<KeyCallback>;
using KeyCallbackStore = std::vector<KeyCallbackElement>;

class KeyManager {
 public:
  enum KeyEvent {
    kPress = 0,
    kRelease,
    kPressed,
    kKeyEventCount,
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

    virtual KeyCallbackStore const &getKeyCallbacks() const;

    virtual KeyCallback const &getKeyCallback(usize const &key_code,
                                              usize const &key_event_code) const;
    virtual void setKeyCallback(usize const &key_code,
                                usize const &key_event_code,
                                KeyCallback const &callback,
                                bool const &can_repeat = false);
    virtual void copyKeyCallback(usize const &key_code_from,
                                 usize const &key_code_to,
                                 usize const &key_event_code);
    virtual void moveKeyCallback(usize const &key_code_from,
                                 usize const &key_code_to,
                                 usize const &key_event_code);
    virtual void swapKeyCallback(usize const &key_code_from,
                                 usize const &key_code_to,
                                 usize const &key_event_code);
    virtual std::pair<KeyCallback, bool> popKeyCallback(usize const &key_code,
                                                        usize const &key_event_code);

    virtual bool canRepeat(usize const &key_code) const;

   protected:
    struct Inner {
      KeyCallbackStore callbacks;
      std::vector<bool> can_repeat;
      bool is_linked;

      explicit Inner();
      explicit Inner(Inner const &rhs);
      virtual Inner &operator=(Inner const &rhs);
    } *ownership;

   private:
    friend class KeyManager;
    virtual void link();
    virtual void unlink();

    explicit KeyMap(KeyMap::Inner *const &ownership) noexcept;
    virtual void ownershipCheck() const;
    virtual void codeCheck(usize const &key_code_from,
                           usize const &key_code_to = -1,
                           usize const &key_event_code = -1) const;
  }; // KeyMap

  static void event(sf::Event const &event);
  static void framework();

  static usize getKeyCount() noexcept;

  static bool getKeyState(usize const &key_code);

  static KeyMap const *const &getKeyMap() noexcept;
  static void setKeyMap(KeyMap const *const &key_map);

 private:
  KeyManager() = delete;
  KeyManager(KeyManager const &rhs) = delete;
  KeyManager &operator=(KeyManager const &rhs) = delete;
  ~KeyManager() = delete;

  static void press(sf::Event::KeyEvent const &key_event);
  static void release(sf::Event::KeyEvent const &key_event);

  friend KeyMap &KeyMap::operator=(KeyMap const &) noexcept;
  friend KeyMap::~KeyMap() noexcept;
  friend void KeyMap::setKeyCount(usize const &);
  static void link(KeyMap const *const &key_map) noexcept;
  static void unlink() noexcept;

  static std::vector<bool> key_state;
  static KeyMap const *key_map;
}; // KeyManager

#endif
