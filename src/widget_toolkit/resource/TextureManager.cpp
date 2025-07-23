#include "TextureManager.hpp"
#include "json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

void TextureManager::loadSheet(const std::string& jsonPath, const std::string& imagePath) {
    // Only load texture if it hasn't been loaded yet
    if (textureMap.find(imagePath) == textureMap.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(imagePath)) {
            throw std::runtime_error("Failed to load texture: " + imagePath);
        }
        // Use imagePath as a key for textures/sprites
        textureMap[imagePath] = std::move(texture);
    }

    loadFromJson(jsonPath, textureMap[imagePath]);
}

void TextureManager::loadFromJson(const std::string& jsonPath, sf::Texture& texture) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + jsonPath);
    }

    json j;
    file >> j;

    for (auto& [key, value] : j.items()) {
        if (value.is_array()) {
            // flat list of frames
            for (size_t i = 0; i < value.size(); ++i) {
                auto& frame = value[i];

                SpriteData data;
                data.x = frame["x"];
                data.y = frame["y"];
                data.z = frame["width"];
                data.t = frame["height"];
                data.texture = &texture;

                // "red-mushroom[0]"
                std::string name = key + "[" + std::to_string(i) + "]";
                spriteMap[name] = data;
            }
        } else if (value.is_object()) {
            // nested animations (like mario-small → walk → frames)
            for (auto& [animName, frames] : value.items()) {
                for (size_t i = 0; i < frames.size(); ++i) {
                    auto& frame = frames[i];

                    SpriteData data;
                    data.x = frame["x"];
                    data.y = frame["y"];
                    data.z = frame["width"];
                    data.t = frame["height"];
                    data.texture = &texture;

                    // Name = "mario-small.walk[0]"
                    std::string name = key + "." + animName + "[" + std::to_string(i) + "]";
                    spriteMap[name] = data;
                }
            }
        }
    }
}

const SpriteData& TextureManager::getSpriteData(const std::string& name) const {
    auto it = spriteMap.find(name);
    if (it == spriteMap.end()) {
        throw std::out_of_range("Sprite not found: " + name);
    }
    return it->second;
}

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}
