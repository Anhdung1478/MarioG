#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace mario::entity {

    class Box {
        private:
            constexpr static float PIXEL_PER_UNIT = 40.f;
        
        protected:
            b2BodyId bodyId = {0};
            b2ShapeId shapeId = {0};
            sf::Vector2f dimension;

            sf::Vector2f convertPixelToUnit(sf::Vector2f vec) { // Convert vector2D from pixels to Box2D units
                vec.x /= PIXEL_PER_UNIT;
                vec.y /= PIXEL_PER_UNIT;
                return vec;
            }

            sf::Vector2f convertUnitToPixel(sf::Vector2f vec) { // Convert vector2D from Box2D units to pixels
                vec.x *= PIXEL_PER_UNIT;
                vec.y *= PIXEL_PER_UNIT;
                return vec;
            }
    
        public:
            Box(b2WorldId worldId, sf::Vector2f position, sf::Vector2f _dimension, float density, float friction, bool isDynamicBody) {
                position = convertPixelToUnit(position);
                _dimension = convertPixelToUnit(_dimension);
                
                dimension = _dimension; // Store the dimensions of the dynamic box
                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.type = (isDynamicBody) ? b2_dynamicBody : b2_staticBody;
                bodyDef.position = b2Vec2({position.x, position.y});
                bodyId = b2CreateBody(worldId, &bodyDef);

                b2Polygon box = b2MakeBox(_dimension.x / 2.0f, _dimension.y / 2.0f); // Create a box shape with the given dimensions
                b2ShapeDef shapeDef = b2DefaultShapeDef(); // Create a default shape definition
                shapeDef.density = density;
                shapeDef.material.friction = friction;
                
                shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box); // Create a polygon shape for the ground body
            }

            virtual ~Box() {
                if(b2Body_IsValid(bodyId)) {
                    b2DestroyBody(bodyId);
                }
            }
            
            void enable() {
                b2Body_Enable(bodyId);
                b2Body_SetAwake(bodyId, true);
            }

            void disable() {
                b2Body_Disable(bodyId);
            }

            sf::Vector2f getPosition() {
                b2Vec2 pos = b2Body_GetPosition(bodyId);
                return convertUnitToPixel(sf::Vector2f(pos.x, pos.y));
            }

            sf::Vector2f getDimension() {
                return convertUnitToPixel(sf::Vector2f(dimension.x, dimension.y));
            }

            float getMass() {
                return b2Body_GetMass(bodyId);
            }

            virtual void applyLinearImpulseToCenter(sf::Vector2f force) = 0;
            virtual void applyForce(sf::Vector2f force) = 0;
            virtual sf::Vector2f getVelocity() = 0;
            virtual void setVelocity(sf::Vector2f vel) = 0;
            virtual void setDamping(float damp) = 0;
    };
}