#include "mode-select.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "levels.hpp"
#include "main-menu.hpp"

namespace mario::pages {
    
    ModeSelectPage::ModeSelectPage(MainWindow& context) 
        : Page(context),
          marioFont(std::make_unique<sf::Font>("../../asset/fonts/SuperMario256.ttf")),
          _selectedLevel(1)  // Initialize _selectedLevel
    {
        // Setup title text
        _titleText = std::make_unique<sf::Text>(*marioFont);
        _titleText->setString("Select Game Mode");
        _titleText->setCharacterSize(50);
        _titleText->setFillColor(sf::Color::White);
        _titleText->setPosition(sf::Vector2f(100.f, 100.f));
    
        // Setup mode instructions text
        _modeText = std::make_unique<sf::Text>(*marioFont);
        _modeText->setString(
            "1 - SINGLE PLAYER\n"
            "2 - HOST GAME (You will play as MARIO)\n"
            "3 - JOIN GAME (You will play as LUIGI)\n\n"
            "ESC - Back to Main Menu"
        );
        _modeText->setCharacterSize(30);
        _modeText->setFillColor(sf::Color::White);
        _modeText->setPosition(sf::Vector2f(100.f, 200.f));
        
        // Setup connection status text
        _statusText = std::make_unique<sf::Text>(*marioFont);
        _statusText->setString("");
        _statusText->setCharacterSize(25);
        _statusText->setFillColor(sf::Color::Yellow);
        _statusText->setPosition(sf::Vector2f(100.f, 500.f));
    }
    
    void ModeSelectPage::handleEvent(const sf::RenderWindow* window, const sf::Event& event) {
        if (event.is<sf::Event::KeyPressed>()) {
            auto keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (!keyEvent) return;
    
            if (keyEvent->scancode == sf::Keyboard::Scancode::Num1) {
                // Single player
                _context->changePage(std::make_shared<MainMenuPage>(*_context));
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::Num2) {
                // Host game as MARIO
                _statusText->setString("Starting server...");
                _statusText->setFillColor(sf::Color::Yellow);
                
                auto networkManager = std::make_shared<NetworkManager>();
                if (networkManager->startServer(54000)) {
                    _statusText->setString("Server started! Waiting for player to join...");
                    
                    mario::resource::LevelState levelState(
                        _selectedLevel, 
                        3, // Start with 3 lives
                        0, 
                        0, 
                        mario::entity::CharacterListType::Mario, 
                        mario::entity::player_state::PlayerStateType::Small
                    );
                    _context->changePage(std::make_shared<LevelsPage>(*_context, levelState, networkManager, GameMode::Host));
                } else {
                    _statusText->setString("Failed to start server! Check if port 54000 is available.");
                    _statusText->setFillColor(sf::Color::Red);
                }
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::Num3) {
                // Join game as LUIGI
                _statusText->setString("Connecting to server...");
                _statusText->setFillColor(sf::Color::Yellow);
                
                auto networkManager = std::make_shared<NetworkManager>();
                if (networkManager->connectToServer("192.168.1.20", 54000)) {
                    _statusText->setString("Connected successfully!");
                    
                    mario::resource::LevelState levelState(
                        _selectedLevel, 
                        3, // Start with 3 lives
                        0, 
                        0, 
                        mario::entity::CharacterListType::Luigi, 
                        mario::entity::player_state::PlayerStateType::Small
                    );
                    _context->changePage(std::make_shared<LevelsPage>(*_context, levelState, networkManager, GameMode::Client));
                } else {
                    _statusText->setString("Failed to connect! Check if server is running and IP is correct.");
                    _statusText->setFillColor(sf::Color::Red);
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                // Back to main menu
                _context->changePage(std::make_shared<MainMenuPage>(*_context));
            }
        }
    }

    void ModeSelectPage::render(sf::RenderWindow* window) {
        if (window) {
            window->draw(*_titleText);
            window->draw(*_modeText);
            window->draw(*_statusText);
        }
    }

    void ModeSelectPage::update(const sf::RenderWindow* window, float deltaTime) {
        // Update logic here if needed
    }
}