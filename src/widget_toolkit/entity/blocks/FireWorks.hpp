#pragma once

#include "Block.hpp"

namespace mario::entity {

    class FireWorks : public Entity {
    private:
        sf::Vector2f position;
        sf::Vector2f size;
        std::vector<SpriteData2> sprites;
        Animation *fireworksAnimation[3];
        std::vector<Animation*> fireWorks;
        bool showFireworks = false;
        float animationTimer = 0.f;
        float animationDuration = 15.f;
    public:
        FireWorks();
        FireWorks(const sf::Vector2f &pos, const sf::Vector2f &size, const std::string &name);
        ~FireWorks();

        void createFireworksAnimation();
        void setShowFireworks(bool show);

        void update(const sf::RenderWindow *window, float dt) override;
        void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
        void render(sf::RenderWindow *window) override;
    };

} // namespace mario::entity
