#include "FireWorks.hpp"

namespace mario::entity {

    FireWorks::FireWorks() : Entity()
    {
        // Default constructor
    }

    FireWorks::FireWorks(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name)
        : position(pos), size(size), Entity()
    {
       
        fireworksAnimation[0] = new Animation("../../asset/maps/Image/effects.png", BLOCK_SCALE, {
            {"fireworks[0]", 2, 76, 18, 18},
            {"fireworks[1]", 26, 71, 28, 28},
            {"fireworks[2]", 64, 69, 32, 32},
            {"fireworks[3]", 104, 69, 32, 32}
        });
        fireworksAnimation[0]->addAnimationStep("fireworks[0]");
        fireworksAnimation[0]->addAnimationStep("fireworks[1]");
        fireworksAnimation[0]->addAnimationStep("fireworks[2]");
        fireworksAnimation[0]->addAnimationStep("fireworks[3]");

        fireworksAnimation[1] = new Animation("../../asset/maps/Image/effects.png", BLOCK_SCALE, {
            {"fireworks[0]", 2, 110, 18, 18},
            {"fireworks[1]", 26, 105, 28, 28},
            {"fireworks[2]", 64, 103, 32, 32},
            {"fireworks[3]", 104, 103, 32, 32}
        });
        fireworksAnimation[1]->addAnimationStep("fireworks[0]");
        fireworksAnimation[1]->addAnimationStep("fireworks[1]");
        fireworksAnimation[1]->addAnimationStep("fireworks[2]");
        fireworksAnimation[1]->addAnimationStep("fireworks[3]");

        fireworksAnimation[2] = new Animation("../../asset/maps/Image/effects.png", BLOCK_SCALE, {
            {"fireworks[0]", 2, 143, 18, 18},
            {"fireworks[1]", 26, 138, 28, 28},
            {"fireworks[2]", 64, 136, 32, 32},
            {"fireworks[3]", 104, 136, 32, 32}
        });
        fireworksAnimation[2]->addAnimationStep("fireworks[0]");
        fireworksAnimation[2]->addAnimationStep("fireworks[1]");
        fireworksAnimation[2]->addAnimationStep("fireworks[2]");
        fireworksAnimation[2]->addAnimationStep("fireworks[3]");

        fireworksAnimation[0]->setTimeBetweenStep(1/5.0f);
        fireworksAnimation[1]->setTimeBetweenStep(1/5.0f);
        fireworksAnimation[2]->setTimeBetweenStep(1/5.0f);
        fireworksAnimation[0]->setAnimationState(true);
        fireworksAnimation[1]->setAnimationState(true);
        fireworksAnimation[2]->setAnimationState(true);
        createFireworksAnimation();
    }

    void FireWorks::createFireworksAnimation() {
        int numberOfFireworks = 15; // Randomly choose between 5 to 10 fireworks
        for (int i = 0; i < numberOfFireworks; ++i) {
            

            fireWorks.push_back(new Animation(*fireworksAnimation[rand() % 3]));
            fireWorks.back()->setPosition(sf::Vector2f(position.x + rand() % int(size.x), position.y + rand() % int(size.y)));
        }
    }

    void FireWorks::setShowFireworks(bool show) {
        showFireworks = show;
        if (!show) {
            for (auto& anim : fireWorks) {
                anim->setAnimationState(false);
            }
        }
    }

    FireWorks::~FireWorks() {
        for (auto& animation : fireworksAnimation) {
            delete animation;
        }
        fireworksAnimation[0] = nullptr;
        fireworksAnimation[1] = nullptr;
        fireworksAnimation[2] = nullptr;
        fireWorks.clear();
    }

    void FireWorks::update(const sf::RenderWindow *window, float dt) {
        // if (showFireworks) {
        //     animationTimer += dt;
        //     if (animationTimer >= animationDuration) {
        //         setShowFireworks(false);
        //         return;
        //     }

            for(auto& anim : fireWorks) {
                anim->update(window, dt);
            }
        // }
    }

    void FireWorks::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

    }

    void FireWorks::render(sf::RenderWindow *window) {
        for(auto& anim : fireWorks) {
            if (anim->getAnimationState()) {
                anim->render(window);
            }
        }
    }

} // namespace mario::entity