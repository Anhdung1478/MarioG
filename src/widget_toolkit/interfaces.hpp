#pragma once

#include <SFML/Graphics.hpp>

namespace mario
{
    class IRenderable
    {
        public:
        virtual ~IRenderable() = default;
        virtual std::unique_ptr<sf::RenderWindow> render(const std::unique_ptr<sf::RenderWindow> window) = 0;
    };

    class IUpdateable
    {
        public:
        virtual ~IUpdateable() = default;
        virtual void update(float deltaTime) = 0;
        virtual void handleEvent(const sf::Event &event) = 0;
    };

    class IScreenElement : public IRenderable, public IUpdateable {};

    class IStyle
    {
        public:
        virtual ~IStyle() = default;
        virtual void renderWithStyle(IRenderable *self) = 0;
    };
}
