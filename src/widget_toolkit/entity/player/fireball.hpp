#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"

/*
    "../../asset/sprites/fireflower.json",
    "../../asset/maps/Image/tiles-8.png",
    sf::Vector2f(2.5f, 2.5f),
    "fireflower[0]",
    sf::Vector2f(block->getPosition().x, block->getPosition().y - block->getSize().y/2.0f),
    sf::Vector2f(16.f, 16.f),
    sf::Vector2f(0.f, 0.f)
*/
namespace mario::entity {
    class Fireball : public Entity {
        private:
            bool _isExploding = false;
            bool _isDisappear = false;

        public:
            Fireball(sf::Vector2f spawnPoint, sf::Vector2f _size, bool _isFaceForward) {
                p_animation = new Animation("../../asset/sprites/fireflower.json", "../../asset/maps/Image/tiles-8.png", sf::Vector2f(2.5f, 2.5f), "fireflower[0]");
                p_body = new DynamicBox(spawnPoint, _size, 0.f, 500.f, -30.f, 1);
            }

            void exploding() {
                _isExploding = true;
                setVelocity(sf::Vector2f(0.f, 0.f));
            }

            void setOnGround(bool isOnGround) {
                p_body->setOnGround(isOnGround);
            }

            void setVelocity(sf::Vector2f vel) {
                p_body->setVelocity(vel);
            }

            sf::Vector2f getVelocity() const {
                return p_body->getVelocity();
            }

            void update(const sf::RenderWindow *window, float dt) override {
                if(p_body->isOnGround())
                    p_body->jump(false);

                p_animation->update(window, dt);
                p_body->update(dt);

                if(_isExploding && p_animation->getAnimationState() == false)
                    _isDisappear = true;
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                Entity::render(window);
            }

            bool isDisappeared() const {
                return _isDisappear;
            }
    };
}