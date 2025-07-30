#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

namespace mario::pages {
class KeyBindingButton {
private:
    sf::Text actionText; // Action name (e.g., "Jump")
    sf::Text keyText;    // Current key name (e.g., "Up")
    sf::RectangleShape buttonShape; // Rectangle around the key
    sf::Font* font;
    bool isWaitingForInput; // Waiting for key press state
    sf::Keyboard::Scan currentKey; // Current key
    std::string actionName;

    // Map scancode to key name
    static const std::unordered_map<sf::Keyboard::Scan, std::string> keyNames;

public:
    KeyBindingButton(sf::Font* font, const std::string& action, sf::Keyboard::Scan initialKey, const sf::Vector2f& position)
        : font(font), actionName(action), currentKey(initialKey), isWaitingForInput(false),
           actionText(*font, action, 20), keyText(*font, getKeyName(initialKey), 20) {
        actionText.setFont(*font);
        actionText.setString(action);
        actionText.setCharacterSize(20);
        actionText.setFillColor(sf::Color::White);
        actionText.setPosition(position);

        keyText.setFont(*font);
        keyText.setString(getKeyName(initialKey));
        keyText.setCharacterSize(20);
        keyText.setFillColor(sf::Color::White);
        keyText.setPosition(position + sf::Vector2f(150, 0));

        buttonShape.setSize(sf::Vector2f(200, 30));
        buttonShape.setPosition(position + sf::Vector2f(140, -5));
        buttonShape.setFillColor(sf::Color(50, 50, 50));
        buttonShape.setOutlineColor(sf::Color::White);
        buttonShape.setOutlineThickness(1);
    }

    void update(const sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool isHovered = buttonShape.getGlobalBounds().contains(sf::Vector2f(mousePos));
        buttonShape.setFillColor(isHovered || isWaitingForInput ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));

        if (isWaitingForInput) {
            keyText.setString("Press a key...");
        } else {
            keyText.setString(getKeyName(currentKey));
        }
    }

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window, const std::vector<std::unique_ptr<KeyBindingButton>>& otherButtons) {
        if (auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if(mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (buttonShape.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    isWaitingForInput = true;
                    return true;
                }
            }
        } else if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            if(isWaitingForInput) {
                sf::Keyboard::Scan newKey = keyPressed->scancode;
                // Check for key conflict
                for (const auto& other : otherButtons) {
                    if (other.get() != this && other->getCurrentKey() == newKey) {
                        isWaitingForInput = false;
                        return false; // Key conflict
                    }
                }
                currentKey = newKey;
                isWaitingForInput = false;
                return true;
            }
        }
        return false;
    }

    void render(sf::RenderWindow& window) const {
        window.draw(buttonShape);
        window.draw(actionText);
        window.draw(keyText);
    }

    sf::Keyboard::Scan getCurrentKey() const { return currentKey; }
    bool isWaiting() const { return isWaitingForInput; }
    void setKey(sf::Keyboard::Scan key) {
        currentKey = key;
        keyText.setString(getKeyName(key));
    }

    sf::Vector2f getPosition() const {
        return actionText.getPosition();
    }
private:
    std::string getKeyName(sf::Keyboard::Scan key) const {
        auto it = keyNames.find(key);
        if (it != keyNames.end()) {
            return it->second;
        }
        return "Unknown";
    }

    static const std::unordered_map<sf::Keyboard::Scan, std::string> initKeyNames();
};
}