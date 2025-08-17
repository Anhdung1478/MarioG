#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "box.hpp"

namespace mario::entity {

    class StaticBox : public Box {
        public:
            StaticBox(sf::Vector2f _pos, sf::Vector2f _size) : Box(_pos, _size) {};

            ~StaticBox() override = default;

            void move(bool _isMoveRight, bool _isReleased) override {
            }

            void jump(bool _isReleased) override {
            }

            void update(float dt) override {
            }

            void resetJump() override {
            }

            void setAcceleration(sf::Vector2f acc) override {
            }

            void setVelocity(sf::Vector2f vel) override {
            }

            sf::Vector2f getVelocity() const override {
                return sf::Vector2f(0, 0);
            }

            bool isNotMoving() const override {
                return true;
            }

            void setOnGround(bool isOnGround) override {
            }

            bool isOnGround() const override {
                return true;
            }
            
            bool isFaceForward() const override {
                return true;
            }

            void setFaceForward(bool faceForward) override {
                
            }
    };
}