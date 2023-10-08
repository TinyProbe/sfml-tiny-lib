#ifndef MOUSE_MANAGER_HPP_
#define MOUSE_MANAGER_HPP_

#include <functional>
#include <utility>
#include <vector>

#include <SFML/Window/Event.hpp>

using i32 = int;
using usize = unsigned long;
using ButtonCallback = std::function<void(i32, i32)>;
using ButtonCallbackElement = std::vector<ButtonCallback>;
using ButtonCallbackStore = std::vector<ButtonCallbackElement>;

class MouseManager {
 public:
  enum ButtonEvent {
    kPress = 0,
    kRelease,
    kPressed,
    kButtonEventCount,
  };
  class ButtonMap {
   public:
    explicit ButtonMap();
    explicit ButtonMap(usize const &button_count);
    explicit ButtonMap(ButtonMap const &rhs) noexcept;
    virtual ButtonMap &operator=(ButtonMap const &rhs) noexcept;
    virtual ~ButtonMap() noexcept;

    virtual MouseManager::ButtonMap clone() const;

    virtual usize getButtonCount() const;
    virtual void setButtonCount(usize const &button_count);

    virtual ButtonCallbackStore const &getButtonCallbacks() const;

    virtual ButtonCallback const &getButtonCallback(
        usize const &button_code,
        usize const &button_event_code) const;
    virtual void setButtonCallback(usize const &button_code,
                                   usize const &button_event_code,
                                   ButtonCallback const &callback);
    virtual void copyButtonCallback(usize const &button_code_from,
                                    usize const &button_code_to,
                                    usize const &button_event_code);
    virtual void moveButtonCallback(usize const &button_code_from,
                                    usize const &button_code_to,
                                    usize const &button_event_code);
    virtual void swapButtonCallback(usize const &button_code_from,
                                    usize const &button_code_to,
                                    usize const &button_event_code);
    virtual ButtonCallback popButtonCallback(usize const &button_code,
                                             usize const &button_event_code);

   protected:
    struct Inner {
      ButtonCallbackStore callbacks;
      bool is_linked;

      explicit Inner();
      explicit Inner(Inner const &rhs);
      virtual Inner &operator=(Inner const &rhs);
    } *ownership;

   private:
    friend class MouseManager;
    virtual void link();
    virtual void unlink();

    explicit ButtonMap(ButtonMap::Inner *const &ownership) noexcept;
    virtual void ownershipCheck() const;
    virtual void codeCheck(usize const &button_code_from,
                           usize const &button_code_to = -1,
                           usize const &button_event_code = -1) const;
  }; // ButtonMap

  enum MouseEvent {
    kVerScrollUp = 0,
    kVerScrollDown,
    kHorScrollUp,
    kHorScrollDown,
    kMove,
    kEnter,
    kLeave,
    kMouseEventCount,
  };
  static void event(sf::Event const &event);
  static void framework();
  static void framework(sf::WindowBase const &relativeTo);

  static usize getButtonCount() noexcept;

  static bool getButtonState(usize const &button_code);

  static ButtonMap const *const &getButtonMap() noexcept;
  static void setButtonMap(ButtonMap const *const &button_map);

  static ButtonCallback const &getMouseEventCallback(
      usize const &mouse_event_code);
  static void setMouseEventCallback(
      usize const &mouse_event_code, ButtonCallback const &callback);

 private:
  MouseManager() = delete;
  MouseManager(MouseManager const &rhs) = delete;
  MouseManager &operator=(MouseManager const &rhs) = delete;
  ~MouseManager() = delete;

  static void scroll(sf::Event::MouseWheelScrollEvent const &button_event);
  static void press(sf::Event::MouseButtonEvent const &button_event);
  static void release(sf::Event::MouseButtonEvent const &button_event);
  static void move(sf::Event::MouseMoveEvent const &button_event);
  static void enter(sf::Event::MouseMoveEvent const &button_event);
  static void leave(sf::Event::MouseMoveEvent const &button_event);

  friend ButtonMap &ButtonMap::operator=(ButtonMap const &) noexcept;
  friend ButtonMap::~ButtonMap() noexcept;
  friend void ButtonMap::setButtonCount(usize const &);
  static void link(ButtonMap const *const &button_map) noexcept;
  static void unlink() noexcept;

  static void codeCheck(usize const &mouse_event_code);

  static bool is_entered;
  static ButtonMap const *button_map;
  static std::vector<bool> button_state;
  static std::vector<ButtonCallback> mouse_event_callbacks;
}; // MouseManager

#endif
