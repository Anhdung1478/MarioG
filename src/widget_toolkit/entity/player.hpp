#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include <box2d/box2d.h>
#include "box/dynamic-box.hpp"
#include "player_state/mario-state-manager.hpp"
#include "player_state/luigi-state-manager.hpp"

namespace mario::entity {
    
    constexpr static float MOVE_ACCELERATION = 15.f;
    constexpr static float MAX_SPEED = 200.f; // pixel per second
    constexpr static float GROUND_DAMPING = 3.f;
    constexpr static float AIR_DAMPING = 0.5f;

    constexpr static float JUMP_ACCELERATION = 50.f;
    constexpr static float MAX_JUMP_TIME = 0.2f;

    #define FILE_PATH "../../asset/sprites/"

    const static sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);
    
    enum class CharacterListType {
        Mario = 0,
        Luigi = 1
    };

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;

            sf::Time jumpTimer;
            
            float shootingDelayTime = 0.f;
            bool _isRunning = false;
            bool _isJumping = false;
            bool _isRunningForward;

        public:
            Player(b2WorldId worldId, sf::Vector2f spawnPoint, CharacterListType characterType) {
                if(characterType == CharacterListType::Mario) {
                    p_animation = new Animation(FILE_PATH"mario.json", FILE_PATH"mario_sheets.png", PLAYER_SCALE, "mario-small.idle[0]");
                    p_stateManager = new mario::entity::player_state::MarioStateManager(worldId, spawnPoint, p_animation, p_body);
                }

                if(characterType == CharacterListType::Luigi) {
                    p_animation = new Animation(FILE_PATH"luigi.json", FILE_PATH"luigi_sheets.png", PLAYER_SCALE, "luigi-small.idle[0]");
                    p_stateManager = new mario::entity::player_state::LuigiStateManager(worldId, spawnPoint, p_animation, p_body);
                }
            }

            ~Player() override {
                delete p_stateManager;
            }

            bool isInSurface() {
                return (p_body->getPosition().y + p_body->getDimension().y >= 714.f && p_body->getVelocity().y <= 0.1f && p_body->getVelocity().y >= -0.1f);
            }

            bool isFaceToBlock() {
                return false;
            }

            void jump(bool isReleased) {
                if(isReleased) {
                    _isJumping = false;
                    return;
                }

                if(!_isJumping && isInSurface()) {
                    _isJumping = true;
                    jumpTimer = sf::Time::Zero;
                }
            }

            void rotateDirection() {
                p_animation->rotate();
            }
            
            void run(bool isForward, bool isReleased) {
                if(isReleased) {
                    _isRunning = false;
                    return;
                }

                if(!_isRunning && !isFaceToBlock()) {
                    _isRunning = true;
                    _isRunningForward = isForward;
                }
            }
            
            void shotFireball(bool isReleased) {
                if(isReleased)
                    return;
            }
                
            void update(const sf::RenderWindow *window, float dt) override {
                sf::Vector2f vel = p_body->getVelocity();
                if(!isInSurface()) {
                    // change texture to jumping
                    p_stateManager->setAnimation(p_animation, "jump[0]");
                    p_animation->setAnimationState(false);
                } else {
                    if(abs(vel.x) <= 0.001f) {
                        // change texture to idle
                        p_stateManager->setAnimation(p_animation, "idle[0]");
                        p_animation->setAnimationState(false);
                    } else {
                        // change to next run animation step
                        p_animation->setAnimationState(true);
                    }
                }

                // change state for debugging
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
                    std::cerr << "CHANGE TO SMALL STATE\n";
                    p_stateManager->changeToSmallState(p_animation, p_body, p_body->getPosition());
                    std::cerr << "SUCCESFULLY\n";
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
                    std::cerr << "CHANGE TO SUPER STATE\n";
                    p_stateManager->changeToSuperState(p_animation, p_body, p_body->getPosition());
                    std::cerr << "SUCCESFULLY\n";
                }
                
                p_animation->update(window, dt);
                float bodyMass = p_body->getMass();

                if(_isJumping) {
                    p_body->applyForce(sf::Vector2f(0.f, -JUMP_ACCELERATION * bodyMass));
                    jumpTimer += sf::seconds(dt);
                }

                if(_isJumping && jumpTimer >= sf::seconds(MAX_JUMP_TIME))
                    _isJumping = false;

                if(_isRunning && isFaceToBlock()) 
                    _isRunning = false;
                
                if(_isRunning) {
                    if(p_animation->isFaceForward() != _isRunningForward && (vel.x == 0.f || (p_animation->isFaceForward() != (vel.x > 0.f))))
                        rotateDirection();

                    if(_isRunningForward && vel.x < MAX_SPEED || !_isRunningForward && vel.x > -MAX_SPEED) {
                        sf::Vector2f force = sf::Vector2f((_isRunningForward ? 1 : -1) * MOVE_ACCELERATION * bodyMass, 0.f);
                        p_body->applyForce(force);
                    }
                }

                p_body->setDamping(isInSurface() ? GROUND_DAMPING : AIR_DAMPING);
                //std::cerr << "PLAYER VELOCITY: " << vel.x << ' ' << vel.y << '\n';
                std::cout << "PLAYER POSITION: " << p_body->getPosition().x << ' ' << p_body->getPosition().y << '\n';
            }
            
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }
            
            void render(sf::RenderWindow *window) override {
                p_animation->renderWithPosition(window, p_body->getPosition());
            }

    };

    #undef FILE_PATH
}