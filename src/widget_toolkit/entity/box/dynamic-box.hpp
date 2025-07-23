#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "box.hpp"

namespace mario::entity {

    class DynamicBox : public Box {
        public:
            DynamicBox(b2WorldId worldId, sf::Vector2f position, sf::Vector2f _dimension, float density = 1.0f, float friction = 0.0f) : Box(worldId, position, _dimension, density, friction, 1) {
                b2Body_SetGravityScale(bodyId, 1.f);
            }

            ~DynamicBox() override {}

            void applyLinearImpulseToCenter(sf::Vector2f force) {
                b2Vec2 forceVec = {force.x, force.y};
                b2Body_ApplyLinearImpulseToCenter(bodyId, forceVec, true);
            }

            void applyForce(sf::Vector2f force) {
                b2Vec2 forceVec = {force.x, force.y}; 
                b2Body_ApplyForceToCenter(bodyId, forceVec, true);
            }

            sf::Vector2f getVelocity() {
                b2Vec2 vel = b2Body_GetLinearVelocity(bodyId);
                return convertUnitToPixel(sf::Vector2f(vel.x, vel.y));
            }
            
            void setVelocity(sf::Vector2f vel) { 
                vel = convertPixelToUnit(vel);
                b2Vec2 b2Vel = b2Vec2({vel.x, vel.y});
                b2Body_SetLinearVelocity(bodyId, b2Vel);
            }

            void setDamping(float damp) {
                b2Body_SetLinearDamping(bodyId, damp);
            }
    };
}