#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"

namespace mario::entity {
    #define FILE_PATH "../../asset/sprites/"

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
            Shell
        };

        KoopaType koopaType;
        std::string typePrefix;
        KoopaState currentState;
        KoopaState lastState;
        bool canJump;
        float stateTimer;
        bool checkShell = false;
        float hitCooldown = 0.f;
        static constexpr float HIT_COOLDOWN_DURATION = 1.0f;
        static constexpr float TRANSITION_DURATION = 0.35f;

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
            if(collision.isWithPlayer() && (side == SideCollision::Top)) {
                hitByPlayer();
            // } else if(collision.isWithPlayer() && (side == SideCollision::Right || side == SideCollision::Left)) {
            //     DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
            //     if(body) {
            //         checkShell = true;
            //         float pushSpeed = 2000.f;
            //         body->setVelocity({body->isFaceForward() ? pushSpeed : -pushSpeed, body->getVelocity().y});
            //     }
            } else if(collision.isWithWall() && (side == SideCollision::Left || side == SideCollision::Right)) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    if(side == SideCollision::Left) {
                        body->move(true, false); // Move right if hit from left
                        body->setIsFaceForward(true);
                        std::cout << "move right" << std::endl;
                    }
                    else if(side == SideCollision::Right) {
                        body->move(false, false); // Move left if hit from right
                        body->setIsFaceForward(false);
                        std::cout << "move left" << std::endl;
                    }
                    if(p_animation->isFaceForward() != body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            } else if(collision.isWithBullet()) {
                currentState = KoopaState::Shell;
                loadShellAnimations();
                p_animation->setSpriteAnimation(typePrefix + "knocked-out[0]");
                p_animation->setAnimationState(true);
                lastState = KoopaState::Shell;
                setActive(false);
            } else if(collision.isWithBrick()) {
                DynamicBox* body = dynamic_cast<DynamicBox*>(p_body);
                if(body) {
                    body->setVelocity({-body->getVelocity().x, body->getVelocity().y});
                    if(p_animation->isFaceForward() == body->isFaceForward()) {
                        p_animation->rotate();
                    }
                }
            } else if(p_body->getPosition().x >= initialPosition.x - patrolRange && p_body->getPosition().x <= initialPosition.x + patrolRange) {
                p_body->move(p_body->isFaceForward(), false); // continue
            }
        }

        void update(const sf::RenderWindow* window, float dt) override {
            if (hitCooldown > 0.f) {
                hitCooldown -= dt;
                if (hitCooldown < 0.f) hitCooldown = 0.f;
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
                    p_body->setVelocity({0.f, p_body->getVelocity().y});
                }
            }
            updateBehavior(dt);
            p_animation->update(window, dt);
            p_body->updateSize(p_animation);
            p_body->update(dt);
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