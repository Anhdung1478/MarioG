#pragma once

#include<bits/stdc++.h>
#include "command.hpp"
#include "../resource/json.hpp"

namespace mario::input {
    class InputManager {
        private:
            std::unordered_map<sf::Keyboard::Scan, std::unique_ptr<mario::input::Command>> keyMap;
            MainWindow* context;

        public:
            InputManager(MainWindow& ctx) : context(&ctx) {
                keyMap.clear();
                loadKeyBindings();
            }

            void bind(sf::Keyboard::Scan scanKey, std::unique_ptr<mario::input::Command> command) {
                keyMap[scanKey] = std::move(command);
            }

            void handleEvent(mario::entity::Player &player, const sf::Event &event) {
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

            // Load key bindings from JSON file
            void loadKeyBindings() {
                std::ifstream file("../../src/widget_toolkit/keybindings.json");
                nlohmann::json config;
                if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
                    try {
                        file >> config;
                    } catch (const nlohmann::json::parse_error& e) {
                        std::cerr << "JSON parse error: " << e.what() << std::endl;
                        loadDefaultBindings();
                        return;
                    }
                    file.close();
                } else {
                    std::cerr << "Warning: keybindings.json not found or empty, using default bindings" << std::endl;
                    loadDefaultBindings();
                    return;
                }

                keyMap.clear();
                if (config.contains("keybindings")) {
                    const auto& bindings = config["keybindings"];
                    if (bindings.contains("move_left")) {
                        bind(static_cast<sf::Keyboard::Scan>(bindings["move_left"].get<int>()), std::make_unique<mario::input::RunCommand>(0));
                    }
                    if (bindings.contains("move_right")) {
                        bind(static_cast<sf::Keyboard::Scan>(bindings["move_right"].get<int>()), std::make_unique<mario::input::RunCommand>(1));
                    }
                    if (bindings.contains("jump")) {
                        bind(static_cast<sf::Keyboard::Scan>(bindings["jump"].get<int>()), std::make_unique<mario::input::JumpCommand>(*context));
                    }
                    if (bindings.contains("fire")) {
                        bind(static_cast<sf::Keyboard::Scan>(bindings["fire"].get<int>()), std::make_unique<mario::input::FireCommand>());
                    }
                } else {
                    loadDefaultBindings();
                }
            }

            // Load default bindings
            void loadDefaultBindings() {
                bind(sf::Keyboard::Scan::Left, std::make_unique<mario::input::RunCommand>(0));
                bind(sf::Keyboard::Scan::Right, std::make_unique<mario::input::RunCommand>(1));
                bind(sf::Keyboard::Scan::Up, std::make_unique<mario::input::JumpCommand>(*context));
                bind(sf::Keyboard::Scan::Z, std::make_unique<mario::input::FireCommand>());
            }
    };
}