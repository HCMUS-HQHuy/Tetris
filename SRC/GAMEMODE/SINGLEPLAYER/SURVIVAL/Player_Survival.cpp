#include "Player_Survival.hpp"

#include "Monitor_Multiplayer.hpp"
#include "CurrentBlockController.hpp"
#include "MovementController.hpp"
#include "CurrentBlock.hpp"

#include <random>

Player_Survival::Player_Survival(int X_COORDINATE, int Y_COORDINATE) : Player(X_COORDINATE, Y_COORDINATE) {}

void Player_Survival::initialize() {
    monitor = new Monitor_Multiplayer(X_COORDINATE, Y_COORDINATE); 
    monitor->CreateMonitor(X_COORDINATE, Y_COORDINATE);

    curBlock = new CurrentBlockController(dynamic_cast<Monitor_Multiplayer*>(monitor)->getMap()); curBlock->createCurrentBlock();
    movementController = new MovementController(monitor, curBlock);
}

void Player_Survival::receiveGarbage(int x) {
    dynamic_cast<Monitor_Multiplayer *>(monitor)->inforReceiveLineFromCompetitor(4);
}

void Player_Survival::putGarbage() {
    std::random_device rd;
    std::mt19937 gen(rd());
    dynamic_cast<Monitor_Multiplayer *>(monitor)->mapReceiveLineFromCompetitor(gen());
    curBlock->getCurrentBlock()->updateShadow(monitor->getMap());
}