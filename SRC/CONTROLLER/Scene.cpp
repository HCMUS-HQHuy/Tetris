#include "Scene.hpp"

#include "Menu.hpp"
#include "SoundManager.hpp"
#include "Common.hpp"

#include <iostream>
#include <windows.h>

const int TITLE_PADDING  = 100;

constexpr float TIME_OUT = 1.0f;

Scene::Scene(sf::RenderWindow *window) {
    font.loadFromFile("ASSETS/fonts/ARLRDBD.TTF");
    
    soundManager = new SoundManager();
    soundManager->loadSound("countdown", "ASSETS/sfx/countdown.mp3");
    soundManager->loadSound("selected" , "ASSETS/sfx/menu_hit.mp3");
    soundManager->loadMusicSound("menu", "ASSETS/sfx/tetristheme.mp3");

    menuBackgroundTexture.loadFromFile("ASSETS/menuBackground.png");
    menuBackground.setTexture(menuBackgroundTexture);
    menuBackground.setColor(sf::Color(255, 255, 255, 50));
    float scaleX = static_cast<float>(window->getSize().x) / menuBackgroundTexture.getSize().x;
    float scaleY = static_cast<float>(window->getSize().y) / menuBackgroundTexture.getSize().y;
    float scale = std::max(scaleX, scaleY);
    menuBackground.setScale(scale, scale);
    menuBackgroundX = window->getSize().x;
    menuBackground.setPosition(menuBackgroundX, 0);

    mainMenu = new Menu(window, {
        "SINGLEPLAYER",
        "MULTIPLAYER",
        "OPTION",
        "QUIT"
    }, MENU_CODE::MAIN);

    pauseMenu = new Menu(window, {
        "RESUME",
        "RESTART",
        "MENU",
        "QUIT"
    }, MENU_CODE::PAUSE);

    gameOverMenu = new Menu(window, {
        "RESTART",
        "MENU",
        "QUIT"
    }, MENU_CODE::GAMEOVER);
}

Scene::~Scene() {
    delete soundManager;

    delete mainMenu;
    delete pauseMenu;
    delete gameOverMenu;
}

bool Scene::notFocus(sf::RenderWindow *window) {    
    if (not window->hasFocus()) {
        ShowWindow(window->getSystemHandle(), SW_MINIMIZE);

        sf::Event event;
        while (window->pollEvent(event)) {
            // nothing
        }

        sf::sleep(sf::milliseconds(100));
   
        return true;
    }

    return false;
}

void Scene::drawChangeMenu(sf::RenderWindow *window, bool fadeIn) {
    sf::Texture screenshot;
    sf::Sprite  background;
    screenshot.create(window->getSize().x, window->getSize().y);
    
    window->display();

    screenshot.update(*window);
    background.setTexture(screenshot);
    
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

    overlayTimeout.restart(); 
    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        if (notFocus(window)) { continue; }

        sf::Event tempEvent;
        while (window->pollEvent(tempEvent)) {
            // Không làm gì cả -> Chỉ lấy ra để loại bỏ buffer
        }

        window->clear();

        window->draw(background); // Draw background

        if (fadeIn) {
            float alpha = 255 * (1 - overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(60, 60, 60, alpha));
        }
        else {
            float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(60, 60, 60, alpha));
        }
        window->draw(overlay);

        window->display();
    }
}

void Scene::drawMenuBackground(sf::RenderWindow *window) {
    if (menuBackgroundClock.getElapsedTime().asMilliseconds() > 16) { // ~60 FPS
        menuBackgroundClock.restart();
        
        menuBackgroundX += 1.0f;
    }
    
    // Nếu ảnh nền đi hết màn hình bên phải, đưa nó về bên trái
    float backgroundWidth = menuBackgroundTexture.getSize().x * menuBackground.getScale().x;
    if (menuBackgroundX >= backgroundWidth) {
        menuBackgroundX -= backgroundWidth; // Đưa về trái để trôi liên tục
    }

    menuBackground.setPosition(menuBackgroundX, 0);
    
    menuBackground.setPosition(menuBackgroundX, 0);

    // Đảm bảo vẽ hai lần để cuộn liên tục
    menuBackground.setPosition(menuBackgroundX, 0);
    window->draw(menuBackground);
    menuBackground.setPosition(menuBackgroundX - backgroundWidth, 0);
    window->draw(menuBackground);
}

