#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    class Goomba : public Enemy {
    protected:
        enum class GoombaState {
            Walking,
            Dead
        };

        GoombaState currentState;
        GoombaState lastState;
        
        void loadWalkingAnimations() {
            p_animation->clearAnimationStep();
            for(int i = 0; i < 4; ++i) {
                std::string spriteID = "goomba-new.walking[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(true);
        }

        void loadDeadAnimations() {
            p_animation->clearAnimationStep();
            for(int i = 0; i < 3; ++i) {
                std::string spriteID = "goomba-new.dead[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(false);
        }

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath,
        sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            currentState = GoombaState::Walking;
            lastState = GoombaState::Walking;
            loadWalkingAnimations();
            try {
                p_animation->setSpriteAnimation("goomba-new.walking[0]");
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: goomba-new.walking[0] - " << e.what() << "\n";
            }
            p_animation->setAnimationState(true);
        }

        std::string getDeadSpriteID() const override {
            return "goomba-new.dead[0]";
        }

    public:
        Goomba(sf::Vector2f startPosition)
            : Enemy(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.3f, 2.3f}, "goomba-new.walking[0]", 
                startPosition, {44.f, 54.f}, "Patrol"),
                currentState(GoombaState::Walking), lastState(GoombaState::Walking) {
            initializeAnimations(FILE_PATH"enemy.json", FILE_PATH"enemy.png", {2.3f, 2.3f});
        }

        void hitByPlayer() {
            if(currentState == GoombaState::Walking) {
                currentState = GoombaState::Dead;
                p_animation->setAnimationState(true);
                loadDeadAnimations();
                try{
                    p_animation->setSpriteAnimation("goomba-new.dead[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: goomba-new.dead[0] - " << e.what() << "\n";
                }

                lastState = GoombaState::Walking;
            }
        }

        void reactCollision(int side, const Collision& collision) override {
            if(collision.isWithPlayer() && (side == SideCollision::Top)) {
                hitByPlayer();
            } else if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    if(side == SideCollision::Left) {
                        body->move(true, false); // Move right if hit from left
                        body->setIsFaceForward(true);
                    } else if(side == SideCollision::Right) {
                        body->move(false, false); // Move left if hit from right
                        body->setIsFaceForward(false);
                    }
                    if(p_animation->isFaceForward() != body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            } else if(collision.isWithBullet()) {
                currentState = GoombaState::Dead;
                loadDeadAnimations();
                try {
                    p_animation->setSpriteAnimation("goomba-new.dead[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: goomba-new.dead[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(true);
                lastState = GoombaState::Dead;
                setActive(true);
            } else if(collision.isWithBrick()) {

            } else {
                p_body->move(p_body->isFaceForward(), false); // continue
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (shouldBeDeleted) {
                return;
            }
            if(p_body->getPosition().y > 1000.f) {
                if(currentState != GoombaState::Dead) {
                    currentState = GoombaState::Dead;
                    p_animation->setAnimationState(false);
                    loadDeadAnimations();
                    try {
                        p_animation->setSpriteAnimation(getDeadSpriteID());
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getDeadSpriteID() << " - " << e.what() << "\n";
                    }
                    shouldBeDeleted = true;
                    lastState = GoombaState::Dead;
                }
            } else {
                if(currentState == GoombaState::Walking && lastState != GoombaState::Walking) {
                    loadWalkingAnimations();
                    try {
                        p_animation->setSpriteAnimation("goomba-new.walking[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: goomba-new.walking[0] - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = GoombaState::Walking;
                } else if (currentState == GoombaState::Dead && lastState != GoombaState::Dead) {
                    loadDeadAnimations();
                    try {
                        p_animation->setSpriteAnimation("goomba-new.dead[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: goomba-new.dead[0] - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = GoombaState::Dead;
                } else if (currentState == GoombaState::Dead && lastState == GoombaState::Dead) {
                    if (p_animation->getStep() >= 2) {
                        p_animation->setAnimationState(false);
                        shouldBeDeleted = true;
                    }
                }
            }
            if (!shouldBeDeleted) {
                updateBehavior(dt);
                p_animation->update(window, dt);
                p_body->update(dt);
                p_body->updateSize(p_animation);
            }
        }
    };

    #undef FILE_PATH
}