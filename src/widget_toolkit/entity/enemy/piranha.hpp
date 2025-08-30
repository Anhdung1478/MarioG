#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "asset/sprites/"

    enum class PiranhaType {
        Green,
        Red
    };

    class Piranha : public Enemy {
    protected:
        enum class PiranhaState {
            Idle,
            MovingUp,
            MovingDown
        };

        PiranhaType piranhaType;
        std::string typePrefix;
        PiranhaState currentState;
        PiranhaState lastState;
        float moveTimer;     // Timer for movement or idle state
        float moveDuration;  // Time for one movement (up or down)
        float idleDuration;  // Time to stay idle at top
        float maxHeight;     // Maximum height the Piranha rises
        sf::Vector2f pipePosition; // Position of the pipe (bottom position)
        float detectionRange; // Range to detect player proximity
        bool isInTop = true;

        void loadEatAnimations() {
            p_animation->clearAnimationStep();
            for (int i = 0; i < 6; ++i) {
                std::string spriteID = typePrefix + "eat[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(true);
        }

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            currentState = PiranhaState::Idle;
            lastState = PiranhaState::Idle;
            loadEatAnimations();
            try {
                p_animation->setSpriteAnimation(typePrefix + "eat[0]");
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: " << typePrefix + "eat[0]" << " - " << e.what() << "\n";
            }
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return typePrefix + "eat[0]";
        }

        void piranhaMove(float dt, const Player* player) {
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (!body) return;

            bool playerNearby = false;
            float currentDistance = std::numeric_limits<float>::max();
            if (player) {
                currentDistance = std::abs(player->getPosition().x - body->getPosition().x);
                playerNearby = currentDistance <= detectionRange;
            }

            if (playerNearby) {
                if (currentState == PiranhaState::MovingUp) {
                    currentState = PiranhaState::MovingDown;
                    moveTimer = moveDuration * (1.0f - moveTimer / moveDuration);
                } else if (body->getPosition().y >= pipePosition.y) {
                    currentState = PiranhaState::Idle;
                    moveTimer = 0.f;
                } else if (currentState == PiranhaState::MovingDown) {
                    moveTimer -= dt;
                    if(moveTimer <= 0.f) {
                        moveTimer = moveTimer;
                    }
                }
            } else {
                if (currentState == PiranhaState::Idle && body->getPosition().y >= pipePosition.y) {
                    // at bot, move up
                    currentState = PiranhaState::MovingUp;
                    moveTimer = moveDuration;
                } else if (currentState == PiranhaState::Idle && body->getPosition().y < pipePosition.y) {
                    // At top, move down
                    moveTimer -= dt;
                    if (moveTimer <= 0.f) {
                        currentState = PiranhaState::MovingDown;
                        moveTimer = moveDuration;
                        isInTop = false;
                    }
                } else if (currentState == PiranhaState::MovingUp || currentState == PiranhaState::MovingDown) {
                    moveTimer -= dt;
                    if (moveTimer <= 0.f) {
                        if(currentState == PiranhaState::MovingUp) isInTop = true;
                        currentState = (currentState == PiranhaState::MovingUp) ? PiranhaState::Idle : PiranhaState::MovingUp;
                        moveTimer = (currentState == PiranhaState::Idle) ? idleDuration : moveDuration;
                    }
                }
            }

            if (currentState == PiranhaState::Idle) {
                body->setVelocity({0.f, 0.f});
                if (isInTop && !playerNearby) {
                    // At top
                    body->setPosition({body->getPosition().x, pipePosition.y - maxHeight});
                } else if (isInTop && playerNearby) {
                    currentState = PiranhaState::MovingDown;
                    moveTimer = moveDuration;
                    isInTop = false;
                } else if (!isInTop) {
                    // At bottom
                    body->setPosition({body->getPosition().x, pipePosition.y});
                }
            } else if (currentState == PiranhaState::MovingUp || currentState == PiranhaState::MovingDown) {
                float progress = moveTimer / moveDuration;
                float targetY;
                if (currentState == PiranhaState::MovingUp) {
                    // Trans from pos to maxHeight
                    targetY = pipePosition.y - maxHeight * (1.0f - progress);
                } else if (currentState == PiranhaState::MovingDown){
                    // Trans from maxHeight to pos
                    targetY = pipePosition.y - maxHeight * progress;
                }
                body->setPosition({body->getPosition().x, targetY});
                body->setVelocity({0.f, 0.f});
            }
        }

    public:
        Piranha(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale,
                const std::string& spriteID, sf::Vector2f startPosition, sf::Vector2f size)
            : typePrefix(spriteID.substr(0, spriteID.find("eat[0]"))),
              piranhaType(spriteID.find("red") != std::string::npos ? PiranhaType::Red : PiranhaType::Green),
              currentState(PiranhaState::Idle), lastState(PiranhaState::Idle),
              moveTimer(1.0f), moveDuration(1.0f), idleDuration(1.0f),
              maxHeight(70.0f), pipePosition(sf::Vector2f(startPosition.x, startPosition.y + 10.f)), detectionRange(120.f),
              Enemy(jsonPath, texturePath, scale, spriteID, startPosition, size, "Piranha") {
            try {
                p_animation->setSpriteAnimation(spriteID);
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: " << spriteID << " - " << e.what() << "\n";
            }
            initializeAnimations(jsonPath, texturePath, scale);
            DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            if (body) {
                body->setOnGround(true); // Prevent falling
                body->setVelocity({0.f, 0.f}); // Initialize velocity to zero
                body->setPosition({startPosition.x, startPosition.y - maxHeight}); // Start at top
            }
            setIsPlayerDeadWhenCollisionT(true);
        }

        virtual ~Piranha() = default;

        void hitByPlayer() {
            // Player dead only
        }

        void reactCollision(int side, const Collision& collision) override {
            if(collision.isWithInvinciblePlayer()) {
                setIsCheckCollisionWithEnemy(false);
                setIsCheckCollisionWithPlayer(false);
                setShouldDelete(true);
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            updateWithPlayer(window, dt, nullptr);
        }

        void updateWithPlayer(const sf::RenderWindow* window, float dt, const Player* player) {
            if (currentState != lastState) {
                loadEatAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "eat[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << typePrefix + "eat[0]" << " - " << e.what() << "\n";
                }
                p_animation->setAnimationState(true);
                lastState = currentState;
            }

            piranhaMove(dt, player);
            
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
        }
    };

    class PiranhaGreen : public Piranha {
    public:
        PiranhaGreen(sf::Vector2f startPosition)
            : Piranha(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.4f, 2.4f},
                      "piranha-green.eat[0]", startPosition, {18.f, 26.f}) {
            piranhaType = PiranhaType::Green;
        }
    };

    class PiranhaRed : public Piranha {
    public:
        PiranhaRed(sf::Vector2f startPosition)
            : Piranha(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.4f, 2.4f},
                      "piranha-red.eat[0]", startPosition, {18.f, 26.f}) {
            piranhaType = PiranhaType::Red;
        }
    };

    #undef FILE_PATH
}