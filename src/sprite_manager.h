#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sprite.h"
#include <SFML/Graphics.hpp>

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

using SpriteID = size_t;

class SpriteManager {
public:
    sf::Texture& registerTexture(const std::string& filepath) {
        // Read texture from a filepath
        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();  // unique pointer

        if (!newTexture->loadFromFile(filepath)) {
            throw std::runtime_error("Texture couldn't be read from filename");  // Texture wasn't read
        }
        
        // move relinquishes control of the pointer to textures to deal with
        textures.push_back(std::move(newTexture));

        return *textures.back();  // Cant reference newTexture directly, since it was moved to textures
    }

    template <typename... Args> 
    SpriteID createSprite(Args&&... args) {
        // Create a sprite from args
        Sprite newSprite = Sprite(std::forward<Args>(args)...);

        SpriteID id;  // Sets a new id for the sprite
        id = sprites.size();
        sprites.push_back(newSprite);

        return id;
    }

    Sprite& getSprite(SpriteID id) {
        // essentially replaces the & operator from lab 7 implementation
        assert(valid(id));
        return sprites[id];
    }

    bool valid(size_t id) const 
    { 
        return (id < sprites.size()); 
    }

private:
    std::vector<Sprite> sprites;
    std::vector<std::unique_ptr<sf::Texture>> textures;  // holds textures as unique pointers so the references aren't destroyed

};

#endif  // SPRITE_MANAGER_H