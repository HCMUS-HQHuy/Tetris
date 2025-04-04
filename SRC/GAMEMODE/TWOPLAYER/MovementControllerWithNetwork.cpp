#include "MovementControllerWithNetwork.hpp"

#include "SFML/Network.hpp"
#include "MessageCode.hpp"
#include "MonitorForTwoPlayer.hpp"
#include "Infor.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "LinkListBlock.hpp"
#include "CurrentBlockController.hpp"
#include "CurrentBlockWithNetwork.hpp"

MovementControllerWithNetwork::MovementControllerWithNetwork(Monitor* monitor, CurrentBlockController* controller, sf::TcpSocket* sock)
                                : MovementController(monitor, controller), socket(sock) {}

MovementControllerWithNetwork::~MovementControllerWithNetwork(){}

void MovementControllerWithNetwork::handleHold() {
    sf::Packet packet; packet << HOLD;
    if (socket->send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    MovementController::handleHold();
}

void MovementControllerWithNetwork::handlePut() {
    int nLines = curBlock->putIntoMap();

    sf::Packet packet; packet << PUT;
    curBlock->compressWithSpin(packet);
    if (nLines == 0) {
        std::random_device rd; int seed = rd();
        packet << seed;
        dynamic_cast<MonitorForTwoPlayer*>(monitor)->mapReceiveLineFromCompetitor(seed);
    }
    if (socket->send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");

    monitor->addLineToInfor(nLines, curBlock->getCurrentBlock());

    collision = false;
    curBlock->setter((monitor->getNext())->updateNext());
    monitor->unlockHold();
    if (curBlock->gameOver())
        setGameOver();
}

void MovementControllerWithNetwork::setGameOver() {
    sf::Packet packet; packet << GAMEOVER;
    if (socket->send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    monitor->setGameOver();
}

void MovementControllerWithNetwork::handleRotateLeft() {
    MovementController::handleRotateLeft();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket->send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}

void MovementControllerWithNetwork::handleRotateRight() {
    MovementController::handleRotateRight();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket->send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}

void MovementControllerWithNetwork::handleRotate180() {
    MovementController::handleRotate180();
    if (curBlock->isJustSpin()) {
        sf::Packet packet; packet << SPIN;
        if (socket->send(packet) != sf::Socket::Done)
            throw std::runtime_error("Failed to send event!");
    }
}