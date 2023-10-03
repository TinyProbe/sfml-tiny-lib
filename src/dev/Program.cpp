#include "common.hpp"

void Program::run() {
  // auto fullModes = sf::VideoMode::getFullscreenModes();
  // if (fullModes.empty()) { /* error process */ }
  // sf::RenderWindow window(
  //   sf::VideoMode(fullModes[0].width, fullModes[0].height),
  //   "Fullscreen SFML!",
  //   sf::Style::Fullscreen
  // );
  sf::RenderWindow window(
    sf::VideoMode({ WIDTH, HEIGHT }),
    "SFML!",
    sf::Style::Titlebar | sf::Style::Close // WindowMode default setting
  );

  // fps manager
  FPSManager::setFramerateLimit(120);

  // fps counter
  sf::Font fnt1;
  if (!fnt1.loadFromFile("res/font/GoMonoNerdFont-Regular.ttf")) {
    throw std::runtime_error("fnt1 load failed!");
  }
  sf::Text txt1;
  txt1.setFont(fnt1);
  txt1.setStyle(sf::Text::Regular);
  txt1.setFillColor(sf::Color({ 255, 0, 0 }));
  txt1.setCharacterSize(16);
  txt1.setPosition(0, 0);

  // background
  WrapImage img1;
  img1.loadFromFile("res/background/ereve.jpg");
  WrapTexture tex1;
  tex1.loadFromImage(img1);
  sf::RectangleShape rts1;
  rts1.setSize(sf::Vector2f({ WIDTH, HEIGHT }));
  rts1.setTexture(tex1);
  rts1.setPosition(0, 0);

  // monster
  WrapImage img2;
  img2.loadFromFile("res/Monsters/TrollDark.PNG");
  img2.createMaskFromColor(img2.getPixel(0, 0)); // image background to invisible
  WrapTexture tex2;
  tex2.loadFromImage(img2);
  sf::RectangleShape rts2;
  rts2.setSize(sf::Vector2f({ 100, 100 }));
  rts2.setTexture(tex2);
  rts2.setPosition(300, 300);

  // test
  enum {
    GreenMushStand,
    GreenMushMove,
    GreenMushCount,
  };
  SpriteGenerator sgn(GreenMushCount);
  sgn.pushBackImage(GreenMushStand, WrapImage("res/maple_res/mob/green_mushroom/stand/1110100.img.stand.0.png"));
  sgn.pushBackImage(GreenMushStand, WrapImage("res/maple_res/mob/green_mushroom/stand/1110100.img.stand.1.png"));
  sgn.pushBackImage(GreenMushStand, WrapImage("res/maple_res/mob/green_mushroom/stand/1110100.img.stand.2.png"));
  sgn.pushBackImage(GreenMushMove, WrapImage("res/maple_res/mob/green_mushroom/move/1110100.img.move.0.png"));
  sgn.pushBackImage(GreenMushMove, WrapImage("res/maple_res/mob/green_mushroom/move/1110100.img.move.1.png"));
  sgn.pushBackImage(GreenMushMove, WrapImage("res/maple_res/mob/green_mushroom/move/1110100.img.move.2.png"));
  sgn.pushBackImage(GreenMushMove, WrapImage("res/maple_res/mob/green_mushroom/move/1110100.img.move.3.png"));
  sgn.setHoldingTime(sf::milliseconds(150));
  SpriteAtlas sat3 = sgn.generateSpriteAtlas();
  AutoSprite aspr3;
  aspr3.setSpriteAtlas(sat3);
  aspr3.setCurrentAnime(GreenMushMove);
  aspr3.setPosition(100, 100);

  // background music
  sf::Music msc1;
  if (!msc1.openFromFile("res/sound/ereve.mp3")) {
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

  // sprite
  enum {
    Idle,
    Move,
    AnimeCount,
  };
  SpriteAtlas sat1;
  sat1.loadFromFile("res/sprite/red_drake.png");
  sat1.setAnimeCount(AnimeCount);
  sat1.setAnime(Idle, {
    { { 3, 5, 129, 83 }, sf::milliseconds(0), },
  });
  sat1.setAnime(Move, {
    { { 4, 113, 133, 75 }, sf::milliseconds(300), },
    { { 149, 113, 127, 82 }, sf::milliseconds(300), },
    { { 289, 113, 124, 76 }, sf::milliseconds(300), },
    { { 430, 113, 126, 82 }, sf::milliseconds(300), },
  });
  AutoSprite aspr1;
  aspr1.setSpriteAtlas(sat1);
  aspr1.setCurrentAnime(Move);
  aspr1.setPosition(400, 578); // left, top pointing is sometimes unmatchable floor position.
  aspr1.setColor(sf::Color(255, 255, 255, 150)); // apply colorate.

  // key manager & key map
  KeyManager::KeyMap kmap(sf::Keyboard::KeyCount);
  KeyManager::setKeyMap(kmap);
  kmap.setCallback(sf::Keyboard::Escape, KeyManager::Press, [&]() {
    window.close();
  });
  kmap.setCallback(sf::Keyboard::Hyphen, KeyManager::Press, [&]() {
    msc1.setVolume(fmax(msc1.getVolume() - 3, 0));
  }, true);
  kmap.setCallback(sf::Keyboard::Equal, KeyManager::Press, [&]() {
    msc1.setVolume(fmin(msc1.getVolume() + 3, 100));
  }, true);
  kmap.setCallback(sf::Keyboard::LBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() - sf::seconds(2));
  }, true);
  kmap.setCallback(sf::Keyboard::RBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() + sf::seconds(2));
  }, true);
  kmap.setCallback(sf::Keyboard::Space, KeyManager::Press, [&]() {
    msc2.play();
  });

  sf::Event event;
  while (window.isOpen()) {
    // update
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {
        KeyManager::keyPress(event.key.code);
      } else if (event.type == sf::Event::KeyReleased) {
        KeyManager::keyRelease(event.key.code);
      }
    }
    KeyManager::keyFramework();
    txt1.setString(std::to_string(FPSManager::getCurrentFPS()));

    // render
    window.draw(rts1);
    window.draw(rts2);
    window.draw(aspr3);
    window.draw(aspr1);
    window.draw(txt1);
    window.display();

    // fps managing
    FPSManager::framePulse();
  }
}

void Program::loop() {

}
