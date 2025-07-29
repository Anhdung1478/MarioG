#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "player-state.hpp"
#include "../animation/animation.hpp"
#include "../box/dynamic-box.hpp"
#include "../box/box.hpp"

namespace mario::entity::player_state {
    #define LUIGI_FILE_PATH "../../asset/sprites/"
    const sf::Vector2f LUIGI_SCALE = sf::Vector2f(2.5f, 2.5f);
    
    constexpr static float LUIGI_DENSITY = 1.f;
    constexpr static float LUIGI_FRICTION = 0.8f;
    
    class LuigiState : public PlayerState {
        protected:
            const std::string LUIGI_TYPE;
    
        public:
            LuigiState(std::string marioType) : LUIGI_TYPE(marioType) {
            }
    };
    

    class LuigiSmallState : public LuigiState {
        private:
            const sf::Vector2f LUIGI_DIM = sf::Vector2f(17, 19);

        public:
            LuigiSmallState() : LuigiState("luigi-small") {
            }

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(LUIGI_DIM.x * LUIGI_SCALE.x, LUIGI_DIM.y * LUIGI_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".idle[0]");
            }
    };


    class LuigiSuperState : public LuigiState {
        private:
            const sf::Vector2f LUIGI_DIM = sf::Vector2f(20, 31);

        public:
            LuigiSuperState() : LuigiState("luigi-super") {
            }

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(LUIGI_DIM.x * LUIGI_SCALE.x, LUIGI_DIM.y * LUIGI_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".idle[0]");
            }
    };

    
    class LuigiFireState : public LuigiState {
        private:
            const sf::Vector2f LUIGI_DIM = sf::Vector2f(20, 31);

        public:
            LuigiFireState() : LuigiState("luigi-fire") {
            }

            void update(Animation *p_eAnimation, Box *p_eBox) override {
                sf::Vector2f GLOBAL_DIM = sf::Vector2f(LUIGI_DIM.x * LUIGI_SCALE.x, LUIGI_DIM.y * LUIGI_SCALE.y);
                p_eBox->reSize(GLOBAL_DIM);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(LUIGI_TYPE + ".idle[0]");
            }
    };

    #undef LUIGI_FILE_PATH
}