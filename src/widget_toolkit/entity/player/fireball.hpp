#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"

namespace mario::entity {
    class Fireball : public Entity {
        private:

        public:
            Fireball(sf::Vector2f spawnPoint, sf::Vector2f _size, bool _isFaceForward) {
                //p_animation = new Animation();
                p_body = new DynamicBox(spawnPoint, _size);
            }

            void update(const sf::RenderWindow *window, float dt) override {

            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {

            }            
    };
}