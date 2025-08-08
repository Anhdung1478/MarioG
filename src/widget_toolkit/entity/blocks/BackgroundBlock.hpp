#pragma once

#include "Block.hpp"

namespace mario::entity {

class BackgroundBlock : public Block {
    private:
        std::vector<SpriteData2> sprites;
    public:
        BackgroundBlock();
        BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name);
        BackgroundBlock(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name, const SpriteData2& sprite);

        ~BackgroundBlock();

        void InitSpritesSheet() override;
        int reactToCollision(int side, Player* player = nullptr) override;

        void update(const sf::RenderWindow *window, float dt) override;
        void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
        void render(sf::RenderWindow *window) override;
    };

} // namespace mario::entity
