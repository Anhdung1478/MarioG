#include "select.hpp"
#include "main-menu.hpp"
#include "levels.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "../../widget_toolkit/resource/TextureManager.hpp"

mario::pages::SelectPage::SelectPage(MainWindow &context, int level)
    : Page(context), selectedChar(0), selectedLevel(level), selectedFrame(0), FRAME_SCALE(4.0f), FRAMES_Y(380.0f), framesStartX(0.0f), frameWidth(0.0f), FRAME_SPACING(200.0f),
      curtainSprite([&]() -> const sf::Texture& {
          TextureManager::getInstance().loadSheet("../../asset/sprites/curtain.json", "../../asset/sprites/curtain.png");
          return *TextureManager::getInstance().getSpriteData("curtain[0]").texture;
      }()),
      titleSprite([&]() -> const sf::Texture& {
          TextureManager::getInstance().loadSheet("../../asset/sprites/selection-frames.json", "../../asset/sprites/selection-frames.png");
          return *TextureManager::getInstance().getSpriteData("select-title[0]").texture;
      }()) {

    availableChar = {
        mario::entity::CharacterListType::Mario,
        mario::entity::CharacterListType::Luigi
    };

    const float SCREEN_WIDTH = 1280.0f;
    const float SCREEN_HEIGHT = 720.0f;

    if (!backgroundTexture.loadFromFile("../../asset/sprites/level_theme.jpg")) {
        throw std::runtime_error("Failed to load background image: level_theme.jpg");
    }
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(SCREEN_WIDTH / textureSize.x);
    float scaleY = static_cast<float>(SCREEN_HEIGHT / textureSize.y);

    // Bigger scale so we fill the screen
    float scale = std::max(scaleX, scaleY);
    backgroundSprite.setScale(sf::Vector2f(scale, scale));

    // Center crop
    sf::FloatRect bounds = backgroundSprite.getGlobalBounds();
    backgroundSprite.setPosition(sf::Vector2f((SCREEN_WIDTH - bounds.width) / 2.f, (SCREEN_HEIGHT - bounds.height) / 2.f));

    const SpriteData& curtainData = TextureManager::getInstance().getSpriteData("curtain[0]");
    curtainSprite.setTexture(*curtainData.texture);
    curtainSprite.setTextureRect(sf::IntRect({curtainData.x, curtainData.y}, {curtainData.z, curtainData.t}));

    // Scale curtain
    float curtainScaleX = SCREEN_WIDTH / static_cast<float>(curtainData.z);
    float curtainScaleY = SCREEN_HEIGHT / static_cast<float>(curtainData.t);
    curtainSprite.setScale(sf::Vector2f(curtainScaleX, curtainScaleY));
    curtainSprite.setPosition(sf::Vector2f(0, 0));

    const SpriteData& titleData = TextureManager::getInstance().getSpriteData("select-title[0]");
    titleSprite.setTexture(*titleData.texture);
    titleSprite.setTextureRect(sf::IntRect({titleData.x, titleData.y}, {titleData.z, titleData.t}));

    // Scale title
    titleSprite.setScale(sf::Vector2f(3.0f, 3.0f));
    float newTitleData = titleData.z * 3.0f;
    float titleX = (SCREEN_WIDTH - newTitleData) / 2.0f;
    float titleY = 220.0f;
    titleSprite.setPosition(sf::Vector2f(titleX, titleY));

    const SpriteData& frame0Data = TextureManager::getInstance().getSpriteData("char-frame[0]");
    frameWidth = frame0Data.z * FRAME_SCALE;
    float totalFramesWidth = (frameWidth * 2) + FRAME_SPACING;
    framesStartX = (SCREEN_WIDTH - totalFramesWidth) / 2.0f;

    for (size_t i = 0; i < availableChar.size(); ++i) {
        const SpriteData& frameData = TextureManager::getInstance().getSpriteData("char-frame[" + std::to_string(i) + "]");
        const SpriteData& charData = TextureManager::getInstance().getSpriteData(
            (i == 0) ? "mario[0]" : "luigi[0]"
        );
        sf::Sprite frameSprite(*frameData.texture);
        frameSprite.setTextureRect(sf::IntRect({frameData.x, frameData.y}, {frameData.z, frameData.t}));
        frameSprite.setScale(sf::Vector2f(FRAME_SCALE, FRAME_SCALE));

        float frameX = framesStartX + (i * (frameWidth + FRAME_SPACING));
        frameSprite.setPosition(sf::Vector2f(frameX, FRAMES_Y));

        sf::Sprite charSprite(*charData.texture);
        charSprite.setTextureRect(sf::IntRect({charData.x, charData.y}, {charData.z, charData.t}));

        float charScale = FRAME_SCALE * 0.8f;
        charSprite.setScale(sf::Vector2f(charScale, charScale));

        // Center the sprites within the frame
        float scaledFrameWidth = frameData.z * FRAME_SCALE;
        float scaledFrameHeight = frameData.t * FRAME_SCALE;
        float scaledCharWidth = charData.z * charScale;
        float scaledCharHeight = charData.t * charScale;

        float charX = frameX + (scaledFrameWidth - scaledCharWidth) / 2.0f;
        float charY = FRAMES_Y + (scaledFrameHeight - scaledCharHeight) / 2.0f;

        if (i == 0) {
            charX -= 5.0f; 
        } else if (i == 1) {
            charX += 10.0f;  
        }
        
        charSprite.setPosition(sf::Vector2f(charX, charY));

        charFrameSprites.push_back(frameSprite);
        charSprites.push_back(charSprite);
    }
}

