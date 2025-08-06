#pragma once
#include "../item.hpp"
#include "../../player/player.hpp"

namespace mario::entity {
    class RedMushroom : public Item {
    private:
        bool hasEmerged = false;
        float emergenceHeight = 16.0f;
        sf::Vector2f emergenceStartPos;

        void handleEmergence(float dt) {
            // Move upward from block until fully emerged
            sf::Vector2f currentPos = p_body->getPosition();
            if (currentPos.y > emergenceStartPos.y - emergenceHeight) {
                p_body->setVelocity({0.0f, -60.0f}); // Move up
            } else {
                hasEmerged = true;
                p_body->setVelocity({30.0f, 0.0f}); // Start walking right
            }
        }
        
        void handleWalkingMovement(float dt) {
            // Mushroom continues moving until it hits something
            sf::Vector2f velocity = p_body->getVelocity();
            
            // Apply gravity
            velocity.y += 980.0f * dt; // Gravity constant
            
            // Maintain horizontal movement
            if (std::abs(velocity.x) < 5.0f) {
                velocity.x = (velocity.x >= 0) ? 30.0f : -30.0f;
            }
            
            p_body->setVelocity(velocity);
        }

    public:
        RedMushroom(const std::string& jsonPath, const std::string& texturePath, 
                    sf::Vector2f scale, const std::string& spriteID, 
                    sf::Vector2f position, sf::Vector2f size, sf::Vector2f velocity)
            : Item(ItemType::RedMushroom, jsonPath, texturePath, scale, spriteID, position, size, velocity) {
                movementType = MovementType::Walking;
                emergenceStartPos = position;
                p_body->move(true, false);
            }

        void onCollect(Entity* collector) override {
            if (Player* player = dynamic_cast<Player*>(collector)) {
                player->collectRedMushroom();               
                collect();
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (!isCollected()) {
                // if (!hasEmerged) {
                //     handleEmergence(dt);
                // } else {
                //     handleWalkingMovement(dt);
                // }
                
                // if (p_body) p_body->update(dt);
            //     if (p_animation) p_animation->update(window, dt);
            }
        }

        void handleEvent(const sf::RenderWindow* window, const sf::Event& event) override {
            // Red mushrooms typically don't need to handle events
            // This method is implemented to satisfy the IUpdateable interface
        }
    };
}
