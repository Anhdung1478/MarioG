#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "box.hpp"

namespace mario::entity {

    class StaticBox : public Box {
        public:
            StaticBox(b2WorldId worldId, sf::Vector2f position, sf::Vector2f _dimension, float density = 1.0f, float friction = 0.0f) : Box(worldId, position, _dimension, density, friction, 0) {}
    
            ~StaticBox() override {}

            void applyLinearImpulseToCenter(sf::Vector2f force) {
                abort();
            }

            void applyForce(sf::Vector2f force) {
                abort();
            }
            
            sf::Vector2f getVelocity() {
                abort();
            }

            void setVelocity(sf::Vector2f vel) {
                abort();
            }

            void setDamping(float damp) {
                abort();
            }
    };
}