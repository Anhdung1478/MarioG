#pragma once

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "../interfaces.hpp"

namespace mario {
    enum class ButtonState {
        Normal = 0,
        Hover = 1,
        Pressed = 2
    };

    class Button : public IScreenElement {
        public:
            std::string buttonText;
            sf::FloatRect buttonRect;
            std::function<void()> Click;
            int fontSize;

            Button(const std::string &text, const sf::FloatRect &rect)
                : buttonText(text), buttonRect(rect), fontSize(20) {
            }
            
            void update(float dt) override {
                // Update logic for the button (not implemented for brevity)
            }

            std::unique_ptr<sf::RenderWindow> render(std::unique_ptr<sf::RenderWindow> window) override {
                // Render text here (not implemented for brevity)
                
                return window;
            }
        };
}