mario::pages::SelectPage::~SelectPage() {}

void mario::pages::SelectPage::update(const sf::RenderWindow *window, float dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    
    static bool leftPressed = false;
    static bool rightPressed = false;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !leftPressed) {
        leftPressed = true;
        if (selectedFrame > 0) {
            selectedFrame--;
        }
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        leftPressed = false;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !rightPressed) {
        rightPressed = true;
        if (selectedFrame < availableChar.size() - 1) {
            selectedFrame++;
        }
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        rightPressed = false;
    }
    
    // Check mouse hover on frames
    int hoveredFrame = -1;
    for (size_t i = 0; i < charFrameSprites.size(); ++i) {
        sf::FloatRect frameBounds = charFrameSprites[i].getGlobalBounds();
        if (frameBounds.contains(mousePosF)) {
            hoveredFrame = static_cast<int>(i);
            selectedFrame = i; 
            break;
        }
    }
    
    for (size_t i = 0; i < charSprites.size(); ++i) {
        float targetScale = FRAME_SCALE * 0.8f;
        float animationSpeed = 9.0f; // Adjust for faster/slower animation
        
        // If this sprite is selected (hovered or keyboard selected), make it larger
        if (static_cast<int>(i) == selectedFrame) {
            targetScale = FRAME_SCALE * 1.5f; // Larger scale when selected
        }
        
        sf::Vector2f currentScale = charSprites[i].getScale();
        float currentScaleValue = currentScale.x;
        
        float newScale = currentScaleValue + (targetScale - currentScaleValue) * animationSpeed * dt;
        charSprites[i].setScale(sf::Vector2f(newScale, newScale));
        
        // keep sprites in center
        const SpriteData& frameData = TextureManager::getInstance().getSpriteData("char-frame[" + std::to_string(i) + "]");
        const SpriteData& charData = TextureManager::getInstance().getSpriteData(
            (i == 0) ? "mario[0]" : "luigi[0]"
        );
        
        float frameX = framesStartX + (i * (frameWidth + FRAME_SPACING));
        float scaledFrameWidth = frameData.z * FRAME_SCALE;
        float scaledFrameHeight = frameData.t * FRAME_SCALE;
        float scaledCharWidth = charData.z * newScale;
        float scaledCharHeight = charData.t * newScale;
        
        float charX = frameX + (scaledFrameWidth - scaledCharWidth) / 2.0f;
        float charY = FRAMES_Y + (scaledFrameHeight - scaledCharHeight) / 2.0f;
        
        if (i == 0) {
            charX -= 5.0f;  
        } else if (i == 1) {
            charX += 10.0f;  
        }
        
        charSprites[i].setPosition(sf::Vector2f(charX, charY));
    }
}

void mario::pages::SelectPage::handleEvent(const sf::RenderWindow *window, const sf::Event &event) {
    bool trigger = false;
    sf::Vector2f clickPosition;

    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left) {
            trigger = true;
            clickPosition = sf::Vector2f(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));
        }
    } else if (event.is<sf::Event::KeyPressed>()) {
        const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->scancode == sf::Keyboard::Scan::Enter) {
            trigger = true;

            // Simulate a click in the middle of the selectedChar frame
            if (selectedChar >= 0 && selectedChar < static_cast<int>(charFrameSprites.size())) {
                sf::FloatRect bounds = charFrameSprites[selectedChar].getGlobalBounds();
                clickPosition = bounds.position + bounds.size / 2.f;
            }
        }
    }

    if (trigger) {
        for (size_t i = 0; i < charFrameSprites.size(); i++) {
            if (charFrameSprites[i].getGlobalBounds().contains(clickPosition)) {
                selectedChar = static_cast<int>(i);
                _context->changePage(std::make_shared<mario::pages::LevelsPage>(
                    *_context,
                    mario::resource::LevelState(selectedLevel, 2, 0, 0, selectedChar, 0)
                ));
                break;
            }
        }
    }
}



void mario::pages::SelectPage::render(sf::RenderWindow *window) {
    window->draw(backgroundSprite);
    window->draw(curtainSprite);
    window->draw(titleSprite);
    for (size_t i = 0; i < charFrameSprites.size(); i++) {
        window->draw(charFrameSprites[i]);
        window->draw(charSprites[i]);
    }
}

int mario::pages::SelectPage::getSelectedCharacter() const {
    return selectedChar;
}