#pragma once

#include<bits/stdc++.h>
#include "command.hpp"

namespace mario::input {
    class InputManager {
        private:
            std::unordered_map<sf::Keyboard::Scan, std::unique_ptr<mario::input::Command>> keyMap;
            
        public:
            InputManager() {
                keyMap.clear();
            }

            void bind(sf::Keyboard::Scan scanKey, std::unique_ptr<mario::input::Command> command) {
                keyMap[scanKey] = std::move(command);
            }

            void handleEvent(Player &player, const sf::Event &event) {
                if(const auto* key = event.getIf<sf::Event::KeyPressed>()) {
                    auto it = keyMap.find(key->scancode);
                    if(it != keyMap.end())
                        it->second->execute(player, 0);
                }

                if(const auto* key = event.getIf<sf::Event::KeyReleased>()) {
                    auto it = keyMap.find(key->scancode);
                    if(it != keyMap.end())
                        it->second->execute(player, 1);
                }
            }
    };
}