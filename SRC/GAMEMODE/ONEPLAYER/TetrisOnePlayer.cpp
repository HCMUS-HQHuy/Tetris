#include "TetrisOnePlayer.hpp"

#include "Common.hpp"
#include "Player.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "SoundManager.hpp"

TetrisOnePlayer::TetrisOnePlayer(sf::RenderWindow* win, Scene* s):TetrisBaseMode(win, s) {
    player = nullptr;
}

TetrisOnePlayer::~TetrisOnePlayer() {
    delete player; player = nullptr;
}

STATUS_CODE TetrisOnePlayer::start() {
    sf::Texture backgroundTexture;
    sf::Sprite  backgroundSprite;
    sf::Music   backgroundMusic;
    loadPlayground(backgroundTexture, backgroundSprite);
    backgroundMusic.play();


restartGameOnePlayer:
    int X_COORDINATE = window->getSize().x / 2 - BLOCK_SIZE * 23 / 2 - BLOCK_SIZE;
    int Y_COORDINATE = 10;
    Player* player = new Player(X_COORDINATE, Y_COORDINATE);
    player->start();

    STATUS_CODE screenStatus = STATUS_CODE::QUIT;

    // Fade in: change menu
    {
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        scene->drawChangeMenu(window, true);
    }

    {
        int HOLD_WIDTH         = 5;

        int GRID_WIDTH         = 10;
        int GRID_HEIGHT        = 24;
        int GRID_POSITION_X    = X_COORDINATE + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
        int GRID_POSITION_Y    = Y_COORDINATE;

        // scene->drawCountdown(window, 
        //     (GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE / 2 - WIDTH_BORDER),
        //     (GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE / 2 - WIDTH_BORDER),
        //     -1,
        //     -1
        // );
    }

    sf::Event event;
    while (not player->isGameOver()) {
        while (window->pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                scene->drawChangeMenu(window, false);

                goto quitStartGameOnePlayer;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    STATUS_CODE escapeOption = scene->drawPause(window);

                    switch (escapeOption) {
                        case STATUS_CODE::RESUME:
                            break;
                        case STATUS_CODE::RESTART:
                            screenStatus = STATUS_CODE::RESTART;
                            
                            goto quitStartGameOnePlayer;
                        case STATUS_CODE::MENU:
                            screenStatus = STATUS_CODE::MENU;
                            
                            goto quitStartGameOnePlayer;
                        case STATUS_CODE::QUIT:
                            screenStatus = STATUS_CODE::QUIT;
                            
                            goto quitStartGameOnePlayer;
                        default:
                            throw std::runtime_error("[escapeOption] cannot find STATUS_CODE");
                    }
                }
            }

            player->processEvents(event);
        }

        player->autoDown();
        
        window->clear();
        window->draw(backgroundSprite); // Draw background
        player->draw(window);
        window->display();

        if (player->isGameOver()) {
            window->clear();
            window->draw(backgroundSprite); // Draw background
            player->draw(window);
            window->display();

            screenStatus = scene->drawGameOver(window);
        }

        backgroundMusic.setVolume(SoundManager::getVolume() - 20);
    }

quitStartGameOnePlayer:
    delete player;

    if (screenStatus == STATUS_CODE::RESTART) {
        goto restartGameOnePlayer;
    }

    return screenStatus;
}
