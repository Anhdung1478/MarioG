#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "asset/sprites/"

    enum class KoopaType {
        Red,
        Green
    };

    class Koopa : public Enemy {
    protected:
        enum class KoopaState {
            Running,
            Jumping,
            TransitionToShell,
            Shell,
            DeadSpecial
        };

        KoopaType koopaType;
        std::string typePrefix;
        KoopaState currentState;
        KoopaState lastState;
        bool canJump;
        float stateTimer;
        bool checkShell = false;
        float hitCooldown = 0.f;
        float shellCollisionTimer = 0.f;
        float verticalVelocity = 0.f;
        const float jumpForce = -450.f;
        const float gravity = 600.f;
        static constexpr float HIT_COOLDOWN_DURATION = 1.0f;
        static constexpr float TRANSITION_DURATION = 0.35f;
        static constexpr float SHELL_COLLISION_DELAY = 0.1f;

        void loadRunningAnimations() {
            p_animation->clearAnimationStep();
            for (int i = 0; i < 11; ++i) {
                std::string spriteID = typePrefix + "running[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
        }

        void loadJumpingAnimations() {
            p_animation->clearAnimationStep();
            for (int i = 0; i < 10; ++i) { 
                std::string spriteID = typePrefix + "jumping[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
        }

        void loadTransitionToShellAnimations() {
            p_animation->clearAnimationStep();
            for (int i = 0; i < 4; ++i) {
                std::string spriteID = typePrefix + "jumping-back-into-shell[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
            p_animation->setLoop(false);
        }

        void loadShellAnimations() {
            p_animation->clearAnimationStep();
            for (int i = 0; i < 1; ++i) {
                std::string spriteID = typePrefix + "knocked-out[" + std::to_string(i) + "]";
                try {
                    p_animation->addAnimationStep(spriteID);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
                }
            }
        }

        void loadDeadSpecialAnimations() {
            p_animation->clearAnimationStep();
            std::string spriteID = typePrefix + "dead-special[" + std::to_string(0) + "]";
            try {
                p_animation->addAnimationStep(spriteID);
            } catch (const std::out_of_range& e) {
                std::cerr << "Error loading sprite: " << spriteID << " - " << e.what() << "\n";
            }
        }

        void initializeAnimations(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale) override {
            p_animation->loadSheet(jsonPath, texturePath);
            if(canJump) {
                currentState = KoopaState::Jumping;
                lastState = KoopaState::Jumping;
                loadJumpingAnimations();
                p_animation->setSpriteAnimation(typePrefix + "jumping[0]");
                p_animation->setAnimationState(true);
                p_body->jump(false);
            } else {
                loadRunningAnimations();
                p_animation->setSpriteAnimation(typePrefix + "running[0]");
                p_animation->setAnimationState(true);
            }
        }

        std::string getDeadSpriteID() const override {
            return typePrefix + "knocked-out[0]";
        }

    public:
        Koopa(const std::string& jsonPath, const std::string& texturePath, sf::Vector2f scale,
              const std::string& spriteID, sf::Vector2f startPosition, sf::Vector2f size,
              const std::string& behaviorStr, bool canJump = false)
            : typePrefix(spriteID.substr(0, spriteID.find("running[0]"))),
              koopaType(spriteID.find("red") != std::string::npos ? KoopaType::Red : KoopaType::Green),
              currentState(KoopaState::Running), lastState(KoopaState::Running), canJump(canJump),
              stateTimer(0.f),
              Enemy(jsonPath, texturePath, scale, spriteID, startPosition, size, behaviorStr) {
            try {
                p_animation->setSpriteAnimation(spriteID);
            } catch (const std::out_of_range& e) {
                std::cerr << "Error setting sprite: " << spriteID << " - " << e.what() << "\n";
            }
            initializeAnimations(jsonPath, texturePath, scale);
        }

        virtual ~Koopa() = default;

        void hitByPlayer() {
            if (hitCooldown > 0.f) return;

            if (currentState == KoopaState::Jumping) {
                currentState = KoopaState::Running;
                p_animation->setAnimationState(true);
                loadRunningAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "running[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << typePrefix + "running[0]" << " - " << e.what() << "\n";
                }
                lastState = KoopaState::Jumping;
                canJump = !canJump;
                hitCooldown = HIT_COOLDOWN_DURATION;
            } else if (currentState == KoopaState::Running) {
                currentState = KoopaState::TransitionToShell;
                stateTimer = TRANSITION_DURATION;
                p_animation->setAnimationState(true);
                loadTransitionToShellAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "jumping-back-into-shell[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: " << typePrefix + "jumping-back-into-shell[0]" << " - " << e.what() << "\n";
                }
                lastState = KoopaState::Running;
                hitCooldown = HIT_COOLDOWN_DURATION;
            }
        }

        void reactCollision(int side, const Collision& collision) override {
            if(collision.isWithPlayer() && (side == SideCollision::Top) && currentState != KoopaState::Shell) {
                hitByPlayer();
            } else if(collision.isWithPlayer() && currentState == KoopaState::Shell && lastState == KoopaState::Shell && !checkShell) {
                // _context->getSoundManager().playSound(mario::event::SoundEvent::SHELL_KICK);
                
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    checkShell = true;
                    float pushSpeed = 500.f;
                    body->setVelocity({side == SideCollision::Right ? -pushSpeed : pushSpeed, body->getVelocity().y});
                    body->setIsFaceForward(side == SideCollision::Left);
                    shellCollisionTimer = SHELL_COLLISION_DELAY;
                }
            } else if(collision.isWithEnemy()){
                currentState = KoopaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: goomba-new.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = KoopaState::DeadSpecial;
                setActive(true);
                setIsCheckCollisionWithEnemy(false);
            } else if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    if(side == SideCollision::Left) {
                        // Move right if hit from left
                        body->moveLeft(true);
                        body->moveRight(false);
                        // body->move(true, false); 
                        body->setIsFaceForward(true);
                    }
                    else if(side == SideCollision::Right) {
                        // Move left if hit from right
                        body->moveRight(true);
                        body->moveLeft(false);
                        // body->move(false, false); 
                        body->setIsFaceForward(false);
                        // std::cout << "COLLISION: " << body->getVelocity().x << std::endl;
                    }

                    if(p_animation->isFaceForward() != body->isFaceForward())
                        p_animation->rotate();
                }
            } else if(collision.isWithFireball()) {
                currentState = KoopaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: goomba-new.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = KoopaState::DeadSpecial;
                setActive(false);
                setIsCheckCollisionWithEnemy(false);
                setIsCheckCollisionWithPlayer(false);
            } else if(collision.isWithInvinciblePlayer()) {
                currentState = KoopaState::DeadSpecial;
                loadDeadSpecialAnimations();
                try {
                    p_animation->setSpriteAnimation(typePrefix + "dead-special[0]");
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error setting sprite: goomba-new.dead-special[0] - " << e.what() << "\n";
                }
                p_animation->setAnimationState(false);
                verticalVelocity = jumpForce;
                lastState = KoopaState::DeadSpecial;
                setActive(false);
                setIsCheckCollisionWithEnemy(false);
                setIsCheckCollisionWithPlayer(false);
            } else if(p_body->getPosition().x >= initialPosition.x - patrolRange && p_body->getPosition().x <= initialPosition.x + patrolRange){
                // continue
                if(p_body->isFaceForward()) {
                    p_body->moveLeft(true);
                    p_body->moveRight(false);
                } else {
                    p_body->moveRight(true);
                    p_body->moveLeft(false);
                }
                // p_body->move(p_body->isFaceForward(), false); 
            } else if (!(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) && currentState == KoopaState::Shell && lastState == KoopaState::Shell && checkShell) {
                float pushSpeed = 500.f;
                p_body->setVelocity({p_body->getVelocity().x < 0 ? -pushSpeed : pushSpeed, p_body->getVelocity().y});
                //std::cout << "After: " << p_body->getVelocity().x << std::endl;
                // std::cout << "Before: " << p_body->getVelocity().x << std::endl;
                //std::cout << "UPDATE: " << p_body->getVelocity().x << std::endl;
                
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (hitCooldown > 0.f) {
                hitCooldown -= dt;
                if (hitCooldown < 0.f) hitCooldown = 0.f;
            }

            if (shellCollisionTimer > 0.f) {
                shellCollisionTimer -= dt;
                if (shellCollisionTimer <= 0.f) {
                    setIsPlayerDeadWhenCollisionLF(true);
                    setIsPlayerDeadWhenCollisionT(true);
                    shellCollisionTimer = 0.f;
                }
            }

            if (p_body->getPosition().y > 1000.f) {
                if (currentState != KoopaState::Shell) {
                    currentState = KoopaState::Shell;
                    p_animation->setAnimationState(false);
                    loadShellAnimations();
                    try {
                        p_animation->setSpriteAnimation(getDeadSpriteID());
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << getDeadSpriteID() << " - " << e.what() << "\n";
                    }
                    setActive(false);
                    lastState = KoopaState::Shell;
                }
            } else {
                if (currentState == KoopaState::Running && lastState != KoopaState::Running) {
                    loadRunningAnimations();
                    try {
                        p_animation->setSpriteAnimation(typePrefix + "running[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << typePrefix + "running[0]" << " - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = KoopaState::Running;
                } else if (currentState == KoopaState::Jumping && lastState != KoopaState::Jumping) {
                    loadJumpingAnimations();
                    try {
                        p_animation->setSpriteAnimation(typePrefix + "jumping[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << typePrefix + "jumping[0]" << " - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = KoopaState::Jumping;
                } else if (currentState == KoopaState::TransitionToShell && lastState != KoopaState::TransitionToShell) {
                    loadTransitionToShellAnimations(); // Use jumping animations for transition
                    try {
                        p_animation->setSpriteAnimation(typePrefix + "jumping-back-into-shell[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << typePrefix + "jumping-back-into-shell[0]" << " - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = KoopaState::TransitionToShell;
                } else if (currentState == KoopaState::Shell && lastState != KoopaState::Shell) {
                    loadShellAnimations();
                    try {
                        p_animation->setSpriteAnimation(typePrefix + "knocked-out[0]");
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Error setting sprite: " << typePrefix + "knocked-out[0]" << " - " << e.what() << "\n";
                    }
                    p_animation->setAnimationState(true);
                    lastState = KoopaState::Shell;
                }

                if (currentState == KoopaState::Running && canJump) {
                    DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                    if (body && body->isOnGround()) {
                        currentState = KoopaState::Jumping;
                        loadJumpingAnimations();
                        try {
                            p_animation->setSpriteAnimation(typePrefix + "jumping[0]");
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Error setting sprite: " << typePrefix + "jumping[0]" << " - " << e.what() << "\n";
                        }
                        p_animation->setAnimationState(true);
                        p_body->jump(false);
                        lastState = KoopaState::Jumping;
                    }
                } else if (currentState == KoopaState::Jumping && canJump) {
                    DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                    if (body && body->isOnGround()) {
                        currentState = KoopaState::Jumping; // Trigger next jump immediately
                        loadJumpingAnimations();
                        try {
                            p_animation->setSpriteAnimation(typePrefix + "jumping[0]");
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Error setting sprite: " << typePrefix + "jumping[0]" << " - " << e.what() << "\n";
                        }
                        p_animation->setAnimationState(true);
                        p_body->jump(false);
                        lastState = KoopaState::Jumping;
                    }
                } else if (currentState == KoopaState::TransitionToShell && lastState == KoopaState::TransitionToShell) {
                    stateTimer -= dt;
                    if (stateTimer <= 0.f) {
                        currentState = KoopaState::Shell;
                        p_body->setAcceleration({0.f, p_body->getAcceleration().y});
                        p_body->setMaxVelocityX(600.f);
                        loadShellAnimations();
                        try {
                            p_animation->setSpriteAnimation(typePrefix + "knocked-out[0]");
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Error setting sprite: " << typePrefix + "knocked-out[0]" << " - " << e.what() << "\n";
                        }
                        p_animation->setAnimationState(true);
                        lastState = KoopaState::TransitionToShell;
                    }
                } else if (currentState == KoopaState::Shell && lastState == KoopaState::Shell && !checkShell) {
                    setIsPlayerDeadWhenCollisionLF(false);
                    p_body->setVelocity({0.f, p_body->getVelocity().y});
                }
                if (currentState == KoopaState::DeadSpecial) {
                    verticalVelocity += gravity * dt;
                    p_body->setPosition(sf::Vector2f(p_body->getPosition().x, p_body->getPosition().y + verticalVelocity * dt));
                    setIsCheckCollisionWithBlock(false);
                } 
            }

            if(currentState == KoopaState::Shell && checkShell) {
                float pushSpeed = 500.f;
                p_body->setVelocity({pushSpeed * (p_body->isFaceForward() > 0.f ? +1 : -1), p_body->getVelocity().y});
            }
 
            updateBehavior(dt);
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
        }

        bool checkShellState() {
            return currentState == KoopaState::Shell && checkShell;
        }
    };

    class KoopaChase : public Koopa {
    public:
        KoopaChase(sf::Vector2f startPosition, KoopaType type, bool canJump = false)
            : Koopa(FILE_PATH"koopa-troopa.json", FILE_PATH"koopa_troopa_sheets.png", {1.8f, 1.8f},
                    (type == KoopaType::Red ? "koopa-troopa-red.running[0]" : "koopa-troopa-green.running[0]"),
                    startPosition, {50.f, 72.f}, "Chase", canJump) {
            koopaType = type;
        }
    };

    class KoopaPatrol : public Koopa {
    public:
        KoopaPatrol(sf::Vector2f startPosition, KoopaType type, bool canJump = false)
            : Koopa(FILE_PATH"koopa-troopa.json", FILE_PATH"koopa_troopa_sheets.png", {1.8f, 1.8f},
                    (type == KoopaType::Red ? "koopa-troopa-red.running[0]" : "koopa-troopa-green.running[0]"),
                    startPosition, {50.f, 72.f}, "Patrol", canJump) {
            koopaType = type;
        }
    };

    #undef FILE_PATH
}