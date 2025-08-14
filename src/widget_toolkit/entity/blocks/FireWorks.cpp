#include "FireWorks.hpp"

namespace mario::entity {

    FireWorks::FireWorks() : Entity()
    {
        // Default constructor
    }

    FireWorks::FireWorks(const sf::Vector2f &pos, const sf::Vector2f &size)
        : position(pos), size(size),Entity()
    {
       if(!texture.loadFromFile("../../asset/sprites/effects.png")) {
           std::cout << "Fail to load fireworks texture\n";
       }
       createFireworksAnimation();
    }

    void FireWorks::createFireworksAnimation() {
        int numberOfFireworks = 30;
        for (int i = 0; i < numberOfFireworks; ++i) {
            fireWorks.push_back(FireworksData(rand() % 3, position, size, texture));
        }
    }

    void FireWorks::setShowFireworks(bool show) {
        showFireworks = show;
    }

    FireWorks::~FireWorks() {
        
    }

    void FireWorks::update(const sf::RenderWindow *window, float dt) {
        if(showFireworks){
            
            for(auto& firework : fireWorks){
                firework.update(dt);
            }
        }
    }

    void FireWorks::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {

    }

    void FireWorks::render(sf::RenderWindow *window) {
        if(showFireworks) {
            for (const auto &firework : fireWorks) {
                if(firework.timeStart > 0.0f) continue;
                window->draw(firework.sprite);
            }
        }
    }

} // namespace mario::entity