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
            static constexpr float CHOOSING_MUSHROOM_INVISIBLE_TIME = 1.f / 8.f;
            static constexpr float CHOOSING_MUSHROOM_VISIBLE_TIME = 1.f / 4.f;
            std::unique_ptr<sf::Font> font;
            ButtonState state;

        public:
            static sf::Texture *p_texture;
            sf::Sprite *p_leftMushroomSprite, *p_rightMushroomSprite;

            std::string buttonText;
            sf::FloatRect buttonRect;
            ButtonListNode *p_nodeOnButton;

            eventpp::CallbackList<void()> Click;
            sf::Time mushroomFlickerTimer;
            
            int fontSize;
            bool enabled = true, selected = false;
            bool isChoosingMushroomAppearing = true;

            // New Constructor: add by vql
            Button(sf::Font& fontRef, const std::string& buttText)
                : fontSize(20), enabled(true), selected(false), buttonText(buttText) {
                    font = std::make_unique<sf::Font>(fontRef);
                    p_nodeOnButton = nullptr;
                }

            Button(std::string buttText = "") : fontSize(12), enabled(true), selected(false), buttonText(buttText) {
                font = std::make_unique<sf::Font>("asset/fonts/MarioPixelRegularFont.ttf");
                p_leftMushroomSprite = new sf::Sprite(*p_texture);
                p_rightMushroomSprite = new sf::Sprite(*p_texture);

                p_leftMushroomSprite->setOrigin(sf::Vector2f(p_leftMushroomSprite->getLocalBounds().size.x / 2.f, p_leftMushroomSprite->getLocalBounds().size.y / 2.f));
                p_leftMushroomSprite->setScale(sf::Vector2f(0.05f, 0.05f));
                p_leftMushroomSprite->setRotation(sf::degrees(-90));

                p_rightMushroomSprite->setOrigin(sf::Vector2f(p_rightMushroomSprite->getLocalBounds().size.x / 2.f, p_rightMushroomSprite->getLocalBounds().size.y / 2.f));
                p_rightMushroomSprite->setScale(sf::Vector2f(0.05f, 0.05f));
                p_rightMushroomSprite->setRotation(sf::degrees(-90));

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

            ~Button() {
                delete p_leftMushroomSprite;
                delete p_rightMushroomSprite;
            }
            
            void update(const sf::RenderWindow *window, float dt) override {
                handleMouseInput(window);
            
                mushroomFlickerTimer -= sf::seconds(dt);
                if(mushroomFlickerTimer <= sf::Time::Zero) {
                    mushroomFlickerTimer = (isChoosingMushroomAppearing) ? sf::seconds(CHOOSING_MUSHROOM_INVISIBLE_TIME) : sf::seconds(CHOOSING_MUSHROOM_VISIBLE_TIME);
                    isChoosingMushroomAppearing = 1 - isChoosingMushroomAppearing;
                }
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
                sf::Color buttonColor;
                if(!enabled) {
                    buttonColor = sf::Color(128, 128, 128, 128);
                } else {
                    buttonColor = (state == ButtonState::Pressed) ? sf::Color::Green : 
                        ((state == ButtonState::Hover) ? sf::Color(173, 255, 205, 255) : sf::Color(49, 139, 23, 255));
                }

                drawRoundedRectangle(window, buttonRect, buttonColor);
                if(enabled && selected && isChoosingMushroomAppearing) {
                    int mushroomXSize = p_leftMushroomSprite->getGlobalBounds().size.x;
                    sf::Vector2f leftPos(buttonRect.position.x - mushroomXSize / 2.f, buttonRect.position.y + buttonRect.size.y / 2.f);
                    sf::Vector2f rightPos(buttonRect.position.x + buttonRect.size.x + mushroomXSize / 2.f, buttonRect.position.y + buttonRect.size.y / 2.f);

                    p_leftMushroomSprite->setPosition(leftPos);
                    p_rightMushroomSprite->setPosition(rightPos);
                    window->draw(*p_leftMushroomSprite);
                    window->draw(*p_rightMushroomSprite);
                }

                sf::Text text(*font, buttonText, fontSize);
                float textLenX = text.getGlobalBounds().size.x;
                float textLenY = text.getGlobalBounds().size.y;

                sf::Color textColor;
                if(state == ButtonState::Hover) {
                    textColor = sf::Color(84, 40, 38, 255);
                } else {
                    textColor = (state == ButtonState::Inactive) ? sf::Color(243, 246, 251, 98) : sf::Color(239, 139, 54, 255);
                }
                
                text.setFillColor(textColor);
                text.setPosition(sf::Vector2f(int(buttonRect.position.x + (buttonRect.size.x - textLenX) / 2.f), int(buttonRect.position.y + 4 + (buttonRect.size.y - textLenY) / 2.f)));
                window->draw(text); 
            }

            void setEnableState(bool _enable) {
                enabled = _enable;
            }

            bool isEnabled() const {
                return enabled;
            }
    };
}