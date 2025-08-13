#pragma once

#include<bits/stdc++.h>
#include "../entity.hpp"
#include "../box/dynamic-box.hpp"

namespace mario::entity {
    class Fireball : public Entity {
        private:
            static constexpr sf::Vector2f FIREBALL_SIZE = sf::Vector2f(8.f, 8.f);
            static constexpr sf::Vector2f FIREBALL_EXPLOSION_SIZE = sf::Vector2f(16.f, 16.f);
            static constexpr sf::Vector2f FIREBALL_SCALE = sf::Vector2f(2.5f, 2.5f);
            static constexpr sf::Vector2f FIREBALL_GLOBAL_SIZE = sf::Vector2f(FIREBALL_SIZE.x * FIREBALL_SCALE.x, FIREBALL_SIZE.y * FIREBALL_SCALE.y);
            static constexpr sf::Vector2f FIREBALL_EXPLOSION_GLOBAL_SIZE = sf::Vector2f(FIREBALL_SCALE.x * FIREBALL_EXPLOSION_SIZE.x, FIREBALL_SCALE.y * FIREBALL_EXPLOSION_SIZE.y);

            bool _isExploding = false;
            bool _isDisappear = false;

        public:
            Fireball(sf::Vector2f spawnPoint, bool _isFaceForward) {
                p_animation = new Animation("../../asset/sprites/fire.json", "../../asset/sprites/effects.png", sf::Vector2f(2.5f, 2.5f), "fire.roll[0]");
                p_animation->loadSheet("../../asset/sprites/explosion.json", "../../asset/sprites/effects.png");
                
                p_body = new DynamicBox(spawnPoint, FIREBALL_GLOBAL_SIZE, 0.f, 400.f, -200.f, 1);
                p_body->setVelocity(sf::Vector2f((_isFaceForward ? 1 : -1) * 400.f, 0.f));
                loadRollAnimation();
            }

            void loadRollAnimation() {
                p_animation->clearAnimationStep();
                for (int i = 0; i < 4; ++i)
                    p_animation->addAnimationStep("fire.roll[" + std::to_string(i) + "]");
                
                p_animation->setAnimationState(true);
            }

            void loadExplodingAnimation() {
                p_animation->clearAnimationStep();
                for (int i = 0; i < 3; ++i)
                    p_animation->addAnimationStep("explosion.explode[" + std::to_string(i) + "]");

                p_animation->setSpriteAnimation("explosion.explode[0]");
                p_animation->setAnimationState(true);
                p_animation->setLoop(false);
            }

            void exploding() {
                if(!_isExploding) {
                    _isExploding = true;
                    loadExplodingAnimation();
                    setVelocity(sf::Vector2f(0.f, 0.f));
                    p_body->setAcceleration(sf::Vector2f(0.f, 0.f));

                    // re pos fireball explosion to middle
                    sf::Vector2f deltaMove = sf::Vector2f(0.f, 0.f);
                    sf::Vector2f fireballSize = getFireballSize();
                    sf::Vector2f fireballExplosionSize = getFireballExplosionSize();
                    //deltaMove.x = (-fireballSize.x + fireballExplosionSize.y) / 4.f;
                    deltaMove.y = (-fireballSize.y + fireballExplosionSize.y) / 2.f;

                    sf::Vector2f pos = p_body->getPosition();
                    pos.x += deltaMove.x, pos.y += deltaMove.y;
                    p_body->setPosition(pos);

                    pos = p_animation->getPosition();
                    pos.x += deltaMove.x, pos.y += deltaMove.y;
                    p_animation->setPosition(pos);
                }
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
                if(!_isExploding && p_body->isOnGround())
                    p_body->jump(false);
                
                p_animation->update(window, dt);
                //p_body->updateSize(p_animation);
                p_body->update(dt);

                if(_isExploding && p_animation->getAnimationState() == false || p_body->getPosition().y > 720.f)
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

            bool isExploding() const {
                return _isExploding;
            }

            bool isOnGround() const {
                return p_body->isOnGround();
            }

            static sf::Vector2f getFireballSize() {
                return FIREBALL_GLOBAL_SIZE;
            }

            static sf::Vector2f getFireballExplosionSize() {
                return FIREBALL_EXPLOSION_GLOBAL_SIZE;
            }
    };
}