#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include "box/dynamic-box.hpp"
#include "player_state/mario-state-manager.hpp"
#include "player_state/luigi-state-manager.hpp"
#include "item/item.hpp"

namespace mario::entity {
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

            float shootingDelayTime = 0.f;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect

            int score = 0;
            int lives = 2;
            int coinCount = 0;

        public:
            Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType) : _characterType(characterType) {
                p_body = new DynamicBox(spawnPoint, sf::Vector2f(40.f, 40.f));
                if(characterType == CharacterListType::Mario) {
                    p_animation = new Animation(FILE_PATH"mario.json", FILE_PATH"mario_sheets.png", PLAYER_SCALE, "mario-small.idle[0]");
                    p_animation->loadSheet(FILE_PATH"mario-fire.json", FILE_PATH"mario-fire.png");

                    p_stateManager = new mario::entity::player_state::MarioStateManager(p_animation, p_body, stateType);
                }

                if(characterType == CharacterListType::Luigi) {
                    p_animation = new Animation(FILE_PATH"luigi.json", FILE_PATH"luigi_sheets.png", PLAYER_SCALE, "luigi-small.idle[0]");
                    p_animation->loadSheet(FILE_PATH"luigi-fire.json", FILE_PATH"luigi-fire.png");

                    p_stateManager = new mario::entity::player_state::LuigiStateManager(p_animation, p_body, stateType);
                }
            }

            ~Player() override {
                delete p_stateManager;
            }

            void jump(bool isReleased) {
                p_body->jump(isReleased);
            }

            void rotateDirection() {
                p_animation->rotate();
            }
            
            void move(bool isMoveRight, bool isReleased) {
                p_body->move(isMoveRight, isReleased);
            }
            
            void shotFireball(bool isReleased) {
                if(isReleased)
                    return;
            }

            sf::Vector2f getVelocity() const {
                return p_body->getVelocity();
            }

            void setVelocity(sf::Vector2f vel) {
                p_body->setVelocity(vel);
            }
                
            void update(const sf::RenderWindow *window, float dt) override {
                if(!p_body->isOnGround()) {
                    // change texture to jumping
                    p_stateManager->setAnimation(p_animation, "jump[0]");
                    p_animation->setAnimationState(false);
                } else {
                    if(p_body->isNotMoving()) {
                        // change texture to idle
                        p_stateManager->setAnimation(p_animation, "idle[0]");
                        p_animation->setAnimationState(false);
                    } else 
                        if(p_animation->getAnimationState() == false) {
                            // change to run animation
                            p_stateManager->setAnimation(p_animation, "idle[0]");
                            p_animation->setAnimationState(true);
                        }
                        
                    hasPlayedJumpSound_ = false;
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
                p_body->updateSize(p_animation);
                p_body->update(dt);

                if(p_animation->isFaceForward() != p_body->isFaceForward())
                    rotateDirection();

                sf::Vector2f vel = p_body->getVelocity();
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

            void setOnGround(bool isOnGround) {
                p_body->setOnGround(isOnGround);
            }

            void resetJump() {
                p_body->resetJump();
            }

            CharacterListType getCharacterType() {
                return _characterType;
            }

            player_state::PlayerStateType getPlayerStateType() {
                return p_stateManager->getCurrentState();
            }

            bool hasPlayedJumpSound() const { 
                return hasPlayedJumpSound_; 
            }
            
            void setJumpSoundPlayed(bool played) { 
                hasPlayedJumpSound_ = played; 
            }

            void collectCoin() {
                coinCount++;
                score += 200;
                
                // 1-up at 100 coins
                if (coinCount >= 100) {
                    coinCount = 0;
                    lives++;
                    // Play 1-up sound
                }
                
                // Play coin sound
            }
            
            void collectRedMushroom() {
                if (getPlayerStateType() == player_state::PlayerStateType::Small) {
                    score += 1000;
                    p_stateManager->changeToSuperState(p_animation, p_body);
                    // Play power-up sound
                } else {
                    // Already super or fire, give points instead
                    score += 1000;
                }
            }
            
            void collectFireFlower() {
                if (getPlayerStateType() != player_state::PlayerStateType::Small) {
                    score += 1000;
                    p_stateManager->changeToFireState(p_animation, p_body);
                    // Play power-up sound
                } else {
                    // Small Mario gets super first then fire
                    p_stateManager->changeToSuperState(p_animation, p_body);
                    // Delay fire transformation or store for next frame?
                }
            }
            
            void collect1UpMushroom() {
                lives++;
                score += 1000;
                // Play 1-up sound
            }
    };

    #undef FILE_PATH
}