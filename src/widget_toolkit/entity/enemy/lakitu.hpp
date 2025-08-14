#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "ball.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    class Lakitu : public Enemy {
    protected:
        enum class LakituState {
            Flying,
            Dead
        };

        LakituState currentState;
        LakituState lastState;

        static constexpr float HORIZONTAL_OFFSET = 100.f;
        static constexpr float DROP_INTERVAL = 3.f;
        static constexpr float DROP_THRESHOLD = 10.f;
        float dropCooldown = DROP_INTERVAL;

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            p_animation->clearAnimationStep();
            try {
                p_animation->addAnimationStep("lakitu.flying[0]");
                p_animation->setSpriteAnimation("lakitu.flying[0]");
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: lakitu.flying[0] - " << e.what() << "\n";
            }
            p_animation->setLoop(false);
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return "lakitu.dead[0]";
        }

    public:
        Lakitu(sf::Vector2f startPosition)
            : Enemy(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f}, "lakitu.flying[0]",
                    startPosition, {48.f, 48.f}, "Chase"),
              currentState(LakituState::Flying), lastState(LakituState::Flying) {
            
            if (auto body = dynamic_cast<DynamicBox*>(p_body)) {
                body->setGravityEnabled(false);
                body->setAcceleration({1100.f, 0.f});
                body->setVelocity({0.f, 0.f});
            }
            
            setIsCheckCollisionWithBlock(false); // Lakitu has no collision with block
            initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f});
        }

        void reactCollision(int side, const Collision& collision) override {
            
        }

        void updateBehavior(float dt, const Player* player, std::vector<Enemy*>& enemies) {
            if (!getActive() || !player) return;

            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                float targetX = player->getPosition().x + HORIZONTAL_OFFSET;
                float currentX = body->getPosition().x;
                float distanceToTarget = std::abs(targetX - currentX);

                bool moveRight = targetX > currentX;
                if (distanceToTarget > 1.f) {
                    body->move(moveRight, false);

                    if (p_animation->isFaceForward() != moveRight) {
                        p_animation->rotate();
                    }
                }

                // Logic để thả Ball
                dropCooldown -= dt;
                if (dropCooldown <= 0.f && currentState == LakituState::Flying) {
                    // Tạo Ball tại vị trí ngay dưới Lakitu
                    sf::Vector2f ballPosition = body->getPosition() + sf::Vector2f(0.f, 48.f); // Dưới Lakitu 48 pixel
                    enemies.push_back(new Ball(ballPosition));
                    dropCooldown = DROP_INTERVAL; // Reset cooldown
                }
            }
        }

        void updateWithPlayer(const sf::RenderWindow* window, float dt, const Player* player, std::vector<Enemy*>& enemies) {
            if (shouldDelete()) return;

            updateBehavior(dt, player, enemies);
            
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
        }

        void update(const sf::RenderWindow* window, float dt) override {
            // updateWithPlayer(window, dt, nullptr);
        }
    };

    #undef FILE_PATH
}