STATUS_CODE Scene::drawMenu(sf::RenderWindow *window) {
    // return STATUS_CODE::QUIT;
    soundManager->playMusic("menu");

    {
        window->clear();

        drawMenuBackground(window);

        mainMenu->draw(window);

        window->display();

        drawChangeMenu(window, true);
    }

backToMainMenu:

    while (window->isOpen() and mainMenu->notSelected()) {
        if (notFocus(window)) { 
            soundManager->pauseMusic("menu");
            continue; 
        }
        soundManager->unPauseMusic("menu");

        sf::Event event;
        while (window->pollEvent(event)) {
            mainMenu->processEvents(window, event);
        }

        mainMenu->update(window);

        window->clear();

        drawMenuBackground(window);

        mainMenu->draw(window);

        window->display();
    }

    STATUS_CODE sceneStatus = mainMenu->getSelectedItem();

    switch (sceneStatus) {
        case STATUS_CODE::SINGLEPLAYER: {
            sceneStatus = drawSubMenu(window, mainMenu->getSubMenu(MENU_CODE::SINGLEPLAYER));

            break;
        }
        case STATUS_CODE::MULTIPLAYER: {
            sceneStatus = drawSubMenu(window, mainMenu->getSubMenu(MENU_CODE::MULTIPLAYER));

            break;
        }
        case STATUS_CODE::OPTION: {
            sceneStatus = drawSubMenu(window, mainMenu->getSubMenu(MENU_CODE::OPTION));

            break;
        }
        case STATUS_CODE::QUIT: {
            // quit - do nothing
            break;
        }
        default:
            throw std::invalid_argument("[Scene.cpp] - drawMenu(): STATUS_CODE error");
    }

    if (sceneStatus == STATUS_CODE::BACK) {
        goto backToMainMenu;
    }

    // Skip
    drawChangeMenu(window, false);

    soundManager->stopMusic("menu");

    return sceneStatus;
}

STATUS_CODE Scene::drawSubMenu(sf::RenderWindow *window, Menu *subMenu) {
    while (window->isOpen() and subMenu->notSelected()) {
        if (notFocus(window)) { 
            soundManager->pauseMusic("menu");
            continue; 
        }
        soundManager->unPauseMusic("menu");
        
        sf::Event event;
        while (window->pollEvent(event)) {
            subMenu->processEvents(window, event);
        }

        subMenu->update(window);

        window->clear();

        drawMenuBackground(window);

        subMenu->draw(window);

        window->display();
    }

    return subMenu->getSelectedItem();
}

STATUS_CODE Scene::waitingForConnection(sf::RenderWindow *window, std::atomic<bool> &isFinish) {
    sf::Clock waitingClock;
    uint8_t count = 0;
    std::string waiting = "Waiting for another player"; 
    sf::Text waitingText(waiting, font, 40);
    waitingText.setPosition(window->getSize().x  / 2 - waitingText.getGlobalBounds().width / 2, 
                            window->getSize().y / 2 - waitingText.getGlobalBounds().height / 2);

    while (window->isOpen() && !isFinish) {
        if (notFocus(window)) { continue; }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                
                return STATUS_CODE::QUIT;
            }
        }

        window->clear(sf::Color(30, 30, 30));
        if (waitingClock.getElapsedTime().asSeconds() >= 0.5f) {
            count++;

            if (count == 4) {
                count = 0;
                waiting.erase(waiting.end() - 6, waiting.end());

                waitingText.setString(waiting);
            } 
            else {
                waitingText.setString(waiting += " .");
            }

            waitingClock.restart();
        }
        window->draw(waitingText);
        window->display();
    }
    return STATUS_CODE::RESUME;
}

