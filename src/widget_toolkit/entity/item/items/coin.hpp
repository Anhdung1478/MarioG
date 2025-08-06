#pragma once
#include "../item.hpp"
#include "../../player.hpp"

namespace mario::entity {
    class Coin : public Item {
    private:
        float floatTimer = 0.0f;
    public:
        Coin(const std::string& jsonPath, const std::string& texturePath, 
             sf::Vector2f scale, const std::string& spriteID, 
             sf::Vector2f position, sf::Vector2f size)
            : Item(ItemType::Coin, jsonPath, texturePath, scale, spriteID, position, size, {0.f, 0.f}) {
                p_animation->addAnimationStep("coin[0]");
                p_animation->addAnimationStep("coin[1]");
                p_animation->addAnimationStep("coin[2]");
                p_animation->setAnimationState(true);
            }

        void onCollect(Entity* collector) override {
            if (Player* player = dynamic_cast<Player*>(collector)) {
                player->collectCoin(); // do we need to add scores?
                collect();
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
            //     floatTimer += dt * floatSpeed;
            //     float offsetY = std::sin(floatTimer) * floatAmptitude;

            //     sf::Vector2f currentPos = p_body->getPosition();
            //     p_body->setPosition({currentPos.x, currentPos.y + offsetY});
                if (p_animation) p_animation->update(window, dt);
            }
        }

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {
            // Coins typically don't need to handle events
            // This method is implemented to satisfy the IUpdateable interface
        }
    };
}
