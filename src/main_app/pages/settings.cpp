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
}

void SettingsPage::handleEvent(const sf::RenderWindow* window, const sf::Event& event) {
    musicSlider->handleEvent(event, *window);
    sfxSlider->handleEvent(event, *window);

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
}

void SettingsPage::render(sf::RenderWindow* window) {
    window->draw(*p_title);
    musicSlider->render(*window);
    sfxSlider->render(*window);
    window->draw(*backSprite);
}