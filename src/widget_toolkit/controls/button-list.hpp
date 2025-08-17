#pragma once

#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include "button.hpp"

namespace mario {
    struct ButtonListNode {
        std::vector<Button*> buttonList;
    };
    
    class ButtonList : public IScreenElement {
        private:
            static constexpr float DEFAULT_DELAY_TIME_AFTER_CLICK_BUTTON = 3.f / 10.f;
            static constexpr float DEFAULT_DELAY_TIME_AFTER_MOVE_BUTTON = 15.f / 100.f;
            ButtonListNode *p_curListNode;
            ButtonList *p_prvList;
            int curr_button = 0;
            
        public:
            float delay_time = 0.f;
            ButtonList(ButtonListNode *p_cur = nullptr, ButtonList *p_prv = nullptr) : curr_button(0), p_prvList(p_prv), p_curListNode(p_cur) {};

            void update(const sf::RenderWindow *window, float dt) override {
                if(delay_time > 0.f || p_curListNode == nullptr) {
                    delay_time -= dt;
                    return;
                }
                
                int buttonListSize = p_curListNode->buttonList.size();
                if(p_curListNode != nullptr) {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
                        do {
                            curr_button = (curr_button + buttonListSize - 1) % buttonListSize; 
                        } while(!p_curListNode->buttonList[curr_button]->isEnabled());

                        delay_time = DEFAULT_DELAY_TIME_AFTER_MOVE_BUTTON;
                    }

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
                        do {
                            curr_button = (curr_button + 1) % buttonListSize;
                        } while(!p_curListNode->buttonList[curr_button]->isEnabled());

                        delay_time = DEFAULT_DELAY_TIME_AFTER_MOVE_BUTTON;
                    }

                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
                    for (int i = 0; i < buttonListSize; ++i) {
                        if(!p_curListNode->buttonList[i]->isEnabled())
                            continue;

                        if(p_curListNode->buttonList[i]->buttonRect.contains(sf::Vector2f(mouse_pos))) {
                            curr_button = i;
                        }
                    }
                }

                sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
                if(p_curListNode->buttonList[curr_button]->isEnabled() && (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Enter) 
                    || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && p_curListNode->buttonList[curr_button]->buttonRect.contains(sf::Vector2f(mouse_pos)))) {
                    
                    p_curListNode->buttonList[curr_button]->Click();
                    if(p_curListNode->buttonList[curr_button]->p_nodeOnButton != nullptr) {
                        ButtonList *p_curList = new ButtonList(p_curListNode, p_prvList);
                        p_curListNode = p_curListNode->buttonList[curr_button]->p_nodeOnButton;
                        p_prvList = p_curList;
                    }

                    delay_time = DEFAULT_DELAY_TIME_AFTER_CLICK_BUTTON;
                    return;
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape) && p_prvList != nullptr) {
                    ButtonList p_curList = *p_prvList;
                    p_curListNode = p_curList.p_curListNode;
                    delete p_prvList;

                    p_prvList = p_curList.p_prvList;
                    delay_time = DEFAULT_DELAY_TIME_AFTER_CLICK_BUTTON;
                    return;
                }

                for (int i = 0; i < buttonListSize; ++i) {
                    p_curListNode->buttonList[i]->update(window, dt);
                }
            }

            void handleEvent(const sf::RenderWindow *window, const sf::Event &event) override {
                if(delay_time > 0.f || p_curListNode == nullptr)
                    return;

                for (int i = 0; i < int(p_curListNode->buttonList.size()); ++i) {
                    p_curListNode->buttonList[i]->handleEvent(window, event);
                }
            }

            void render(sf::RenderWindow *window) override {
                if(p_curListNode == nullptr)
                    return;

                p_curListNode->buttonList[curr_button]->selected = true;
                for (int i = 0; i < int(p_curListNode->buttonList.size()); ++i) {
                    p_curListNode->buttonList[i]->render(window);
                }

                p_curListNode->buttonList[curr_button]->selected = false;
            }
    };
}