#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "../entity/player/player.hpp"

namespace mario::input { // Command pattern
    class Command {
        public:
            virtual ~Command() = default;
            virtual void execute(mario::entity::Player *player, bool isReleased) = 0; // isReleased = 1 -> key is released, isReleased = 0 -> key is pressed
    };

    class JumpCommand : public Command {
        public: 
            JumpCommand() {}
            void execute(mario::entity::Player *player, bool isReleased) override {
                player->jump(isReleased, true);
            }
    };

    class MoveLeftCommand : public Command {
        public: 
            MoveLeftCommand() {}

            void execute(mario::entity::Player *player, bool isReleased) override {
                player->moveLeft(isReleased, true);
            }
    };

    class MoveRightCommand : public Command {
        public: 
            MoveRightCommand() {}

            void execute(mario::entity::Player *player, bool isReleased) override {
                player->moveRight(isReleased, true);
            }
    };
    
    class FireCommand : public Command {
        public: 
            void execute(mario::entity::Player *player, bool isReleased) override {
                player->shotFireball(isReleased, true);
            }
    };
}