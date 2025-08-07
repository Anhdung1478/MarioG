#pragma once

#include "../main-window.hpp"
#include "../../widget_toolkit/interfaces.hpp"

namespace mario {
    class Page : public IScreenElement { // State pattern
        protected:
            MainWindow *_context;
        
        public:
            Page(MainWindow &context) : _context(&context) { }
            
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
            
            bool getPaused() const { return false; }

            virtual ~Page() { }
    };
}