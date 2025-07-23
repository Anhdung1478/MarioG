#pragma once

#include <SFML/Graphics.hpp>

namespace mario
{
    class IRenderable
    {
        public:
        virtual ~IRenderable() = default;
        virtual void render(sf::RenderWindow *window) = 0;
    };

    class IUpdateable
    {
        public:
        virtual ~IUpdateable() = default;
        virtual void update(const sf::RenderWindow *window, float deltaTime) = 0;
        virtual void handleEvent(const sf::RenderWindow *window, const sf::Event &event) = 0;
    };

    class IScreenElement : public IRenderable, public IUpdateable {};

    class IStyle
    {
        public:
        virtual ~IStyle() = default;
        virtual void renderWithStyle(IRenderable *self) = 0;
    };
}
