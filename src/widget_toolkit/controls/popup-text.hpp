#pragma once

#include <bits/stdc++.h>
#include "../interfaces.hpp"

namespace mario {
    class PopUpText : public IScreenElement {
        private:
            std::unique_ptr<sf::Font> p_font;
            std::unique_ptr<sf::Text> p_text;
            
            sf::Vector2f velocity;
            sf::Time appearTimer;
            bool _isDisappear = false;
        
        public:
            PopUpText(sf::Vector2f spawnPoint, std::string text, float appearTime, int fontSize, sf::Vector2f vel, std::string fontPath, sf::Color textColor) 
                : velocity(vel), appearTimer(sf::seconds(appearTime)) {
                    _isDisappear = false;
                    p_font = std::make_unique<sf::Font>(fontPath);
                    p_text = std::make_unique<sf::Text>(*p_font, text, fontSize);
                    p_text->setPosition(spawnPoint);
                    p_text->setFillColor(textColor);
                }

            ~PopUpText() override {};

            void update(const sf::RenderWindow *window, float dt) override {
                appearTimer -= sf::seconds(dt);
                if(appearTimer <= sf::seconds(0))
                    _isDisappear = true;

                sf::Vector2f deltaPos(velocity.x * dt, velocity.y * dt);
                p_text->move(deltaPos);
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                if(!_isDisappear)
                    window->draw(*p_text);
            }

            bool isDisappeared() const {
                return _isDisappear;
            }
    };
}