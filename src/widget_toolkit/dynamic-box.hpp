#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace mario {
    class DynamicBox {
        private:
            b2BodyId bodyId;
            b2ShapeId shapeId;
            sf::Vector2f dimension;
    
        public:
            DynamicBox(b2WorldId worldId, sf::Vector2f position, sf::Vector2f _dimension) {
                position.x /= 40.f; // Convert position from pixels to Box2D units (1 unit = 40 pixels)
                position.y /= 40.f;
                _dimension.x /= 40.f;
                _dimension.y /= 40.f;
                
                dimension = _dimension; // Store the dimensions of the dynamic box
                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.type = b2_dynamicBody; // Set the ground body as static
                bodyDef.position = b2Vec2({position.x, position.y}); // Set the position of the ground body
                bodyId = b2CreateBody(worldId, &bodyDef); // Create the ground body

                b2Polygon box = b2MakeBox(_dimension.x / 2.0f, _dimension.y / 2.0f); // Create a box shape with the given dimensions
                
                b2ShapeDef shapeDef = b2DefaultShapeDef(); // Create a default shape definition
                shapeDef.density = 1.0f; // Set density for the ground shape
                shapeDef.material.friction = 0.3f; // Set friction for the ground shape
                
                shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box); // Create a polygon shape for the ground body
            }

            void applyForce(b2Vec2 force) {
                force.x /= 40.f; // Convert force from pixels to Box2D units (1 unit = 40 pixels)
                force.y /= 40.f;
                b2Body_ApplyForceToCenter(bodyId, force, true);
            }

            void applyLinearImpulse(b2Vec2 force) {
                force.x /= 40.f; // Convert force from pixels to Box2D units (1 unit = 40 pixels)
                force.y /= 40.f;
                b2Body_ApplyLinearImpulseToCenter(bodyId, force, true);
            }

            sf::Vector2f getPosition() {
                b2Vec2 pos = b2Body_GetPosition(bodyId);
                return sf::Vector2f(pos.x * 40.f, pos.y * 40.f);
            }

            sf::Vector2f getDimension() {
                return sf::Vector2f(dimension.x * 40.f, dimension.y * 40.f);
            }

            sf::Vector2f getVeclocity() {
                b2Vec2 vel = b2Body_GetLinearVelocity(bodyId);
                return sf::Vector2f(vel.x * 40.f, vel.y * 40.f);
            }
    };
}