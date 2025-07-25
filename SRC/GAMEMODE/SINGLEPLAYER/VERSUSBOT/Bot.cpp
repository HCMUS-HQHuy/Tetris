#include "Bot.hpp"

#include "CurrentBlock_Bot.hpp"
#include "CurrentBlockController_Bot.hpp"
#include "MovementController_VersusBot.hpp"
#include "Monitor_VersusBot.hpp"
#include "Player_VersusBot.hpp"
#include "CommonMap.hpp"

#include <chrono>
#include <iostream>

Bot::Bot(int x, int y): X_COORDINATE(x), Y_COORDINATE(y) {
    monitor = new Monitor_VersusBot(x, y); monitor->createMonitor(x, y);
    curBlock = new CurrentBlockController_Bot(monitor->getMap()); curBlock->createCurrentBlock();
    movementController = new MovementController_VersusBot(monitor, curBlock);
}

Bot::~Bot() {
    if (brain.joinable()) brain.join();
    if (handle.joinable()) handle.join();

    delete monitor; monitor = nullptr;
    delete movementController; monitor = nullptr;
    delete curBlock; monitor = nullptr;
}

void Bot::setGameOver() { 
    monitor->setGameOver();
    monitor->updateScoreForBrainBot();
}
bool Bot::isGameOver() { return monitor->isGameOver(); }

void Bot::addEvent(const sf::Keyboard::Key &e) {
    sf::Event fakeEvent;
    fakeEvent.type = sf::Event::KeyPressed;
    fakeEvent.key.code = e;
    mtx.lock();
    event.push(fakeEvent);
    fakeEvent.type = sf::Event::KeyReleased;
    event.push(fakeEvent);
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void Bot::start(uint32_t seed, Player_VersusBot* player) {
    if (brain.joinable()) brain.join();
    if (handle.joinable()) handle.join();

    player->setCompetitor(monitor);
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    while(!event.empty()) event.pop();
    finish.store(true); pauseGame.store(true);

    brain = std::thread(&Bot::play, this, std::ref(player));
    handle = std::thread(&Bot::update, this);
}

void Bot::setTimer() {
    pauseGame.store(true);
    monitor->setTimer();
    pauseGame.store(false);
}

void Bot::pauseTimer() {
    monitor->pauseTimer();
    pauseGame.store(true);
}

void Bot::unPauseTimer() {
    monitor->unPauseTimer();
    pauseGame.store(false);
}


void Bot::draw(sf::RenderWindow *window) {
    monitor->draw(window, curBlock->getCurrentBlock());
}

void Bot::handleAddLine(uint8_t nLines) {
    monitor->inforReceiveLineFromCompetitor(nLines);
}

void Bot::update() {
    while (!monitor->isGameOver()) {
        mtx.lock();
        while (!event.empty()) {
            movementController->processEvents(event.front());
            if (event.front().type == sf::Event::KeyReleased && event.front().key.code == sf::Keyboard::Space)
                finish.store(true);
            event.pop();
        }
        mtx.unlock();
    }
}

void Bot::play(Player_VersusBot* &player) {
    while (!monitor->isGameOver()) {
        while(!finish || pauseGame);
        finish.store(false);
        int8_t target_X = 0, timeRotate = 0, posX = WIDTH_MAP / 2 - BLOCK_EDGE / 2;
        bool isHold = false;
        monitor->findPath(target_X, timeRotate, isHold, dynamic_cast<CurrentBlock_Bot*>(curBlock->getCurrentBlock()));

        if (isHold) addEvent(sf::Keyboard::C);
        switch (timeRotate) {
            case 1:
                addEvent(sf::Keyboard::Up);
                break;
            case 2:
                addEvent(sf::Keyboard::A);
                break;
            case 3:
                addEvent(sf::Keyboard::Z);
                break;
            default:
                break;
        }

        while (posX != target_X) {
            if (target_X < posX) { addEvent(sf::Keyboard::Left); posX--; }
            if (target_X > posX) { addEvent(sf::Keyboard::Right); posX++; }
        }
        addEvent(sf::Keyboard::Space);
    }
}

void Bot::setCompetitor(Monitor* mon) {
    movementController->setCompetitor(mon);
}