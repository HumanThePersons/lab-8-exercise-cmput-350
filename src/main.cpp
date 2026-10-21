#include <iostream>

#include "basic_transform.h"
#include "sprite_manager.h"

int main() {
    // Code commented out for CI compiling
    // Uncomment them after implementation

    SpriteManager spriteManager;
    auto& texture = spriteManager.registerTexture("assets/sword_diamond.png");
    auto spriteID = spriteManager.createSprite(texture);
    auto& sprite = spriteManager.getSprite(spriteID);
    sprite.origin() = {114, 14};
    sprite.position() = {128, 128};
    sprite.scale().x = -1;

    sf::RenderWindow window(sf::VideoMode({256, 256}), "Sprites lab", sf::State::Windowed);
    // window.setMouseCursorVisible(false); // you can use this to hide the mouse
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                return 0;
            }
        }
        sprite.position() = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::RenderStates states{};
        window.clear(sf::Color::White);
        sprite.draw(window, states);
        window.display();
    }
    return 0;
}
