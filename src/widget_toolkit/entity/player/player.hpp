#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "player-list.hpp"
#include "../box/dynamic-box.hpp"
#include "mario-state-manager.hpp"
#include "luigi-state-manager.hpp"
#include "../../resource/LevelState.hpp"
#include "../item/item.hpp"

namespace mario::entity {
    static constexpr sf::Vector2f PLAYER_SCALE = sf::Vector2f(2.5f, 2.5f);

    class Player : public Entity {
        private:
            mario::entity::player_state::PlayerStateManager *p_stateManager;
            CharacterListType _characterType;

            sf::Time deadAnimationTime = sf::seconds(3);
            sf::Time shootingDelayTime;
            bool _isOnGround;

            bool hasPlayedJumpSound_ = false; // For sound effect
            bool _isAlive = true, _isFinishedDeadAnimation = false;

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

            void setStartedDead(); // set Player status when started to dead
            void managePlayerAnimation(); // manage Player's animation when they did smth
            void managePlayerDeadState(float dt); // manage Player when being dead
            void beingHit(); // being hit by enemy or entity like level trap
            bool isDead() const;
            bool isInDeadAnimation() const;

            void collectCoin();            
            void collectRedMushroom();            
            void collectFireFlower();
            void collect1UpMushroom();
            void collectStarman();

            CharacterListType getCharacterType();
            player_state::PlayerStateType getPlayerStateType();
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;
    };
}