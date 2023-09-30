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
    sf::VideoMode({ WIDTH, HEIGHT }),
    "SFML!",
    sf::Style::Titlebar | sf::Style::Close // WindowMode default setting
  );

  // fps manager
  FPSManager fpsmng;
  fpsmng.setFramerateLimit(120);

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
  ImageTexture itxr2;
  itxr2.loadFromFile("res/background/ereve.jpg");
  sf::RectangleShape rts1;
  rts1.setSize(sf::Vector2f({ WIDTH, HEIGHT }));
  rts1.setTexture(itxr2);
  rts1.setPosition(0, 0);

  // monster
  ImageTexture itxr1;
  itxr1.loadFromFile("res/Monsters/TrollDark.PNG");
  itxr1.createMaskFromColor(itxr1.getPixel(0, 0)); // image background to invisible
  sf::RectangleShape rts2;
  rts2.setSize(sf::Vector2f({ 100, 100 }));
  rts2.setTexture(itxr1);
  rts2.setPosition(300, 300);

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

  // [objects]           [resources]
  //             Image + Texture
  // Drawable -> { ImageTexture} + Animation
  // Sprite   -> {      SpriteTexture      }  // need Sprite2??
  // => std::vector<ImageTexture>[IMAGE_CODE] => link to objects
  // => std::vector<SpriteTexture>[SPRITE_CODE] => link to objects
  //
  // Image list : backgrounds, sculptures, terrains, interfaces, mobs,
  // npcs, players, effects,
  // Texture list : same to Image
  // rendering order : order by z depth

  // sprite
  enum Anime {
    Idle,
    Move,
    AnimeCount,
  };
  SpriteTexture stxr1;
  stxr1.loadFromFile("res/sprite/red_drake.png");
  stxr1.setAnimeCount(AnimeCount);
  stxr1.setAnime(Idle, {
    { { 3, 5, 129, 83 }, sf::milliseconds(0), },
  });
  stxr1.setAnime(Move, {
    { { 4, 113, 133, 75 }, sf::milliseconds(300), },
    { { 149, 113, 127, 82 }, sf::milliseconds(300), },
    { { 289, 113, 124, 76 }, sf::milliseconds(300), },
    { { 430, 113, 126, 82 }, sf::milliseconds(300), },
  });
  AutoSprite aspr1;
  aspr1.setSpriteTexture(stxr1);
  aspr1.setCurrentAnime(Idle);
  aspr1.setPosition(400, 578); // left, top pointing is sometimes unmatchable floor position.

  AutoSprite aspr2;
  aspr2.setSpriteTexture(stxr1);
  aspr2.setCurrentAnime(Move);
  aspr2.setPosition(550, 584);
  aspr2.setColor(sf::Color(200, 200, 255, 180)); // activate colorate.

  // key manager & key map
  KeyManager::KeyMap km(sf::Keyboard::KeyCount);
  KeyManager keymng(km);
  km.setCallback(sf::Keyboard::Escape, KeyManager::Press, [&]() {
    window.close();
  });
  km.setCallback(sf::Keyboard::Up, KeyManager::Pressed, [&]() {
    rts2.move(0, -MOVE_UNIT);
  });
  km.setCallback(sf::Keyboard::Down, KeyManager::Pressed, [&]() {
    rts2.move(0, MOVE_UNIT);
  });
  km.setCallback(sf::Keyboard::Left, KeyManager::Pressed, [&]() {
    rts2.move(-MOVE_UNIT, 0);
  });
  km.setCallback(sf::Keyboard::Right, KeyManager::Pressed, [&]() {
    rts2.move(MOVE_UNIT, 0);
  });
  km.setCallback(sf::Keyboard::Hyphen, KeyManager::Press, [&]() {
    msc1.setVolume(fmax(msc1.getVolume() - 3, 0));
  }, true);
  km.setCallback(sf::Keyboard::Equal, KeyManager::Press, [&]() {
    msc1.setVolume(fmin(msc1.getVolume() + 3, 100));
  }, true);
  km.setCallback(sf::Keyboard::LBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() - sf::seconds(2));
  }, true);
  km.setCallback(sf::Keyboard::RBracket, KeyManager::Press, [&]() {
    msc1.setPlayingOffset(msc1.getPlayingOffset() + sf::seconds(2));
  }, true);
  km.setCallback(sf::Keyboard::Space, KeyManager::Press, [&]() {
    msc2.play();
  });

  sf::Event event;
  while (window.isOpen()) {
    // update
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
    txt1.setString(std::to_string(fpsmng.getCurrentFPS()));

    // render
    window.draw(rts1);
    window.draw(rts2);
    window.draw(txt1);
    window.draw(aspr1);
    window.draw(aspr2);
    window.display();

    // fps managing
    fpsmng.framePulse();
  }
}
