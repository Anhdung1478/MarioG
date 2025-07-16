#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "luigi-state.hpp"

namespace mario::entity::player_state {
    class LuigiStateManager : public PlayerStateManager {
        private:
            const std::string LUIGI_TYPE_STR[3] = {
                "luigi-small",
                "luigi-super",
                "luigi-fire",
            };

        public:
            LuigiStateManager(b2WorldId worldId, sf::Vector2f pos, Animation *&p_animation, Box *&p_box) {

                player_small = new LuigiSmallState(worldId);
                player_super = new LuigiSuperState(worldId);
                player_fire = new LuigiFireState(worldId);
                curr_state = player_small;
                curr_state->update(p_animation, p_box, pos);
            }

            ~LuigiStateManager() override {
            }

            void setAnimation(Animation *&p_animation, const std::string &ID) override {
                p_animation->setSpriteAnimation(LUIGI_TYPE_STR[(int) currStateType] + "." + ID);
            }
    };
}