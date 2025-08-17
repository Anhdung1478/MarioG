#include "slider.hpp"
#include <iostream>
#include <algorithm>

using namespace mario;

Slider::Slider(std::unique_ptr<sf::Texture> barTex, std::unique_ptr<sf::Texture> handleTex, std::unique_ptr<sf::Font> f, 
               sf::Vector2f pos, std::string label, sf::Vector2f Sc, float minVal, float maxVal, float initialVal)
    : barTexture(std::move(barTex)), handleTexture(std::move(handleTex)), font(std::move(f)),
      minValue(minVal), maxValue(maxVal), currentValue(initialVal), Scale(Sc), position(pos) {
    barSprite = std::make_unique<sf::Sprite>(*barTexture);
    barSprite->setOrigin(sf::Vector2f(barTexture->getSize().x / 2.f, barTexture->getSize().y / 2.f));
    barSprite->setPosition(sf::Vector2f(position));
    barSprite->setScale(Scale);
    
    handleSprite = std::make_unique<sf::Sprite>(*handleTexture);
    handleSprite->setOrigin(sf::Vector2f(handleTexture->getSize().x / 2.f, handleTexture->getSize().y / 2.f));
    handleSprite->setPosition(sf::Vector2f(position));
    
    labelText = std::make_unique<sf::Text>(*font, label + ": " + std::to_string(static_cast<int>(currentValue)) + "%", 24);
    labelText->setFillColor(sf::Color::White);

    sf::FloatRect barBounds = barSprite->getGlobalBounds();
    labelText->setPosition(sf::Vector2f(barBounds.position.x - labelText->getGlobalBounds().size.x - 30, 
                                        barBounds.position.y + barBounds.size.y / 2.f - labelText->getCharacterSize() / 1.5f));
    
    // Initialize highlight rectangle
    highlightRect = std::make_unique<sf::RectangleShape>();
    highlightRect->setFillColor(sf::Color(222, 220, 7, 225));
    float highlightHeight = barBounds.size.y * 0.3f;
    highlightRect->setPosition(sf::Vector2f(barBounds.position.x + 28, 
                                        barBounds.position.y + (barBounds.size.y - highlightHeight) / 2));
    highlightRect->setSize(sf::Vector2f(0, highlightHeight));

    setValue(currentValue);
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({mouseButtonPressed->position.x, mouseButtonPressed->position.y});
            if (handleSprite->getGlobalBounds().contains(mousePos)) {
                isDragging = true;
            }
            // If clicking on the bar, move handle to mouse position
            else if (barSprite->getGlobalBounds().contains(mousePos)) {
                float barStartX = barSprite->getGlobalBounds().position.x + 28;
                float barWidth = barSprite->getGlobalBounds().size.x - 55;
                float handleWidth = handleSprite->getGlobalBounds().size.x;

                // Calculate new handle position within bounds
                float percent = (mousePos.x - barStartX) / barWidth;
                percent = std::clamp(percent, 0.f, 1.f);
                currentValue = minValue + (maxValue - minValue) * percent;

                setValue(currentValue);
                isDragging = true;

                if (onValueChanged) {
                    onValueChanged(currentValue);
                }
            }
        }
    } else if (auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
            isDragging = false;
        }
    } else if (auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (isDragging) {
            update(window);
        }
    }
}

void Slider::update(const sf::RenderWindow& window) {
    if (isDragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float barStartX = barSprite->getGlobalBounds().position.x + 28;
        float barWidth = barSprite->getGlobalBounds().size.x - 55;

        // Limit handle position
        float handleWidth = handleSprite->getGlobalBounds().size.x;

        // Limit handle position to stay within bar bounds
        float percent = (mousePos.x - barStartX) / barWidth;
        percent = std::clamp(percent, 0.f, 1.f);
        currentValue = minValue + (maxValue - minValue) * percent;

        setValue(currentValue);

        if (onValueChanged) {
            onValueChanged(currentValue);
        }
    }
}

void Slider::render(sf::RenderTarget& target) {
    target.draw(*barSprite);
    target.draw(*highlightRect);
    target.draw(*handleSprite);
    target.draw(*labelText);
}

void Slider::setValue(float value) {
    currentValue = std::clamp(value, minValue, maxValue);

    float barStartX = barSprite->getGlobalBounds().position.x + 28;
    float barWidth = barSprite->getGlobalBounds().size.x - 55;
    float handleWidth = handleSprite->getGlobalBounds().size.x;
    float percent = (currentValue - minValue) / (maxValue - minValue);

    // Add padding to keep handle within bar bounds
    float padding = handleWidth * 0.2f; // Adjust this value to control how much the handle is inset
    float handleX = barStartX + padding + (barWidth - 2 * padding) * percent;

    handleSprite->setPosition(sf::Vector2f(handleX, barSprite->getPosition().y));
    labelText->setString(labelText->getString().toAnsiString().substr(0, labelText->getString().toAnsiString().find(":") + 1)
                         + " " + std::to_string(static_cast<int>(currentValue)) + "%");

    // Update highlight rectangle to extend from bar start to handle position
    float highlightHeight = barSprite->getGlobalBounds().size.y * 0.3f;
    highlightRect->setSize(sf::Vector2f(handleX - barStartX, highlightHeight));
    highlightRect->setPosition(sf::Vector2f(barStartX, 
            barSprite->getGlobalBounds().position.y + (barSprite->getGlobalBounds().size.y - highlightHeight) / 2));
}

float Slider::getValue() const {
    return currentValue;
}

void Slider::setOnValueChanged(std::function<void(float)> callback) {
    onValueChanged = callback;
}

void Slider::setPosition(sf::Vector2f newPos) {
    position = newPos;

    barSprite->setPosition(position);

    float barStartX = barSprite->getGlobalBounds().position.x + 28;
    float barWidth = barSprite->getGlobalBounds().size.x - 55;
    float handleWidth = handleSprite->getGlobalBounds().size.x;
    float percent = (currentValue - minValue) / (maxValue - minValue);
    
    // Add padding to keep handle within bar bounds
    float padding = handleWidth * 0.2f; // Adjust this value to control how much the handle is inset
    float handleX = barStartX + padding + (barWidth - 2 * padding) * percent;
    
    handleSprite->setPosition(sf::Vector2f(handleX, barSprite->getPosition().y));

    sf::FloatRect barBounds = barSprite->getGlobalBounds();
    labelText->setPosition(sf::Vector2f(barBounds.position.x - labelText->getGlobalBounds().size.x - 30,
                                        barBounds.position.y + barBounds.size.y / 2.f - labelText->getCharacterSize() / 1.5f));

    // Update highlight rectangle position and size
    float highlightHeight = barBounds.size.y * 0.3f;
    highlightRect->setPosition(sf::Vector2f(barBounds.position.x + 28, 
                                            barBounds.position.y + (barBounds.size.y - highlightHeight) / 2));
    highlightRect->setSize(sf::Vector2f(handleX - (barBounds.position.x + 28), highlightHeight));
}