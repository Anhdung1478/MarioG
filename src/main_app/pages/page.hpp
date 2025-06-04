#pragma once

#include "../../widget_toolkit/interfaces.hpp"

namespace mario {
    class MainWindow;

    class Page : public IScreenElement {
        protected:
            MainWindow *_context;

        public:
            Page(MainWindow &context) : _context(&context) { }
            
            void handleEvent(const sf::Event &event) { }
            
            void setContext(MainWindow *context) { _context = context; }

            virtual ~Page() { }
    };
}