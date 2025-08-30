#include "settings.hpp"
#include "main-menu.hpp"

using namespace mario::pages;

SettingsPage::SettingsPage(MainWindow& context) : Page(context) {
    p_font = std::make_unique<sf::Font>("asset/fonts/SuperMario256.ttf");
    
    // Background texture and sprite
    backgroundTexture = std::make_unique<sf::Texture>("asset/sprites/mario-theme.png");
    backgroundSprite = std::make_unique<sf::Sprite>(*backgroundTexture);
    sf::Vector2u windowSize = sf::Vector2u(1280, 720);
    float bgScaleX = static_cast<float>(windowSize.x) / backgroundTexture->getSize().x;
    float bgScaleY = static_cast<float>(windowSize.y) / backgroundTexture->getSize().y;
    backgroundSprite->setScale(sf::Vector2f(bgScaleX, bgScaleY));
    backgroundSprite->setPosition(sf::Vector2f(0, 0));

    p_title = std::make_unique<sf::Text>(*p_font, "Settings", 60);
    p_title->setFillColor(sf::Color(0xFF, 0xD7, 0x00));
    p_title->setOutlineColor(sf::Color::Black);
    p_title->setOutlineThickness(3.0f);

    sf::FloatRect titleBounds = p_title->getLocalBounds();
    p_title->setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2.f,
                    titleBounds.position.y  + titleBounds.size.y / 2.f));
    p_title->setPosition(sf::Vector2f(windowSize.x / 2.f, 80.f));

    // Left Panel (Sliders)
    leftPanel = std::make_unique<sf::RoundedRectangleShape>(sf::Vector2f(500, 400), 20.f, 20);
    leftPanel->setFillColor(sf::Color(0, 0, 0, 180));
    leftPanel->setPosition(sf::Vector2f(100, 200));
    leftPanel->setOutlineThickness(2);
    leftPanel->setOutlineColor(sf::Color::White);

    // Right Panel (Key Bindings)
    rightPanel = std::make_unique<sf::RoundedRectangleShape>(sf::Vector2f(500, 400), 20.f, 20);
    rightPanel->setFillColor(sf::Color(0, 0, 0, 180));
    rightPanel->setPosition(sf::Vector2f(680, 200));
    rightPanel->setOutlineThickness(2);
    rightPanel->setOutlineColor(sf::Color::White);

    // Left Panel Title
    leftPanelTitle = std::make_unique<sf::Text>(*p_font, "Volume", 30);
    leftPanelTitle->setFillColor(sf::Color::White);
    sf::FloatRect leftBounds = leftPanelTitle->getLocalBounds();
    leftPanelTitle->setOrigin(sf::Vector2f(leftBounds.size.x / 2.f, leftBounds.size.y)); 
    leftPanelTitle->setPosition(sf::Vector2f(leftPanel->getPosition().x + leftPanel->getSize().x / 2.f,
                                leftPanel->getPosition().y + 50.f));

    // Right Panel Title
    rightPanelTitle = std::make_unique<sf::Text>(*p_font, "Key Configure", 30);
    rightPanelTitle->setFillColor(sf::Color::White);
    sf::FloatRect rightBounds = rightPanelTitle->getLocalBounds();
    rightPanelTitle->setOrigin(sf::Vector2f(rightBounds.size.x / 2.f, rightBounds.size.y)); 
    rightPanelTitle->setPosition(sf::Vector2f(rightPanel->getPosition().x + rightPanel->getSize().x / 2.f,
                                rightPanel->getPosition().y + 50.f));

    // Slider textures
    sliderBarTexture = std::make_unique<sf::Texture>("asset/textures/slider-bar.png");
    sliderHandleTexture = std::make_unique<sf::Texture>("asset/textures/slider-handle.png");

    sf::Vector2f pos = {windowSize.x / 2.f, windowSize.y / 2.f};

    // Center of left panel
    sf::Vector2f leftPos = leftPanel->getPosition();
    sf::Vector2f leftSize = leftPanel->getSize();
    sf::Vector2f leftCenter = leftPos + leftSize / 2.f;

    musicSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture),
        std::make_unique<sf::Texture>(*sliderHandleTexture),
        std::make_unique<sf::Font>(*p_font),
        leftCenter + sf::Vector2f(80.f, -50.f + 10.f),
        "Music",
        sf::Vector2f(0.3f, 0.3f),
        0.0f, 100.0f,
        _context->getSoundManager().getMusicVolume()
    );

    musicSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustBackgroundMusicVolume(value);
    });

    sfxSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture),
        std::make_unique<sf::Texture>(*sliderHandleTexture),
        std::make_unique<sf::Font>(*p_font),
        leftCenter + sf::Vector2f(80.f, 50.f + 10.f),
        "SFX",
        sf::Vector2f(0.3f, 0.3f),
        0.0f, 100.0f,
        _context->getSoundManager().getSoundVolume()
    );

    sfxSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustSoundEffectsVolume(value);
    });
    
    // Back Button (as sprite)
    backTexture = std::make_unique<sf::Texture>("asset/textures/home.png");
    backHoverTexture = std::make_unique<sf::Texture>("asset/textures/home-hover.png");
    backSprite = std::make_unique<sf::Sprite>(*backTexture);
    backSprite->setPosition({20, 20});
    backSprite->setScale({1.f, 1.f});

    // Center of right panel
    sf::Vector2f rightPos = rightPanel->getPosition();
    sf::Vector2f rightSize = rightPanel->getSize();
    sf::Vector2f rightCenter = rightPos + rightSize / 2.f;

    float spacing = 50.f;
    int totalButtons = 4;
    float startY = rightCenter.y - (totalButtons - 1) * spacing / 2.f;

    keyBindings.clear();
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Move Left",  sf::Keyboard::Scan::Left,  sf::Vector2f(rightCenter.x - 170.f, startY + 0 * spacing)));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Move Right", sf::Keyboard::Scan::Right, sf::Vector2f(rightCenter.x - 170.f, startY + 1 * spacing)));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Jump",       sf::Keyboard::Scan::Up,    sf::Vector2f(rightCenter.x - 170.f, startY + 2 * spacing)));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Fire",       sf::Keyboard::Scan::Z,    sf::Vector2f(rightCenter.x - 170.f, startY + 3 * spacing)));

    confirmButton = std::make_unique<sf::RectangleShape>(sf::Vector2f(100, 30));
    confirmButton->setOrigin(confirmButton->getSize() / 2.f);
    confirmButton->setPosition(sf::Vector2f(rightCenter.x, startY + 4 * spacing + 20));
    confirmButton->setFillColor(sf::Color(50, 50, 50));
    confirmButton->setOutlineColor(sf::Color::White);
    confirmButton->setOutlineThickness(1);

    confirmText = std::make_unique<sf::Text>(*p_font, "OK", 20);
    confirmText->setFillColor(sf::Color::White);
    sf::FloatRect textRect = confirmText->getLocalBounds();
    confirmText->setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.f, textRect.position.y + textRect.size.y / 2.f));
    confirmText->setPosition(confirmButton->getPosition());

    std::ifstream file("../../src/widget_toolkit/keybindings.json");
    nlohmann::json config;
    if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Warning: keybindings.json not found or empty, creating default file" << std::endl;
        config["keybindings"] = {
            {"move_left", static_cast<int>(sf::Keyboard::Scan::Left)},
            {"move_right", static_cast<int>(sf::Keyboard::Scan::Right)},
            {"jump", static_cast<int>(sf::Keyboard::Scan::Up)},
            {"fire", static_cast<int>(sf::Keyboard::Scan::Z)}
        };
        std::ofstream outFile("../../src/widget_toolkit/keybindings.json");
        if (outFile.is_open()) {
            outFile << std::setw(4) << config << std::endl;
            outFile.close();
        }
    } else {
        try {
            file >> config;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            config["keybindings"] = {
                {"move_left", static_cast<int>(sf::Keyboard::Scan::Left)},
                {"move_right", static_cast<int>(sf::Keyboard::Scan::Right)},
                {"jump", static_cast<int>(sf::Keyboard::Scan::Up)},
                {"fire", static_cast<int>(sf::Keyboard::Scan::Z)}
            };
        }
        file.close();
    }
    // Apply
    if (config.contains("keybindings")) {
        keyBindings[0]->setKey(static_cast<sf::Keyboard::Scan>(config["keybindings"]["move_left"].get<int>()));
        keyBindings[1]->setKey(static_cast<sf::Keyboard::Scan>(config["keybindings"]["move_right"].get<int>()));
        keyBindings[2]->setKey(static_cast<sf::Keyboard::Scan>(config["keybindings"]["jump"].get<int>()));
        keyBindings[3]->setKey(static_cast<sf::Keyboard::Scan>(config["keybindings"]["fire"].get<int>()));
    }
}

