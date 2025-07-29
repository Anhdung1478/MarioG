#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include "box/dynamic-box.hpp"
#include "player_state/mario-state-manager.hpp"
#include "player_state/luigi-state-manager.hpp"
//#include "item/item.hpp"

namespace mario::entity {
    
    constexpr static float MOVE_ACCELERATION = 15.f;
    constexpr static float MAX_SPEED = 250.f; // pixel per second
    constexpr static float GROUND_DAMPING = 0.75f;
    constexpr static float AIR_DAMPING = 0.5f;

    constexpr static float JUMP_VELO = 300.f;
    constexpr static float START_JUMP_FORCE = 1.5f;
    constexpr static float MAX_JUMP_HEIGHT = 40.f * 4;

    #define FILE_PATH "../../asset/sprites/"

    const static sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);
    
    enum class CharacterListType {
        Mario = 0,
        Luigi = 1
    };

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;

            sf::Time jumpTimer;
            
            float startJumpPosY = -10000.f;
            float shootingDelayTime = 0.f;
            bool _isRunning = false;
            bool _isJumping = false;
            bool _isRunningForward;

        public:
            Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType) : _characterType(characterType) {
                p_body = new DynamicBox(spawnPoint, sf::Vector2f(40.f, 40.f));
                if(characterType == CharacterListType::Mario) {
                    p_animation = new Animation(FILE_PATH"mario.json", FILE_PATH"mario_sheets.png", PLAYER_SCALE, "mario-small.idle[0]");
                    p_stateManager = new mario::entity::player_state::MarioStateManager(p_animation, p_body, stateType);
                }

                if(characterType == CharacterListType::Luigi) {
                    p_animation = new Animation(FILE_PATH"luigi.json", FILE_PATH"luigi_sheets.png", PLAYER_SCALE, "luigi-small.idle[0]");
                    p_stateManager = new mario::entity::player_state::LuigiStateManager(p_animation, p_body, stateType);
                }
            }

            ~Player() override {
                delete p_stateManager;
            }

            bool isInSurface() {
                return (p_body->getPosition().y >= 600.f && p_body->getVelocity().y <= 0.1f && p_body->getVelocity().y >= -0.1f);
            }

            bool isFaceToBlock() {
                return false;
            }

            void jump(bool isReleased) {
                p_body->jump(isReleased);
            }

            void rotateDirection() {
                p_animation->rotate();
            }
            
            void move(bool isForward, bool isReleased) {
                p_body->move(isForward, isReleased);
            }
            
            void shotFireball(bool isReleased) {
                if(isReleased)
                    return;
            }

            void setVelocity(sf::Vector2f vel) {
                p_body->setVelocity(vel);
            }
                
            void update(const sf::RenderWindow *window, float dt) override {
                sf::Vector2f vel = p_body->getVelocity();

                if(!isInSurface()) {
                    // change texture to jumping
                    p_stateManager->setAnimation(p_animation, "jump[0]");
                    p_animation->setAnimationState(false);
                } else 
                    if(abs(vel.x) <= 0.001f) {
                        // change texture to idle
                        p_stateManager->setAnimation(p_animation, "idle[0]");
                        p_animation->setAnimationState(false);
                    } else 
                        if(p_animation->getAnimationState() == false) {
                            // change to run animation
                            p_stateManager->setAnimation(p_animation, "idle[0]");
                            p_animation->setAnimationState(true);   
                        }

                // change state for debugging
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
                    std::cerr << "CHANGE TO SMALL STATE\n";
                    p_stateManager->changeToSmallState(p_animation, p_body);
                    std::cerr << "SUCCESFULLY\n";
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
                    std::cerr << "CHANGE TO SUPER STATE\n";
                    p_stateManager->changeToSuperState(p_animation, p_body);
                    std::cerr << "SUCCESFULLY\n";
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
                    std::cerr << "CHANGE TO FIRE STATE\n";
                    p_stateManager->changeToFireState(p_animation, p_body);
                    std::cerr << "SUCCESFULLY\n";
                }
                
                p_animation->update(window, dt);
                p_body->update(dt);

                //std::cout << "PLAYER: POSITION: " << p_body->getPosition().x << ' ' << p_body->getPosition().y << " VELOCITY: " << vel.x << ' ' << vel.y << '\n';
            }
            
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }

            /*void pickUpPowerUp(Item *item) {
                if(item == nullptr) { // item is fireflower
                    p_stateManager->changeToFireState(p_animation, p_body, p_body->getPosition());
                }

                if(item == nullptr) { // item is mushroom
                    p_stateManager->changeToSuperState(p_animation, p_body, p_body->getPosition());
                }
            }*/
            
            void render(sf::RenderWindow *window) override {
                Entity::render(window);
            }

            CharacterListType getCharacterType() {
                return _characterType;
            }

            player_state::PlayerStateType getPlayerStateType() {
                return p_stateManager->getCurrentState();
            }

    };

    #undef FILE_PATH
}