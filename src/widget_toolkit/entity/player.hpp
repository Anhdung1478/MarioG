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

            void collectCoin();
            void collectRedMushroom();
            void collectFireFlower();
            void collect1UpMushroom();

            CharacterListType getCharacterType();
            player_state::PlayerStateType getPlayerStateType();
            
            void setJumpSoundPlayed(bool played);
            bool hasPlayedJumpSound() const;
    };
}