void SettingsPage::handleEvent(const sf::RenderWindow* window, const sf::Event& event) {
    musicSlider->handleEvent(event, *window);
    sfxSlider->handleEvent(event, *window);

    bool anyWaiting = false;
    for(auto& kb : keyBindings) {
        if(kb->handleEvent(event, *window, keyBindings)){
            anyWaiting = true;
        }
    }

    if(!anyWaiting) {
        if(auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if(mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                sf::FloatRect backRect = sf::FloatRect(backSprite->getPosition(),
                    sf::Vector2f(backTexture->getSize().x * backSprite->getScale().x,
                                backTexture->getSize().y * backSprite->getScale().y));

                if(backRect.contains(sf::Vector2f(mousePos))) {
                    backClicked = true;
                    backAnimClock.restart();
                }

                sf::FloatRect confirmRect(keyBindings[0]->getPosition() + sf::Vector2f(0, 200), sf::Vector2f(100, 30));
                if (confirmButton->getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    // Save key bindings to JSON
                    nlohmann::json config;
                    config["keybindings"]["move_left"] = static_cast<int>(keyBindings[0]->getCurrentKey());
                    config["keybindings"]["move_right"] = static_cast<int>(keyBindings[1]->getCurrentKey());
                    config["keybindings"]["jump"] = static_cast<int>(keyBindings[2]->getCurrentKey());
                    config["keybindings"]["fire"] = static_cast<int>(keyBindings[3]->getCurrentKey());
                    std::ofstream file("../../src/widget_toolkit/keybindings.json");
                    if (file.is_open()) file << config.dump(4);

                    confirmClicked = true;
                    confirmFlashClock.restart();
                }
            }
        }
    }

    if (auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            sf::FloatRect backRect = sf::FloatRect(backSprite->getPosition(),
                sf::Vector2f(backTexture->getSize().x * backSprite->getScale().x,
                             backTexture->getSize().y * backSprite->getScale().y));
            if (backRect.contains(sf::Vector2f(mousePos))) {
                backClicked = true;
                backAnimClock.restart();
            }
        }
    }
}

