#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "common.hpp"

#define WIDTH  1280
#define HEIGHT 800

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
  window.setFramerateLimit(60);

  sf::Clock clock;
  usize frameCount = 0;

  sf::Font fnt1;
  if (!fnt1.loadFromFile("/Users/tiny/Library/Fonts/GoMonoNerdFont-Regular.ttf")) {
    throw std::runtime_error("font load failed!");
  }
  sf::Text txt1;
  txt1.setFont(fnt1);
  txt1.setStyle(sf::Text::Regular);
  txt1.setFillColor(sf::Color(0, 255, 0));
  txt1.setCharacterSize(16);
  txt1.setPosition(0, 0);

  sf::Texture txr1;
  if (!txr1.loadFromFile("res/space.png")) {
    throw std::runtime_error("image load failed!");
  }
  sf::RectangleShape rts1;
  rts1.setSize(sf::Vector2f(WIDTH, HEIGHT));
  rts1.setTexture(&txr1);
  rts1.setPosition(0, 0);

  auto event = sf::Event({});
  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {
        window.close();
      }
    }

    ++frameCount;
    if (clock.getElapsedTime() >= sf::seconds(1.0f)) {
      txt1.setString(std::string("FPS: ") + std::to_string(frameCount));
      clock.restart();
      frameCount = 0;
    }

    window.draw(rts1);
    window.draw(txt1);
    window.display();
  }
}
