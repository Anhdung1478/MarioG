#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"

namespace mario::entity {
    class Fireball : public Entity {
        private:

        public:
            Fireball(sf::Vector2f spawnPoint, sf::Vector2f _size) {
                p_animation = new Animation();
                p_body = new DynamicBox(spawnPoint, _size);
            }
    };
}