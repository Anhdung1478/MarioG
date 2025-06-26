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
                if(_context) {
                    _context->closeWindow();
                }
            }

            virtual ~Page() { }
    };
}