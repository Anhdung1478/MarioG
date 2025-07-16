#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

struct SpriteData {
    int x, y;
    int z, t; // z = width and t = height for sprite
    const sf::Texture* texture; 
};

class TextureManager {
public:
    void loadSheet(const std::string& jsonPath, const std::string& imagePath);

    const SpriteData& getSpriteData(const std::string& name) const;

private:
    void loadFromJson(const std::string& jsonPath, sf::Texture& texture);

    std::unordered_map<std::string, sf::Texture> textureMap;
    std::unordered_map<std::string, SpriteData> spriteMap; 
};

#endif

// HOW TO USE
// First, create a TextureManager 
// TextureManager textureManager;

// Load a sprite sheet + its JSON metadata
// textureManager.loadSheet("mario.json", "mario-sheets.png");
// const SpriteData& mario = textureManager.getSpriteData("mario-small.walk[0]");
// setTexture + setTextureRect

// All sprites from mario.json are now loaded.
// - Loads the mario.png image file
// - Reads mario.json to see where each sprite is located
// - Creates names automatically for each sprite