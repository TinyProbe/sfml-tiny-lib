#include "lib/MouseManager.hpp"

#include <stdexcept>
#include <iostream>

#include <SFML/Window/Mouse.hpp>

// ButtonMap
MouseManager::ButtonMap::ButtonMap()
    : ownership(new MouseManager::ButtonMap::Inner()) {
  ownership->is_linked_ = bool();
}

MouseManager::ButtonMap::ButtonMap(usize const &button_count)
    : ownership(new MouseManager::ButtonMap::Inner()) {
  ownership->callbacks_.resize(
      button_count, ButtonCallbackElement(MouseManager::kButtonEventCount));
  ownership->is_linked_ = bool();
}

MouseManager::ButtonMap::ButtonMap(MouseManager::ButtonMap const &rhs) noexcept
    : ownership() {
  *this = rhs;
}

MouseManager::ButtonMap &MouseManager::ButtonMap::operator=(
    MouseManager::ButtonMap const &rhs) noexcept {
  if (this == &rhs) { return *this; }
  if (ownership != nullptr) {
    if (ownership->is_linked_) { MouseManager::unlink(); }
    delete ownership;
  }
  ownership = rhs.ownership;
  const_cast<MouseManager::ButtonMap &>(rhs).ownership = nullptr;
  if (ownership != nullptr) {
    if (ownership->is_linked_) { MouseManager::link(this); }
  }
  return *this;
}

MouseManager::ButtonMap::~ButtonMap() noexcept {
  if (ownership != nullptr) {
    if (ownership->is_linked_) { MouseManager::unlink(); }
    delete ownership;
  }
}

MouseManager::ButtonMap MouseManager::ButtonMap::clone() const {
  this->ownershipCheck();
  return MouseManager::ButtonMap(
      new MouseManager::ButtonMap::Inner(*ownership));
}

usize MouseManager::ButtonMap::getButtonCount() const {
  this->ownershipCheck();
  return ownership->callbacks_.size();
}

void MouseManager::ButtonMap::setButtonCount(usize const &button_count) {
  this->ownershipCheck();
  ownership->callbacks_.resize(
      button_count, ButtonCallbackElement(MouseManager::kButtonEventCount));
  if (ownership->is_linked_) {
    MouseManager::button_state_.resize(button_count);
  }
}

ButtonCallbackStore const &MouseManager::ButtonMap::getButtonCallbacks() const {
  this->ownershipCheck();
  return ownership->callbacks_;
}

ButtonCallback const &MouseManager::ButtonMap::getButtonCallback(
    usize const &button_code,
    usize const &button_event_code) const {
  this->codeCheck(button_code, usize(-1), button_event_code);
  return ownership->callbacks_[button_code][button_event_code];
}

void MouseManager::ButtonMap::setButtonCallback(usize const &button_code,
                                                usize const &button_event_code,
                                                ButtonCallback const &callback
                                                ) {
  this->codeCheck(button_code, usize(-1), button_event_code);
  ownership->callbacks_[button_code][button_event_code] = callback;
}

void MouseManager::ButtonMap::copyButtonCallback(usize const &button_code_from,
                                                 usize const &button_code_to,
                                                 usize const &button_event_code
                                                 ) {
  this->codeCheck(button_code_from, button_code_to, button_event_code);
  ownership->callbacks_[button_code_to][button_event_code] =
      ownership->callbacks_[button_code_from][button_event_code];
}

void MouseManager::ButtonMap::moveButtonCallback(usize const &button_code_from,
                                                 usize const &button_code_to,
                                                 usize const &button_event_code
                                                 ) {
  this->codeCheck(button_code_from, button_code_to, button_event_code);
  ownership->callbacks_[button_code_to][button_event_code] =
      std::move(ownership->callbacks_[button_code_from][button_event_code]);
  ownership->callbacks_[button_code_from][button_event_code] = ButtonCallback();
}

void MouseManager::ButtonMap::swapButtonCallback(usize const &button_code_from,
                                                 usize const &button_code_to,
                                                 usize const &button_event_code
                                                 ) {
  this->codeCheck(button_code_from, button_code_to, button_event_code);
  ButtonCallback tmp =
      std::move(ownership->callbacks_[button_code_to][button_event_code]);
  ownership->callbacks_[button_code_to][button_event_code] =
      std::move(ownership->callbacks_[button_code_from][button_event_code]);
  ownership->callbacks_[button_code_from][button_event_code] = std::move(tmp);
}

