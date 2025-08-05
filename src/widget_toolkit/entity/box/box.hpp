#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../animation/animation.hpp"

namespace mario::entity {

    class Box {
        protected:
            sf::Vector2f position, size;
            
        public:
            Box(sf::Vector2f _pos, sf::Vector2f _size) : position(_pos), size(_size) {};

            virtual ~Box() = default;

            sf::Vector2f getPosition() const {
                return position;
            }

            sf::Vector2f getSize() const {
                return size;
            }

            sf::FloatRect getHitbox() const {
                sf::Vector2f left_most_pos(position.x - size.x / 2.f, position.y - size.y);
                return sf::FloatRect(left_most_pos, size);
            }

            void setPosition(sf::Vector2f _pos) {
                position = _pos;
            }

            void reSize(sf::Vector2f _size) {
                size = _size;
            }

            void updateSize(const mario::entity::Animation *p_animation) {
                reSize(p_animation->getSize());
            }

            virtual sf::Vector2f getVelocity() const = 0;
            virtual void setVelocity(sf::Vector2f vel) = 0;
            virtual void move(bool isMoveRight, bool isReleased) = 0;
            virtual void jump(bool isReleased) = 0;
            virtual void setOnGround(bool isOnGround) = 0;
            virtual void update(float dt) = 0;
            virtual bool isNotMoving() const = 0;
            virtual bool isFaceForward() const = 0;
            virtual bool isOnGround() const = 0;
            virtual void resetJump() = 0;

            void renderHitboxRect(sf::RenderWindow *window) {
                sf::FloatRect hitbox = getHitbox();
                sf::Vector2f _pos = hitbox.position;
                sf::Vector2f _size = hitbox.size;

                ++_pos.x, ++_pos.y;
                --_size.x, --_size.y;

                std::array line1 = {
                    sf::Vertex{_pos},
                    sf::Vertex{sf::Vector2f(_pos.x + _size.x, _pos.y)}
                };
                
                std::array line2 = {
                    sf::Vertex{sf::Vector2f(_pos.x + _size.x, _pos.y)},
                    sf::Vertex{sf::Vector2f(_pos.x + _size.x, _pos.y + _size.y)}
                };
                
                std::array line3 = {
                    sf::Vertex{sf::Vector2f(_pos.x + _size.x, _pos.y + _size.y)},
                    sf::Vertex{sf::Vector2f(_pos.x, _pos.y + _size.y)}
                };
                
                std::array line4 = {
                    sf::Vertex{sf::Vector2f(_pos.x, _pos.y + _size.y)},
                    sf::Vertex{_pos}
                };

                window->draw(line1.data(), line1.size(), sf::PrimitiveType::Lines);
                window->draw(line2.data(), line2.size(), sf::PrimitiveType::Lines);
                window->draw(line3.data(), line3.size(), sf::PrimitiveType::Lines);
                window->draw(line4.data(), line4.size(), sf::PrimitiveType::Lines);
            }
    };
}