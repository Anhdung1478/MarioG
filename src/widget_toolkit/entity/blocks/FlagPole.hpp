#pragma once

#include "Block.hpp"
#include "BackgroundBlock.hpp"

namespace mario::entity {

    class FlagPole : public Entity {
        private:
            sf::Vector2f startPosition{0.0f, 10000.0f};
            sf::Vector2f finalPosition{0.0f, 0.0f};
            bool isWin = false;
            bool isClimbing = false;
            Animation* enemiesFlagAnimation;
            Animation* alliesFlagAnimation;
            std::vector<BackgroundBlock*> flagPoles;
        public:
            FlagPole();
            ~FlagPole();

            void addEnemiesFlag(sf::Vector2f pos);
            void addAlliesFlag(sf::Vector2f pos);
            void addFlagPole(sf::Vector2f pos, sf::Vector2f size, std::string name);
            int reactToCollision(Player* player = nullptr);
            bool getWinState();

            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };

} // namespace mario::entity