ButtonCallback MouseManager::ButtonMap::popButtonCallback(
    usize const &button_code,
    usize const &button_event_code) {
  this->codeCheck(button_code, usize(-1), button_event_code);
  ButtonCallback tmp =
      std::move(ownership->callbacks_[button_code][button_event_code]);
  ownership->callbacks_[button_code][button_event_code] = ButtonCallback();
  return ButtonCallback(std::move(tmp));
}

MouseManager::ButtonMap::Inner::Inner() {
}

MouseManager::ButtonMap::Inner::Inner(Inner const &rhs) {
  *this = rhs;
}

MouseManager::ButtonMap::Inner &MouseManager::ButtonMap::Inner::operator=(
    MouseManager::ButtonMap::Inner const &rhs) {
  if (this == &rhs) { return *this; }
  this->callbacks_.assign(rhs.callbacks_.begin(), rhs.callbacks_.end());
  this->is_linked_ = rhs.is_linked_;
  return *this;
}

void MouseManager::ButtonMap::link() {
  this->ownershipCheck();
  ownership->is_linked_ = true;
}

void MouseManager::ButtonMap::unlink() {
  this->ownershipCheck();
  ownership->is_linked_ = false;
}

MouseManager::ButtonMap::ButtonMap(
    MouseManager::ButtonMap::Inner *const &ownership) noexcept
    : ownership(ownership) {
}

void MouseManager::ButtonMap::ownershipCheck() const {
  if (ownership == nullptr) {
    throw std::runtime_error(
        "No ownership rights whatsoever: MouseManager::ButtonMap");
  }
}

void MouseManager::ButtonMap::codeCheck(usize const &button_code_from,
                                        usize const &button_code_to,
                                        usize const &button_event_code) const {
  this->ownershipCheck();
  if (button_code_from >= ownership->callbacks_.size()) {
    throw std::runtime_error("No exist button_code_from.");
  } 
  if (button_code_to != usize(-1) &&
      button_code_to >= ownership->callbacks_.size()) {
    throw std::runtime_error("No exist button_code_to.");
  }
  if (button_event_code != usize(-1) &&
      button_event_code >= MouseManager::kButtonEventCount) {
    throw std::runtime_error("No exist button_event_code.");
  }
}

// MouseManaer
bool MouseManager::is_entered_ = true;
MouseManager::ButtonMap const *MouseManager::button_map_ = nullptr;
std::vector<bool> MouseManager::button_state_;
std::vector<ButtonCallback> MouseManager::mouse_event_callbacks_(
    MouseManager::kMouseEventCount);

void MouseManager::eventProcess(sf::Event const &event) {
  if (event.type == sf::Event::MouseWheelScrolled) {
    MouseManager::scroll(event.mouseWheelScroll);
  } else if (event.type == sf::Event::MouseButtonPressed) {
    MouseManager::press(event.mouseButton);
  } else if (event.type == sf::Event::MouseButtonReleased) {
    MouseManager::release(event.mouseButton);
  } else if (event.type == sf::Event::MouseMoved) {
    MouseManager::move(event.mouseMove);
  } else if (event.type == sf::Event::MouseEntered) {
    MouseManager::enter(event.mouseMove);
  } else if (event.type == sf::Event::MouseLeft) {
    MouseManager::leave(event.mouseMove);
  }
}

void MouseManager::framework() {
  if (MouseManager::button_map_ != nullptr) {
    ButtonCallbackStore const &callbacks =
        MouseManager::button_map_->getButtonCallbacks();
    sf::Vector2i position = sf::Mouse::getPosition();
    for (usize i = MouseManager::button_state_.size(); i--; ) {
      if (MouseManager::button_state_[i] &&
          callbacks[i][MouseManager::kPressed]) {
        callbacks[i][MouseManager::kPressed](position.x, position.y);
      }
    }
  }
}

void MouseManager::framework(sf::WindowBase const &relativeTo) {
  if (MouseManager::button_map_ != nullptr) {
    ButtonCallbackStore const &callbacks =
        MouseManager::button_map_->getButtonCallbacks();
    sf::Vector2i position = sf::Mouse::getPosition(relativeTo);
    for (usize i = MouseManager::button_state_.size(); i--; ) {
      if (MouseManager::button_state_[i] &&
          callbacks[i][MouseManager::kPressed]) {
        callbacks[i][MouseManager::kPressed](position.x, position.y);
      }
    }
  }
}

bool MouseManager::getIsEntered() noexcept {
  return MouseManager::is_entered_;
}

MouseManager::ButtonMap const *const &MouseManager::getButtonMap() noexcept {
  return MouseManager::button_map_;
}

