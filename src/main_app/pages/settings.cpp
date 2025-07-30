#include "settings.hpp"
#include "main-menu.hpp"

using namespace mario::pages;

SettingsPage::SettingsPage(MainWindow& context) : Page(context) {
    p_font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");

    p_title = std::make_unique<sf::Text>(*p_font, "Settings", 40);
    p_title->setPosition({540, 100});
    p_title->setFillColor(sf::Color::White);

    // Slider textures
    sliderBarTexture = std::make_unique<sf::Texture>("../../asset/textures/slider-bar.png");
    sliderHandleTexture = std::make_unique<sf::Texture>("../../asset/textures/slider-handle.png");

    sf::Vector2u windowSize =  sf::Vector2u(1280, 720);
    sf::Vector2f pos = {windowSize.x / 2.f, windowSize.y / 2.f};

    // Music Slider
    musicSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture),
        std::make_unique<sf::Texture>(*sliderHandleTexture),
        std::make_unique<sf::Font>(*p_font),
        sf::Vector2f(pos.x + 80, pos.y - 50),
        std::string("Music"),
        sf::Vector2f(0.5f, 0.5f),
        0.0f, 100.0f,
        _context->getSoundManager().getMusicVolume()
    );
    musicSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustBackgroundMusicVolume(value);
    });

    // SFX Slider
    sfxSlider = std::make_unique<Slider>(
        std::make_unique<sf::Texture>(*sliderBarTexture),
        std::make_unique<sf::Texture>(*sliderHandleTexture),
        std::make_unique<sf::Font>(*p_font),
        sf::Vector2f(pos.x + 80, pos.y + 50),
        std::string("SFX"),
        sf::Vector2f(0.5f, 0.5f),
        0.0f, 100.0f,
        _context->getSoundManager().getSoundVolume()
    );
    sfxSlider->setOnValueChanged([this](float value) {
        _context->getSoundManager().adjustSoundEffectsVolume(value);
    });

    // Back Button (as sprite)
    backTexture = std::make_unique<sf::Texture>("../../asset/textures/home.png");
    backHoverTexture = std::make_unique<sf::Texture>("../../asset/textures/home-hover.png");
    backSprite = std::make_unique<sf::Sprite>(*backTexture);
    backSprite->setPosition({20, 20});
    backSprite->setScale({1.f, 1.f});

    // Key Bindings
    sf::Vector2f keyPos = {pos.x + 280, pos.y - 300};
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Move Left", sf::Keyboard::Scan::Left, keyPos));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Move Right", sf::Keyboard::Scan::Right, keyPos + sf::Vector2f(0, 50)));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Jump", sf::Keyboard::Scan::Up, keyPos + sf::Vector2f(0, 100)));
    keyBindings.push_back(std::make_unique<KeyBindingButton>(p_font.get(), "Fire", sf::Keyboard::Scan::Z, keyPos + sf::Vector2f(0, 150)));

    // Confirm Button
    confirmButton = std::make_unique<sf::RectangleShape>(sf::Vector2f(100, 30));
    confirmButton->setPosition(keyPos + sf::Vector2f(0, 200));
    confirmButton->setFillColor(sf::Color(50, 50, 50));
    confirmButton->setOutlineColor(sf::Color::White);
    confirmButton->setOutlineThickness(1);
    // Confirm Text
    confirmText = std::make_unique<sf::Text>(*p_font, "OK", 20);
    confirmText->setFillColor(sf::Color::White);
    sf::FloatRect textRect = confirmText->getLocalBounds();
    confirmText->setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    confirmText->setPosition(confirmButton->getPosition() + sf::Vector2f(50, 15));

    // Read file keybindings.json
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
                    _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
                }

                sf::FloatRect confirmRect(keyBindings[0]->getPosition() + sf::Vector2f(0, 200), sf::Vector2f(100, 30));
                if (confirmRect.contains(sf::Vector2f(mousePos))) {
                    // Save key bindings to JSON
                    nlohmann::json config;
                    config["keybindings"]["move_left"] = static_cast<int>(keyBindings[0]->getCurrentKey());
                    config["keybindings"]["move_right"] = static_cast<int>(keyBindings[1]->getCurrentKey());
                    config["keybindings"]["jump"] = static_cast<int>(keyBindings[2]->getCurrentKey());
                    config["keybindings"]["fire"] = static_cast<int>(keyBindings[3]->getCurrentKey());
                    std::ofstream file("../../src/widget_toolkit/keybindings.json");
                    if (file.is_open()) file << config.dump(4);
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
                _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
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

    sf::FloatRect confirmRect = sf::FloatRect(confirmButton->getPosition(), confirmButton->getSize());
    if (confirmRect.contains(sf::Vector2f(mousePos))) {
        confirmButton->setFillColor(sf::Color(70, 70, 70));
    } else {
        confirmButton->setFillColor(sf::Color(50, 50, 50));
    }
}

void SettingsPage::render(sf::RenderWindow* window) {
    window->draw(*p_title);
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