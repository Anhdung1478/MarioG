#include<bits/stdc++.h>
#include "page.hpp"

namespace mario::pages {
    class LevelsPage : public Page {
        public:


            LevelsPage(MainWindow &context);
            void update(const sf::RenderWindow *window, float dt) override;
            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override;
            void render(sf::RenderWindow *window) override;
    };
}