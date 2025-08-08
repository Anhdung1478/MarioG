#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace mario {
    class Slider {
        private:
            std::unique_ptr<sf::Texture> barTexture;
            std::unique_ptr<sf::Texture> handleTexture;
            std::unique_ptr<sf::Sprite> barSprite;
            std::unique_ptr<sf::Sprite> handleSprite;
            std::unique_ptr<sf::Text> labelText;
            std::unique_ptr<sf::Font> font; 

            float minValue;
            float maxValue;
            float currentValue;
            sf::Vector2f Scale;
            sf::Vector2f position;

            bool isDragging = false;

            // Callback function
            std::function<void(float)> onValueChanged;

        public:
            Slider(std::unique_ptr<sf::Texture> barTex,  std::unique_ptr<sf::Texture> handleTex, std::unique_ptr<sf::Font> font, 
                        sf::Vector2f pos, std::string label, sf::Vector2f Sc, float minVal = 0.f, float maxVal = 100.f, float initialVal = 50.0f);

            void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
            void update(const sf::RenderWindow& window);
            void render(sf::RenderTarget& target);

            void setValue(float value);
            float getValue() const;

            void setOnValueChanged(std::function<void(float)> callback);

            void setPosition(sf::Vector2f newPos);
    };
}