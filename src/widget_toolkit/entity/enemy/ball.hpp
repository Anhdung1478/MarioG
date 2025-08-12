#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    class Ball : public Enemy {
    protected:
        enum class BallState {
            FreeFall,
            Walking
        };

        BallState currentState;
        BallState lastState;

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            p_animation->clearAnimationStep();

            try {
                if (currentState == BallState::FreeFall) {
                    p_animation->addAnimationStep("ball.free-fall[0]");
                    p_animation->addAnimationStep("ball.free-fall[1]");
                    p_animation->setSpriteAnimation("ball.free-fall[0]");
                } else {
                    p_animation->addAnimationStep("ball.walking[0]");
                    p_animation->addAnimationStep("ball.walking[1]");
                    p_animation->setSpriteAnimation("ball.walking[0]");
                }
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting ball animation - " << e.what() << "\n";
            }

            p_animation->setLoop(true);
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return ""; // ball không có trạng thái chết
        }

    public:
        Ball(sf::Vector2f startPosition)
            : Enemy(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f}, "ball.free-fall[0]",
                    startPosition, {32.f, 32.f}, "Patrol"), // "Patrol" chỉ để Enemy không chase
              currentState(BallState::FreeFall), lastState(BallState::FreeFall) {
            
            if (auto body = dynamic_cast<DynamicBox*>(p_body)) {
                body->setGravityEnabled(true);
                body->setAcceleration({800.f, 980.f}); // tốc độ rơi hợp lý
            }

            setIsCheckCollisionWithBlock(true);
            initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f});
        }

        void reactCollision(int side, const Collision& collision) override {
            if (collision.isWithWall()) {
                if (currentState == BallState::FreeFall) {
                    currentState = BallState::Walking;
                    initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.5f, 2.5f});
                    
                    if (auto body = dynamic_cast<DynamicBox*>(p_body)) {
                        body->setVelocity({-150.f, 0.f}); // move left
                    }
                }
            }
        }

        void updateBehavior(float dt, Player* player = nullptr) override {
            if (currentState == BallState::Walking) {
                if (auto body = dynamic_cast<DynamicBox*>(p_body)) {
                    body->move(false, false); // always move left
                }
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (shouldDelete()) return;

            updateBehavior(dt);
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
        }
    };

    #undef FILE_PATH
}
