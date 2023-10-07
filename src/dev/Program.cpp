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
    sf::VideoMode({ kWidth, kHeight }),
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
  WrapImage img1("res/background/ereve.jpg");
  WrapTexture tex1(img1.getImage());
  sf::RectangleShape rts1;
  rts1.setSize(sf::Vector2f({ kWidth, kHeight }));
  rts1.setTexture(&tex1.getTexture());
  rts1.setPosition(0, 0);

  // monster
  WrapImage img2("res/Monsters/TrollDark.PNG");
  img2.createMaskFromColor(img2.getPixel(0, 0)); // background to invisible
  WrapTexture tex2(img2.getImage());
  sf::RectangleShape rts2;
  rts2.setSize(sf::Vector2f({ 100, 100 }));
  rts2.setTexture(&tex2.getTexture());
  rts2.setPosition(300, 300);

  // sprite
  WrapImage img3("res/sprite/red_drake.png");
  WrapTexture tex3(img3.getImage());
  sf::Sprite spr1;
  spr1.setTexture(tex3.getTexture());
  spr1.setTextureRect(sf::IntRect({ 0, 0, 130, 100 }));
  spr1.setPosition(200, 200);
  spr1.setOrigin(spr1.getTextureRect().width / 2,
                 spr1.getTextureRect().height);

  // animation ownership pattern test
  // Animation anm1(AnimeStore({
  //   {
  //     { sf::IntRect({ 0, 0, 100, 100 }), sf::milliseconds(100), },
  //   },
  // }));
  // Animation anm2(anm1.clone());
  // AnimeStore const &animes = anm2.getAnimes();
  // std::cout << animes[0][0].first.width << '\n';
  // AnimeStore const &asd = anm1.getAnimes();
  // std::cout << asd[0][0].first.width << '\n';

  sf::SoundBuffer sbf1, sbf2;
  if (!sbf1.loadFromFile("res/sound/ereve.mp3")) {
    throw std::runtime_error("sbf1 load failed!");
  }
  if (!sbf2.loadFromFile("res/sound/attack.mp3.flac")) {
    throw std::runtime_error("sbf2 load failed!");
  }
  sf::Sound snd1, snd2;
  // background music
  snd1.setBuffer(sbf1);
  snd1.setVolume(100);
  snd1.setLoop(true);
  snd1.play();
  // attack sound
  snd2.setBuffer(sbf2);
  snd2.setVolume(100);
  snd2.setLoop(false);

  // key manager & key map
  KeyManager::KeyMap kmap(sf::Keyboard::KeyCount);
  KeyManager::setKeyMap(&kmap);
  kmap.setCallback(sf::Keyboard::Escape, KeyManager::kPress, [&]() {
    window.close();
  });
  kmap.setCallback(sf::Keyboard::Hyphen, KeyManager::kPress, [&]() {
    snd1.setVolume(fmax(snd1.getVolume() - 3, 0));
  }, true);
  kmap.setCallback(sf::Keyboard::Equal, KeyManager::kPress, [&]() {
    snd1.setVolume(fmin(snd1.getVolume() + 3, 100));
  }, true);
  kmap.setCallback(sf::Keyboard::LBracket, KeyManager::kPress, [&]() {
    snd1.setPlayingOffset(snd1.getPlayingOffset() - sf::seconds(2));
  }, true);
  kmap.setCallback(sf::Keyboard::RBracket, KeyManager::kPress, [&]() {
    snd1.setPlayingOffset(snd1.getPlayingOffset() + sf::seconds(2));
  }, true);
  kmap.setCallback(sf::Keyboard::Space, KeyManager::kPress, [&]() {
    snd2.play();
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

    spr1.setRotation(usize(spr1.getRotation() + 1.0f) % 360);

    // render
    window.draw(rts1);
    window.draw(rts2);
    window.draw(spr1);
    window.draw(txt1);
    window.display();

    // fps managing
    FPSManager::framePulse();
  }
}

void Program::loop() {

}
