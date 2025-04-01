#include "Bot.hpp"

#include "CurrentBlock_Bot.hpp"
#include "CurrentBlockController_Bot.hpp"
#include "MovementController_VersusBot.hpp"
#include "Monitor_VersusBot.hpp"
#include "Player_VersusBot.hpp"
#include "Common.hpp"

#include <thread>
#include <chrono>
#include <iostream>

Bot::Bot(int x, int y): X_COORDINATE(x), Y_COORDINATE(y) {
    monitor = new Monitor_VersusBot(x, y); monitor->createMonitor(x, y);
    curBlock = new CurrentBlockController_Bot(monitor->getMap()); curBlock->createCurrentBlock();
    movementController = new MovementController_VersusBot(monitor, curBlock);
}

Bot::~Bot() {
    delete monitor; monitor = nullptr;
    delete movementController; monitor = nullptr;
    delete curBlock; monitor = nullptr;
}

void Bot::setGameOver() { monitor->setGameOver(); }
bool Bot::isGameOver() { return monitor->isGameOver(); }

void Bot::addEvent(const sf::Keyboard::Key &e) {
    sf::Event fakeEvent;
    mtx.lock();
    fakeEvent.type = sf::Event::KeyPressed;
    fakeEvent.key.code = e;
    event.push(fakeEvent);
    fakeEvent.type = sf::Event::KeyReleased;
    event.push(fakeEvent);
    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Bot::start(uint32_t seed, Player_VersusBot* player) {
    player->setCompetitor(monitor);
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
    finish.store(true);
    std::thread thinking([this](Player_VersusBot* &player) {
        while (!monitor->isGameOver()) {
            while(!finish);
            finish.store(false);

            int8_t target_X = 0, posX = Common::WIDTH_MAP / 2 - BLOCK_EDGE / 2;
            monitor->findPath(target_X, dynamic_cast<CurrentBlock_Bot*>(curBlock->getCurrentBlock()));
            while (posX != target_X) {
                if (target_X < posX) { addEvent(sf::Keyboard::Left); posX--; }
                if (target_X > posX) { addEvent(sf::Keyboard::Right); posX++; }
            }
            addEvent(sf::Keyboard::Space);
        }
    }, std::ref(player));
    thinking.detach();
}

void Bot::draw(sf::RenderWindow *window) {
    monitor->draw(window, curBlock->getCurrentBlock());
}

void Bot::handleAddLine(uint8_t nLines) {
    monitor->inforReceiveLineFromCompetitor(nLines);
}

void Bot::update() {
    mtx.lock();
    while (!event.empty()) {
        movementController->processEvents(event.front());
        if (event.front().type == sf::Event::KeyReleased && event.front().key.code == sf::Keyboard::Space)
            finish.store(true);
        event.pop();
    }

    mtx.unlock();
    // movementController->autoDown();
}

void Bot::setCompetitor(Monitor* mon) {
    movementController->setCompetitor(mon);
}