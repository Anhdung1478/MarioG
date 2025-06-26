#pragma once

#include<bits/stdc++.h>
#include "../interfaces.hpp"

namespace mario::entity {
    class Entity : public IScreenElement {
        public:
            void update(const sf::RenderWindow *window, float dt) override {
                
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                
            }
    };
}