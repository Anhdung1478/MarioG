#pragma once

#include "../main-window.hpp"
#include "../../widget_toolkit/interfaces.hpp"
#include "../../widget_toolkit/networking/GameMode.hpp"

namespace mario {
    class Page : public IScreenElement { // State pattern
        protected:
            MainWindow *_context;
            std::unique_ptr<Camera> p_camera;
        
        public:
            Page(MainWindow &context, sf::Vector2u cameraBounds = sf::Vector2u(1280, 720)) : _context(&context) {
                p_camera = std::make_unique<Camera>(cameraBounds);
            }
            
            void setContext(MainWindow *context) { _context = context; }

            void closeWindow() {
                if(_context) 
                    _context->closeWindow();
            }

            void rePositionTextToMiddle(sf::Text &text, int rectX, int rectY) {
                float textLenX = text.getGlobalBounds().size.x;
                float textLenY = text.getGlobalBounds().size.y;

                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(int((rectX - textLenX) / 2.0), rectY));
            }

            void updateCamera(float dt) {
                p_camera->update(dt);
            }

            void applyCameraTo(sf::RenderWindow &window) {
                p_camera->applyTo(window);
            }
            
            bool getPaused() const { return false; }

            virtual ~Page() {
                std::cout << "Reset camera view\n";
                p_camera->resetToDefaultView();
            }
    };
}