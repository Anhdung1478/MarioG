#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "player-state.hpp"
#include "../animation/animation.hpp"
#include "../box/dynamic-box.hpp"
#include "../box/box.hpp"

namespace mario::entity::player_state {
    #define MARIO_FILE_PATH "../../asset/sprites/"
    const sf::Vector2f MARIO_SCALE = sf::Vector2f(2.5f, 2.5f);
    
    constexpr static float MARIO_DENSITY = 1.f;
    constexpr static float MARIO_FRICTION = 0.8f;
    
    class MarioState : public PlayerState {
        protected:
            const std::string MARIO_TYPE;
            b2WorldId _worldId;
    
        public:
            MarioState(b2WorldId worldId, std::string marioType) : _worldId(worldId), MARIO_TYPE(marioType) {
            }
    };
    

    class MarioSmallState : public MarioState {
        private:
            const sf::Vector2f MARIO_DIM = sf::Vector2f(17, 19);

        public:
            MarioSmallState(b2WorldId worldId) : MarioState(worldId, "mario-small") {
            }

            void update(Animation *&p_eAnimation, Box *&p_eBox, sf::Vector2f pos) override {
                if(p_eBox != nullptr) 
                    delete p_eBox; 
                
                p_eBox = new DynamicBox(_worldId, pos, MARIO_DIM, MARIO_DENSITY, MARIO_FRICTION);
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
            MarioSuperState(b2WorldId worldId) : MarioState(worldId, "mario-super") {
            }

            void update(Animation *&p_eAnimation, Box *&p_eBox, sf::Vector2f pos) override {
                if(p_eBox != nullptr) 
                    delete p_eBox; 
                
                p_eBox = new DynamicBox(_worldId, pos, MARIO_DIM, MARIO_DENSITY, MARIO_FRICTION);
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
            MarioFireState(b2WorldId worldId) : MarioState(worldId, "mario-fire") {
            }

            void update(Animation *&p_eAnimation, Box *&p_eBox, sf::Vector2f pos) override {
                if(p_eBox != nullptr) 
                    delete p_eBox; 
                
                p_eBox = new DynamicBox(_worldId, pos, MARIO_DIM, MARIO_DENSITY, MARIO_FRICTION);
                p_eAnimation->clearAnimationStep();
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[0]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".walk[1]");
                p_eAnimation->addAnimationStep(MARIO_TYPE + ".idle[0]");
            }
    };

    #undef MARIO_FILE_PATH
}