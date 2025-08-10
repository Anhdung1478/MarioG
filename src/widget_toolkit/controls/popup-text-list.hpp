#pragma once

#include "popup-text.hpp"

namespace mario {
    class PopUpTextList : public IScreenElement {
        private:
            std::vector<PopUpText*> popUpTexts;
            sf::Vector2f velocity;

            sf::Color textColor;
            std::string fontPath;

            int fontSize;
            int idFirstAppear;
            float appearTime;

        public:
            PopUpTextList(float _appearTime, int _fontSize, sf::Vector2f _vel, std::string _fontPath, sf::Color _textColor) 
                : appearTime(_appearTime), fontSize(_fontSize), velocity(_vel), fontPath(_fontPath), textColor(_textColor), idFirstAppear(0) {}
            
            ~PopUpTextList() override {
                for (int it = idFirstAppear; it < int(popUpTexts.size()); ++it)
                    delete popUpTexts[it];
            }
    
            void addAPopUpText(sf::Vector2f spawnPoint, std::string text) {
                popUpTexts.push_back(new PopUpText(spawnPoint, text, appearTime, fontSize, velocity, fontPath, textColor));
            }

            void update(const sf::RenderWindow *window, float dt) override {
                for (int it = idFirstAppear; it < int(popUpTexts.size()); ++it) {
                    popUpTexts[it]->update(window, dt);
                }

                while(idFirstAppear < int(popUpTexts.size()) && popUpTexts[idFirstAppear]->isDisappeared())
                    delete popUpTexts[idFirstAppear++];
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {

            }

            void render(sf::RenderWindow *window) override {
                for (int it = idFirstAppear; it < int(popUpTexts.size()); ++it) {
                    PopUpText *text = popUpTexts[it];
                    text->render(window);
                }
            }
    };
}