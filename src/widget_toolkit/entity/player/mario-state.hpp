#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "player-state.hpp"
#include "../animation/animation.hpp"
#include "../box/dynamic-box.hpp"
#include "../box/box.hpp"

namespace mario::entity::player_state {
    const sf::Vector2f MARIO_SCALE = sf::Vector2f(2.5f, 2.5f);
    


    class MarioState : public PlayerState {
        protected:
            const std::string MARIO_TYPE;
    
        public:
            MarioState(std::string marioType) : MARIO_TYPE(marioType) {}
    };
    


    class MarioSmallState : public MarioState {
        private:
            const sf::Vector2f MARIO_DIM = sf::Vector2f(17, 19);

        public:
            MarioSmallState(const std::string &state_name) : MarioState(state_name) {}

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(MARIO_DIM.x * MARIO_SCALE.x, MARIO_DIM.y * MARIO_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".idle[0]");
            }
    };



    class MarioSuperState : public MarioState {
        private:
            const sf::Vector2f MARIO_DIM = sf::Vector2f(20, 31);

        public:
            MarioSuperState(const std::string &state_name) : MarioState(state_name) {}

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(MARIO_DIM.x * MARIO_SCALE.x, MARIO_DIM.y * MARIO_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".idle[0]");
            }
    };

    

    class MarioFireState : public MarioState {
        private:
            const sf::Vector2f MARIO_DIM = sf::Vector2f(20, 31);

        public:
            MarioFireState(const std::string &state_name) : MarioState(state_name) {}

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(MARIO_DIM.x * MARIO_SCALE.x, MARIO_DIM.y * MARIO_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".idle[0]");
            }
    };


    
    class MarioStarState : public MarioState {
        private:
            const sf::Vector2f MARIO_DIM = sf::Vector2f(20, 31);

        public:
            MarioStarState(const std::string &state_name) : MarioState(state_name) {}

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(MARIO_DIM.x * MARIO_SCALE.x, MARIO_DIM.y * MARIO_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".idle[0]");
            }
    };
    
    #undef MARIO_FILE_PATH
}