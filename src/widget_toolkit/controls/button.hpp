#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <eventpp/callbacklist.h>
#include "../interfaces.hpp"
#include "../sfml-extra-feature/rounded-rectangle-shape.hpp"

namespace mario {
    enum class ButtonState {
        Inactive = 0,
        Normal = 1,
        Hover = 2,
        Pressed = 3
    };

    struct ButtonListNode;

    class Button : public IScreenElement {
        private:
            std::unique_ptr<sf::Font> font;
            ButtonState state;

        public:
            std::string buttonText;
            sf::FloatRect buttonRect;
            ButtonListNode *p_nodeOnButton;

            eventpp::CallbackList<void()> Click;
            int fontSize;
            bool enabled, selected;

            // New Constructor: add by vql
            Button(sf::Font& fontRef, const std::string& buttText)
                : fontSize(20), enabled(true), selected(false), buttonText(buttText) {
                font = std::make_unique<sf::Font>(fontRef);
                p_nodeOnButton = nullptr;
            }

            Button(std::string buttText = "") : fontSize(20), enabled(true), selected(false), buttonText(buttText) {
                font = std::make_unique<sf::Font>("../../asset/fonts/SuperMario256.ttf");
                p_nodeOnButton = nullptr;
            }

            Button(const Button &button) {
                buttonText = button.buttonText;
                buttonRect = button.buttonRect;
                Click = button.Click;
                state = button.state;
                fontSize = button.fontSize;
                enabled = button.enabled;
                selected = button.selected;
            }
            
            void update(const sf::RenderWindow *window, float dt) override {
                handleMouseInput(window);
            }

            virtual void handleMouseInput(const sf::RenderWindow *window) {
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

            void drawRoundedRectangle(sf::RenderWindow *window, sf::FloatRect rect, sf::Color buttonColor) {
                // Define properties for the rounded rectangle
                float cornerRadius = 7.5f;

                sf::RoundedRectangleShape roundedRect(rect.size, cornerRadius, 4);
                roundedRect.setPosition(rect.position);
                roundedRect.setFillColor(buttonColor);
                
                window->draw(roundedRect);
            }
                
            void render(sf::RenderWindow *window) override {
                if(!enabled) {

                    return;
                }

                sf::Color buttonColor = (state == ButtonState::Pressed) ? sf::Color::Green : 
                    ((state == ButtonState::Hover) ? sf::Color::Yellow : sf::Color(49, 139, 23, 255));

                drawRoundedRectangle(window, buttonRect, buttonColor);

                if(selected) {
                    sf::CircleShape triangle(5);
                    triangle.setPosition(sf::Vector2f(buttonRect.position.x - 30, buttonRect.position.y));
                    triangle.setFillColor(sf::Color::White);
                    window->draw(triangle);
                }

                sf::Text text(*font, buttonText, fontSize);
                float textLenX = text.getGlobalBounds().size.x;
                float textLenY = text.getGlobalBounds().size.y;

                text.setFillColor(sf::Color(239, 139, 54, 255));
                text.setPosition(sf::Vector2f(int(buttonRect.position.x + (buttonRect.size.x - textLenX) / 2.f), int(buttonRect.position.y - 4 + (buttonRect.size.y - textLenY) / 2.f)));
                window->draw(text); 
            }
    };
}