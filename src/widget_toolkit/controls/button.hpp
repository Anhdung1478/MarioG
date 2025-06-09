#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <eventpp/callbacklist.h>
#include "../interfaces.hpp"

namespace mario {
    enum class ButtonState {
        Inactive = 0,
        Normal = 1,
        Hover = 2,
        Pressed = 3
    };

    class Button : public IScreenElement {
        public:
            std::unique_ptr<sf::Font> font;
            std::string buttonText;
            sf::FloatRect buttonRect;

            eventpp::CallbackList<void()> Click;
            ButtonState state;
            int fontSize;
            bool enabled;

            Button() : fontSize(20), enabled(true), buttonText("") {
                font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");
            }
            
            void update(const sf::RenderWindow *window, float dt) override {
                handleMouseInput(window);
            }

            void handleMouseInput(const sf::RenderWindow *window) {
                if(!enabled) {
                    state = ButtonState::Inactive;
                    return;
                }
                
                state = ButtonState::Normal;
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                if(buttonRect.contains(sf::Vector2f(mousePos))) {
                    state = ButtonState::Hover;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        state = ButtonState::Pressed;
                        Click(); // Trigger click event
                    }
                }
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
            }
                
            void render(sf::RenderWindow *window) override {
                if(enabled) {
                    sf::RectangleShape rect(sf::Vector2f(buttonRect.size.x, buttonRect.size.y));
                    rect.setPosition(buttonRect.position);
                    rect.setFillColor((state == ButtonState::Pressed) ? sf::Color::Green : 
                        ((state == ButtonState::Hover) ? sf::Color::Yellow : sf::Color::Magenta));
                    
                    window->draw(rect);

                    sf::Text text(*font, buttonText, fontSize);
                    float textLenX = text.getGlobalBounds().size.x;
                    float textLenY = text.getGlobalBounds().size.y;

                    text.setFillColor(sf::Color::White);
                    text.setPosition(sf::Vector2f(int(buttonRect.position.x + (buttonRect.size.x - textLenX) / 2.0), int(buttonRect.position.y + (buttonRect.size.y - fontSize * 4 / 3.0) / 2.0)));
                    window->draw(text); 
                }
            }
        };
}