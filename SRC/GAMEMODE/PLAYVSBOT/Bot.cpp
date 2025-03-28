#include "Bot.hpp"

#include "CurrentBlock.hpp"
#include "CurrentBlockController.hpp"
#include "MovementController.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "PlayerWithBot.hpp"

#include <thread>
#include <iostream>

Bot::Bot(int x, int y, int seed): X_COORDINATE(x), Y_COORDINATE(y) {
    monitor = new MonitorForTwoPlayer(x, y);
    curBlock = new CurrentBlockController(monitor->getMap());
    movementController = new MovementController(monitor, curBlock);
    monitor->resetMonitor(seed);
}

Bot::~Bot() {
    delete monitor; monitor = nullptr;
    delete movementController; monitor = nullptr;
    delete curBlock; monitor = nullptr;
}

void Bot::setGameOver() { monitor->setGameOver(); }
bool Bot::isGameOver() { return monitor->isGameOver(); }

void Bot::start(PlayerWithBot* player) {
    resetComponent();
    std::thread thinking([this](PlayerWithBot* &player) {
        while (!monitor->isGameOver()) {
            std::cout << "THINKING!\n";
            sleep(2);
        }
    }, std::ref(player));
    thinking.detach();
}

void Bot::resetComponent() {
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
}

void Bot::draw(sf::RenderWindow *window) {
    monitor->draw(window, curBlock->getCurrentBlock());
}

void Bot::handleAddLine(uint8_t nLines) {
    monitor->inforReceiveLineFromCompetitor(nLines);
}

void Bot::update() {
    movementController->autoDown();
}