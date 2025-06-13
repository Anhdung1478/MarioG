#include<bits/stdc++.h>
#include "Entity.hpp"

namespace mario {
    class Player : public Entity {
        private:
            std::unique_ptr<sf::Sprite> p_sprite;
            std::unique_ptr<sf::Texture> p_texture;

        public:
            Player(std::string fileName) {
                p_texture = std::make_unique<sf::Texture>(fileName);
                p_sprite = std::make_unique<sf::Sprite>(p_sprite);
            }

            void update(const sf::RenderWindow *window, float dt) override {
                
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                
            }

            void render(sf::RenderWindow *window) override {
                window->draw(*p_sprite);
            }

    };
}