#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>
#include "common.hpp"

i32 main() {
  // It work if current os display resolution has support FullScreenMode.
  // Maybe many game do first change os display resolution before apply this.
  // auto fullModes = sf::VideoMode::getFullscreenModes();
  // if (fullModes.empty()) { /* error process */ }
  // sf::RenderWindow window(
  //   sf::VideoMode(fullModes[0].width, fullModes[0].height),
  //   "Fullscreen SFML!",
  //   sf::Style::Fullscreen
  // );
  sf::RenderWindow window(
    sf::VideoMode(WIDTH, HEIGHT),
    "SFML!",
    sf::Style::Titlebar | sf::Style::Close // WindowMode default setting
  );

  // fps manager
  FPSManager fpsmng;
  fpsmng.setFramerateLimit(60);

  // fps counter
  sf::Font fnt1;
  if (!fnt1.loadFromFile("res/font/GoMonoNerdFont-Regular.ttf")) {
    throw std::runtime_error("fnt1 load failed!");
  }
  sf::Text txt1;
  txt1.setFont(fnt1);
  txt1.setStyle(sf::Text::Regular);
  txt1.setFillColor(sf::Color(0, 255, 0));
  txt1.setCharacterSize(16);
  txt1.setPosition(0, 0);

  // background
  sf::Texture txr1;
  if (!txr1.loadFromFile("res/background/space.png")) {
    throw std::runtime_error("txr1 load failed!");
  }
  sf::RectangleShape rts1;
  rts1.setSize(sf::Vector2f(WIDTH, HEIGHT));
  rts1.setTexture(&txr1);
  rts1.setPosition(0, 0);

  // monster
  sf::Image img1;
  if (!img1.loadFromFile("res/Monsters/TrollDark.PNG")) {
    throw std::runtime_error("img1 load failed!");
  }
  img1.createMaskFromColor(img1.getPixel(0, 0)); // image background to invisible
  sf::Texture txr2;
  if (!txr2.loadFromImage(img1)) {
    throw std::runtime_error("txr2 load failed!");
  }
  sf::RectangleShape rts2;
  rts2.setSize(sf::Vector2f(100, 100));
  rts2.setTexture(&txr2);
  rts2.setPosition(300, 300);

  // background music
  sf::Music msc1;
  if (!msc1.openFromFile("res/sound/bgm.mp3")) {
    throw std::runtime_error("msc1 load failed!");
  }
  msc1.setVolume(100);
  msc1.setLoop(true);
  msc1.play();

  // attack sound
  sf::Music msc2;
  if (!msc2.openFromFile("res/sound/attack.mp3.flac")) {
    throw std::runtime_error("msc2 load failed!");
  }
  msc2.setVolume(100);
  msc2.setLoop(false);

  // key manager & key map
  KeyManager keymng(sf::Keyboard::KeyCount);
  KeyManager::KeyMap keymap(sf::Keyboard::KeyCount);
  keymap.setCallback(sf::Keyboard::Escape, KeyManager::Press, [&]() {
    window.close();
  });
  keymap.setCallback(sf::Keyboard::Up, KeyManager::Pressed, [&]() {
    rts2.move(0, -MOVE_UNIT);
  });
  keymap.setCallback(sf::Keyboard::Down, KeyManager::Pressed, [&]() {
    rts2.move(0, MOVE_UNIT);
  });
  keymap.setCallback(sf::Keyboard::Left, KeyManager::Pressed, [&]() {
    rts2.move(-MOVE_UNIT, 0);
  });
  keymap.setCallback(sf::Keyboard::Right, KeyManager::Pressed, [&]() {
    rts2.move(MOVE_UNIT, 0);
  });
  keymap.setCallback(sf::Keyboard::Hyphen, KeyManager::Press, [&]() {
    msc1.setVolume(fmax(msc1.getVolume() - 3, 0));
  }, true);
  keymap.setCallback(sf::Keyboard::Equal, KeyManager::Press, [&]() {
    msc1.setVolume(fmin(msc1.getVolume() + 3, 100));
  }, true);
  keymap.setCallback(sf::Keyboard::LBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() - sf::seconds(2));
  }, true);
  keymap.setCallback(sf::Keyboard::RBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() + sf::seconds(2));
  }, true);
  keymap.setCallback(sf::Keyboard::Space, KeyManager::Press, [&]() {
    msc2.play();
  });
  keymng.setKeyMap(&keymap);

  auto event = sf::Event({});
  while (window.isOpen()) {
    fpsmng.framePulse();
    txt1.setString(std::to_string(fpsmng.getCurrentFPS()));
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {
        keymng.keyPress(event.key.code);
      } else if (event.type == sf::Event::KeyReleased) {
        keymng.keyRelease(event.key.code);
      }
    }

    keymng.keyFramework();

    window.draw(rts1);
    window.draw(rts2);
    window.draw(txt1);
    window.display();
  }
}