STATUS_CODE Scene::drawGameOver(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);

    overlayTimeout.restart();
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        // if (notFocus(window)) { continue; }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            // Chờ để hủy Restart ngay lập tức
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(60, 60, 60, std::min(alpha, 200.0f)));        // 200.0f để mờ mờ nhìn thấy background
        
        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->display();
    }

    while (window->isOpen() and gameOverMenu->notSelected()) {
        // if (notFocus(window)) { continue; }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            gameOverMenu->processEvents(window, event);
        }

        gameOverMenu->update(window);

        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);    
        gameOverMenu->draw(window);
        window->display();
    }
    
    drawChangeMenu(window, false);

    return gameOverMenu->getSelectedItem();
}

STATUS_CODE Scene::drawPause(sf::RenderWindow *window) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);
    
    overlayTimeout.restart();
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));

    soundManager->play("selected");

    while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
        if (notFocus(window)) { continue; }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            // Chờ để hủy resume ngay lập tức
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(60, 60, 60, std::min(alpha, 200.0f)));        // 200.0f để mờ mờ nhìn thấy background
        
        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        window->display();
    }

    while (window->isOpen() and pauseMenu->notSelected()) {
        if (notFocus(window)) { continue; }
        
        sf::Event event;
        while (window->pollEvent(event)) {
            pauseMenu->processEvents(window, event);
        }

        float alpha = 255 * (overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
        overlay.setFillColor(sf::Color(60, 60, 60, std::min(alpha, 200.0f)));

        pauseMenu->update(window);

        window->clear();
        window->draw(background); // Draw background
        window->draw(overlay);
        pauseMenu->draw(window);
        window->display();
    }

    STATUS_CODE result = pauseMenu->getSelectedItem();

    if (result == STATUS_CODE::RESUME) {
        overlayTimeout.restart(); 
        while (overlayTimeout.getElapsedTime().asSeconds() <= TIME_OUT) {
            if (notFocus(window)) { continue; }
        
            sf::Event tempEvent;
            while (window->pollEvent(tempEvent)) {
                // Không làm gì cả -> Chỉ lấy ra để loại bỏ buffer
            }

            window->clear();

            window->draw(background); // Draw background
            
            float alpha = 255 * (1 - overlayTimeout.getElapsedTime().asSeconds() / TIME_OUT);
            overlay.setFillColor(sf::Color(60, 60, 60, std::min(alpha, 200.0f)));
            
            window->draw(overlay);

            window->display();
        }
    }
    else {
        drawChangeMenu(window, false);
    }

    return result;
}

void Scene::drawCountdown(sf::RenderWindow *window, int gridCenterX, int gridCenterY, int otherGridCenterX = -1, int otherGridCenterY = -1) {
    sf::Texture screenshot;
    screenshot.create(window->getSize().x, window->getSize().y);
    screenshot.update(*window);
    sf::Sprite background;
    background.setTexture(screenshot);

    sf::Clock timeout;
    
    soundManager->play("countdown");

    int count = 3;
    while (true) {  // count >= 0
        if (notFocus(window)) { 
            soundManager->pause("countdown");
            
            continue; 
        }
        
        soundManager->unPause("countdown");
        
        sf::Event event;
        while (window->pollEvent(event)) {
            // clear buffer
        }

        if (timeout.getElapsedTime().asSeconds() > 1) {
            count--;

            if (count < 0) {
                break;
            }

            timeout.restart();
        }

        float alpha = 255 * (1 - timeout.getElapsedTime().asSeconds() / TIME_OUT);
        float scaleFactor = 1.0f + 1.0f * timeout.getElapsedTime().asSeconds();;  // Tăng scale

        sf::Text countdown(count ? std::to_string(count) : "GO!", font, 40);

        countdown.setFillColor(sf::Color(255, 255, 0, alpha)); // Vàng nhưng giảm alpha
        countdown.setScale(scaleFactor, scaleFactor);
        countdown.setPosition(sf::Vector2f(
            gridCenterX - countdown.getGlobalBounds().width / 2,
            gridCenterY - countdown.getGlobalBounds().height / 2
        ));

        window->clear();
        window->draw(background);
        window->draw(countdown);

        // Second playground
        if (otherGridCenterX != -1 and otherGridCenterY != -1) {
            countdown.setPosition(sf::Vector2f(
                otherGridCenterX - countdown.getGlobalBounds().width / 2,
                otherGridCenterY - countdown.getGlobalBounds().height / 2
            ));

            window->draw(countdown);
        }

        window->display();
    }
}