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

struct SpriteData2 {
    std::string id;
    int x, y;
    int z, t;
    SpriteData2(const std::string& id, int x, int y, int z, int t)
        : id(id), x(x), y(y), z(z), t(t) {}
};

class TextureManager {
public:
    void loadSheet(const std::string& jsonPath, const std::string& imagePath);
    void loadSheet(const std::string& imagePath, const std::vector<SpriteData2>& sprites) {
        // Load texture without JSON metadata
        if (textureMap.find(imagePath) == textureMap.end()) {
            sf::Texture texture;
            if (!texture.loadFromFile(imagePath)) {
                throw std::runtime_error("Failed to load texture: " + imagePath);
            }
            textureMap[imagePath] = std::move(texture);
        }

        for (auto &spr : sprites) {
            SpriteData data;
            data.x = spr.x;
            data.y = spr.y;
            data.z = spr.z;
            data.t = spr.t;
            data.texture = &textureMap[imagePath];

            // Use the sprite ID as the key
            spriteMap[spr.id] = data;
        }
    }

    const SpriteData& getSpriteData(const std::string& name) const;
    static TextureManager& getInstance();

private:
    void loadFromJson(const std::string& jsonPath, sf::Texture& texture);

    std::unordered_map<std::string, sf::Texture> textureMap;
    std::unordered_map<std::string, SpriteData> spriteMap; 
    TextureManager() = default;
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