void SettingsPage::update(const sf::RenderWindow* window, float dt) {
    musicSlider->update(*window);
    sfxSlider->update(*window);

    for(auto& kb : keyBindings) {
        kb->update(*window);
    }

    // Check for hover state on back button
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::FloatRect backRect = sf::FloatRect(backSprite->getPosition(),
        sf::Vector2f(backTexture->getSize().x * backSprite->getScale().x,
                     backTexture->getSize().y * backSprite->getScale().y));
    if (backRect.contains(sf::Vector2f(mousePos))) {
        backSprite->setTexture(*backHoverTexture);
    } else {
        backSprite->setTexture(*backTexture);
    }

    if (confirmClicked) {
        float t = confirmFlashClock.getElapsedTime().asSeconds();

        if (t < 0.3f) {
            confirmButton->setFillColor(sf::Color(233, 241, 86, 200));
        }

        if (t < confirmAnimDuration) {
            float progress = t / confirmAnimDuration;
            float scale = 1.0f - 0.1f * std::sin(progress * 3.14159f); 
            confirmButton->setScale(sf::Vector2f(scale, scale));
        } else {
            confirmButton->setScale(sf::Vector2f(1.f, 1.f)); // reset scale
        }

        if (t > 0.5f) {
            confirmClicked = false;
        }
    } else {
        if (confirmButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            confirmButton->setFillColor(sf::Color(70, 70, 70));
        } else {
            confirmButton->setFillColor(sf::Color(50, 50, 50));
        }
        confirmButton->setScale(sf::Vector2f(1.f, 1.f));
    }

    if (backClicked) {
        float t = backAnimClock.getElapsedTime().asSeconds();

        if (t < backAnimDuration) {
            float progress = t / backAnimDuration;
            float scale = 1.0f - 0.1f * std::sin(progress * 3.14159f); 
            backSprite->setScale(sf::Vector2f(scale, scale));
        } else {
            backSprite->setScale(sf::Vector2f(1.f, 1.f)); // reset scale
            backClicked = false;

            _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
        }
    } else {
        backSprite->setScale(sf::Vector2f(1.f, 1.f));
    }

}

void SettingsPage::render(sf::RenderWindow* window) {
    window->draw(*backgroundSprite);
    window->draw(*p_title);

    window->draw(*leftPanel);
    window->draw(*rightPanel);

    window->draw(*leftPanelTitle);
    window->draw(*rightPanelTitle);

    musicSlider->render(*window);
    sfxSlider->render(*window);

    for (const auto& kb : keyBindings) {
        kb->render(*window);
    }

    // Draw Back Button
    window->draw(*backSprite);

    // Draw confirm button
    window->draw(*confirmButton);
    window->draw(*confirmText);
}