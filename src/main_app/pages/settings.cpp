#include "settings.hpp"
#include "main-menu.hpp" // để quay lại menu chính

using namespace mario::pages;

SettingsPage::SettingsPage(MainWindow& context) : Page(context) {
    p_font = std::make_unique<sf::Font>("../../asset/fonts/Cascadia.ttf");

    p_title = std::make_unique<sf::Text>(*p_font, "Settings", 40);
    p_title->setPosition({540, 100});
    p_title->setFillColor(sf::Color::White);

    p_settingsButtonListNode = new ButtonListNode();

    // Music Toggle Button
    auto* musicToggleBtn = new Button("Music: " + std::string(_context->getSoundManager().isMusicEnable() ? "On" : "Off"));
    musicToggleBtn->buttonRect = sf::FloatRect({540, 200}, {200, 30});
    musicToggleBtn->Click.append([this, musicToggleBtn]() {
        bool current = _context->getSoundManager().isMusicEnable();
        _context->getSoundManager().toggleBackgroundMusic(!current);
        musicToggleBtn->buttonText = "Music: " + std::string(!current ? "On" : "Off");
    });
    p_settingsButtonListNode->buttonList.push_back(musicToggleBtn);

    // Sound Toggle Button
    auto* soundToggleBtn = new Button("SFX: " + std::string(_context->getSoundManager().isSoundEnable() ? "On" : "Off"));
    soundToggleBtn->buttonRect = sf::FloatRect({540, 250}, {200, 30});
    soundToggleBtn->Click.append([this, soundToggleBtn]() {
        bool current = _context->getSoundManager().isSoundEnable();
        _context->getSoundManager().toggleSoundEffects(!current);
        soundToggleBtn->buttonText = "SFX: " + std::string(!current ? "On" : "Off");
    });
    p_settingsButtonListNode->buttonList.push_back(soundToggleBtn);

    // Music Volume Up
    auto* musicUpBtn = new Button("Music Volume +");
    musicUpBtn->buttonRect = sf::FloatRect({540, 300}, {200, 30});
    musicUpBtn->Click.append([this]() {
        _context->getSoundManager().adjustBackgroundMusicVolume(10.0f + _context->getSoundManager().getMusicVolume());
    });
    p_settingsButtonListNode->buttonList.push_back(musicUpBtn);

    // Music Volume Down
    auto* musicDownBtn = new Button("Music Volume -");
    musicDownBtn->buttonRect = sf::FloatRect({540, 350}, {200, 30});
    musicDownBtn->Click.append([this]() {
        _context->getSoundManager().adjustBackgroundMusicVolume(_context->getSoundManager().getMusicVolume() - 10.0f);
    });
    p_settingsButtonListNode->buttonList.push_back(musicDownBtn);

    // SFX Volume Up
    auto* sfxUpBtn = new Button("SFX Volume +");
    sfxUpBtn->buttonRect = sf::FloatRect({540, 400}, {200, 30});
    sfxUpBtn->Click.append([this]() {
        _context->getSoundManager().adjustSoundEffectsVolume(10.0f + _context->getSoundManager().getSoundVolume());
    });
    p_settingsButtonListNode->buttonList.push_back(sfxUpBtn);

    // SFX Volume Down
    auto* sfxDownBtn = new Button("SFX Volume -");
    sfxDownBtn->buttonRect = sf::FloatRect({540, 450}, {200, 30});
    sfxDownBtn->Click.append([this]() {
        _context->getSoundManager().adjustSoundEffectsVolume(_context->getSoundManager().getSoundVolume() - 10.0f);
    });
    p_settingsButtonListNode->buttonList.push_back(sfxDownBtn);

    // Back to Menu
    auto* backBtn = new Button("Back");
    backBtn->buttonRect = sf::FloatRect({540, 500}, {200, 30});
    backBtn->Click.append([this]() {
        _context->changePage(std::make_shared<mario::pages::MainMenuPage>(*_context));
    });
    p_settingsButtonListNode->buttonList.push_back(backBtn);

    p_currButtonList = std::make_unique<ButtonList>(p_settingsButtonListNode, nullptr);
}

void SettingsPage::handleEvent(const sf::RenderWindow* window, const sf::Event& event) {
    if (p_currButtonList)
        p_currButtonList->handleEvent(window, event);
}

void SettingsPage::update(const sf::RenderWindow* window, float dt) {
    if (p_currButtonList){
        p_currButtonList->update(window, dt);
    }
}

void SettingsPage::render(sf::RenderWindow* window) {
    window->draw(*p_title);
    if (p_currButtonList){
        p_currButtonList->render(window);
    }
}
