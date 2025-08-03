#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

    class Goomba : public Enemy {
    protected:
        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            for (int i = 0; i < 8; ++i) {
                p_animation->addAnimationStep("goomba.walking[" + std::to_string(i) + "]");
            }
        }

        std::string getDeadSpriteID() const override {
            return "goomba.hurt[0]";
        }

    public:
        Goomba(sf::Vector2f startPosition)
            : Enemy(FILE_PATH"goomba.json", FILE_PATH"goomba_sheets.png", {2.f, 2.f}, "goomba.walking[0]", startPosition, {44.f, 54.f}, "Patrol") {
            initializeAnimations(FILE_PATH"goomba.json", FILE_PATH"goomba_sheets.png", {2.f, 2.f});
        }
    };

    #undef FILE_PATH
}