void MouseManager::setButtonMap(
    MouseManager::ButtonMap const *const &button_map) {
  if (MouseManager::button_map_ != nullptr) {
    const_cast<MouseManager::ButtonMap *>(MouseManager::button_map_)->unlink();
  }
  MouseManager::link(button_map);
  if (MouseManager::button_map_ != nullptr) {
    MouseManager::button_state_.resize(button_map->getButtonCount());
    const_cast<MouseManager::ButtonMap *>(MouseManager::button_map_)->link();
  }
}

usize MouseManager::getButtonCount() noexcept {
  return MouseManager::button_state_.size();
}

bool MouseManager::getButtonState(usize const &button_code) {
  if (MouseManager::button_map_ == nullptr) {
    throw std::runtime_error("No linked any ButtonMap.");
  }
  MouseManager::button_map_->codeCheck(button_code);
  return MouseManager::button_state_[button_code];
}

ButtonCallback const &MouseManager::getMouseEventCallback(
    usize const &mouse_event_code) {
  MouseManager::codeCheck(mouse_event_code);
  return MouseManager::mouse_event_callbacks_[mouse_event_code];
}

void MouseManager::setMouseEventCallback(usize const &mouse_event_code,
                                         ButtonCallback const &callback) {
  MouseManager::codeCheck(mouse_event_code);
  MouseManager::mouse_event_callbacks_[mouse_event_code] = callback;
}

void MouseManager::scroll(
    sf::Event::MouseWheelScrollEvent const &button_event) {
  if (button_event.wheel == sf::Mouse::VerticalWheel) {
    if (button_event.delta > 0) {
      ButtonCallback const &callback =
          MouseManager::mouse_event_callbacks_[MouseManager::kVerScrollUp];
      if (callback) { callback(button_event.x, button_event.y); }
    } else if (button_event.delta < 0) {
      ButtonCallback const &callback =
          MouseManager::mouse_event_callbacks_[MouseManager::kVerScrollDown];
      if (callback) { callback(button_event.x, button_event.y); }
    }
  } else {
    if (button_event.delta > 0) {
      ButtonCallback const &callback =
          MouseManager::mouse_event_callbacks_[MouseManager::kHorScrollUp];
      if (callback) { callback(button_event.x, button_event.y); }
    } else if (button_event.delta < 0) {
      ButtonCallback const &callback =
          MouseManager::mouse_event_callbacks_[MouseManager::kHorScrollDown];
      if (callback) { callback(button_event.x, button_event.y); }
    }
  }
}

void MouseManager::press(
    sf::Event::MouseButtonEvent const &button_event) {
  if (MouseManager::button_map_ != nullptr) {
    MouseManager::button_map_->codeCheck(button_event.button);
    ButtonCallback const &callback =
        MouseManager::button_map_->getButtonCallback(button_event.button,
                                                    MouseManager::kPress);
    if (callback) { callback(button_event.x, button_event.y); }
  }
  MouseManager::button_state_[button_event.button] = true;
}

void MouseManager::release(
    sf::Event::MouseButtonEvent const &button_event) {
  if (MouseManager::button_map_ != nullptr) {
    MouseManager::button_map_->codeCheck(button_event.button);
    ButtonCallback const &callback =
        MouseManager::button_map_->getButtonCallback(button_event.button,
                                                    MouseManager::kRelease);
    if (callback) { callback(button_event.x, button_event.y); }
  }
  MouseManager::button_state_[button_event.button] = false;
}

void MouseManager::move(
    sf::Event::MouseMoveEvent const &button_event) {
  ButtonCallback const &callback =
      MouseManager::mouse_event_callbacks_[MouseManager::kMove];
  if (callback) { callback(button_event.x, button_event.y); }
}

void MouseManager::enter(
    sf::Event::MouseMoveEvent const &button_event) {
  ButtonCallback const &callback =
      MouseManager::mouse_event_callbacks_[MouseManager::kEnter];
  if (callback) { callback(button_event.x, button_event.y); }
  MouseManager::is_entered_ = true;
}

void MouseManager::leave(
    sf::Event::MouseMoveEvent const &button_event) {
  ButtonCallback const &callback =
      MouseManager::mouse_event_callbacks_[MouseManager::kLeave];
  if (callback) { callback(button_event.x, button_event.y); }
  MouseManager::is_entered_ = false;
}

void MouseManager::link(
    MouseManager::ButtonMap const *const &button_map) noexcept {
  MouseManager::button_map_ = button_map;
}

void MouseManager::unlink() noexcept {
  MouseManager::button_map_ = nullptr;
}

void MouseManager::codeCheck(usize const &mouse_event_code) {
  if (mouse_event_code >= MouseManager::kMouseEventCount) {
    throw std::runtime_error("No exist mouse_event_code.");
  }
}
