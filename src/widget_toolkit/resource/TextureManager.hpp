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