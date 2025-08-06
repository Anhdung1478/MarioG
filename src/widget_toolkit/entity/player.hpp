#pragma once

#include<bits/stdc++.h>
#include "entity.hpp"
#include "player-list.hpp"
#include "box/dynamic-box.hpp"
#include "player_state/mario-state-manager.hpp"
#include "player_state/luigi-state-manager.hpp"
#include "../resource/LevelState.hpp"
#include "item/item.hpp"

namespace mario::entity {
    static constexpr sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;

            float shootingDelayTime = 0.f;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect

            int score = 0;
            int lives = 0;
            int coinCount = 0;

            bool isInvincible = false;
            float invincibleTimer = 0.f;

        public:
            Player(sf::Vector2f spawnPoint, CharacterListType characterType, player_state::PlayerStateType stateType);
            ~Player() override;
            
            void rotateDirection();
            void jump(bool isReleased);
            sf::Vector2f getVelocity() const;
            void setVelocity(sf::Vector2f vel);
            void shotFireball(bool isReleased);
            void move(bool isMoveRight, bool isReleased);
        
            void update(const sf::RenderWindow *window, float dt) override;
            void updateToLevelState(mario::resource::LevelState &levelState);
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;

            void setOnGround(bool isOnGround);
            void resetJump();

            /*void pickUpPowerUp(Item *item) {
                if(item == nullptr) { // item is fireflower
                    p_stateManager->changeToFireState(p_animation, p_body, p_body->getPosition());
                }

                if(item == nullptr) { // item is mushroom
                    p_stateManager->changeToSuperState(p_animation, p_body, p_body->getPosition());
                }
            }*/
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

            void collectStarman() {
                isInvincible = true;
                invincibleTimer = 10.f; // 10 seconds of invincibility
                // p_stateManager->changeToStarmanState(p_animation, p_body);
            }

            CharacterListType getCharacterType();
            player_state::PlayerStateType getPlayerStateType();
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;
    };
}