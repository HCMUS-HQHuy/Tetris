#include "PlayerWithBot.hpp"

#include "CurrentBlockController.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "MovementControllerWithBot.hpp"

#include <iostream>
#include <random>
#include <SFML/Network.hpp>

PlayerWithBot::PlayerWithBot(int x, int y, uint32_t seed): Player(x, y) {
    initialize(); 
    dynamic_cast<MonitorForTwoPlayer*>(monitor)->setNewSeed(seed);
    restart(seed);
}

void PlayerWithBot::initialize() {
    monitor = new MonitorForTwoPlayer(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController(monitor->getMap());
    movementController = new MovementControllerWithBot(monitor, curBlock);
}

PlayerWithBot::~PlayerWithBot() {}

void PlayerWithBot::setGameOver() {
    throw std::runtime_error("Not install set gameover yet");
    // dynamic_cast<MovementControllerWithBot*>(movementController)->setGameOver();
}

void PlayerWithBot::restart(uint32_t seed) {
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
}

void PlayerWithBot::handleAddLine(uint8_t nLines) {
    dynamic_cast<MonitorForTwoPlayer*>(monitor)->inforReceiveLineFromCompetitor(nLines);
}