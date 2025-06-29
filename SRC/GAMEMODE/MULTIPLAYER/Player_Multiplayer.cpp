#include "Player_Multiplayer.hpp"

#include "CurrentBlockController_Multiplayer.hpp"
#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "Monitor_Multiplayer.hpp"
#include "MovementController_Multiplayer.hpp"
#include "SoundManager.hpp"

#include <iostream>
#include <random>
#include <SFML/Network.hpp>

Player_Multiplayer::Player_Multiplayer(int x, int y, sf::TcpListener &listener, uint32_t seed): Player(x, y) {
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
    socket.send(&seed, sizeof(seed));
    initialize(); dynamic_cast<Monitor_Multiplayer*>(monitor)->setNewSeed(seed);
    start(seed);
}

Player_Multiplayer::Player_Multiplayer(int x, int y, const char* ipv4, int port): Player(x, y){
    socket.connect(ipv4, port);
    uint32_t seed = 0; std::size_t tmp=0;
    if (socket.receive(&seed, sizeof(seed), tmp) != sf::Socket::Done) {
        throw std::runtime_error("Failed to receive seed!");
    }
    initialize(); dynamic_cast<Monitor_Multiplayer*>(monitor)->setNewSeed(seed);
    start(seed);
    std::cout << "New client connected: " << socket.getRemoteAddress() << " SEED:" << seed << std::endl;
}

void Player_Multiplayer::initialize() {
    monitor = new Monitor_Multiplayer(); monitor->createMonitor(X_COORDINATE, Y_COORDINATE);
    curBlock = new CurrentBlockController_Multiplayer(monitor->getMap()); curBlock->createCurrentBlock();
    movementController = new MovementController_Multiplayer(monitor, curBlock, socket);
}

Player_Multiplayer::~Player_Multiplayer() {}

void Player_Multiplayer::setGameOver() {
    monitor->setGameOver();
    sf::Packet packet; packet << GAMEOVER;
    if (socket.send(packet) != sf::Socket::Done)
        std::cerr << "Error: Failed to send GAMEOVER event!\n";
    // waitingComfirm(GAMEOVER);
}

void Player_Multiplayer::setQuitGame() {
    monitor->setGameOver();
    sf::Packet packet; packet << QUITGAME;
    if (socket.send(packet) != sf::Socket::Done)
        std::cerr << "Error: Failed to send GAMEOVER event!\n";
    // waitingComfirm(QUITGAME);
}

void Player_Multiplayer::sendCurBlock() {
    sf::Packet packet; packet << CURBLOCK;
    dynamic_cast<CurrentBlockController_Multiplayer*>(curBlock)->compress(packet);
    
    if (socket.send(packet) != sf::Socket::Done)
        std::cerr << "Error: Failed to send GAMEOVER event!\n";
} 

void Player_Multiplayer::start(uint32_t seed) {
    restart(seed);
}

void Player_Multiplayer::restart(uint32_t seed) {
    monitor->resetMonitor(seed);
    movementController->resetComponent();
    curBlock->setter(monitor->getNext());
    monitor->unlockHold();
}

void Player_Multiplayer::handleAddLine(uint8_t nLines) {
    dynamic_cast<Monitor_Multiplayer*>(monitor)->inforReceiveLineFromCompetitor(nLines);
    sf::Packet packet; packet << RECVLINE << nLines;

    if (socket.send(packet) != sf::Socket::Done)
        std::cerr << "Error: Failed to send GAMEOVER event!\n";
}

void Player_Multiplayer::ready(int& seed) {
    sf::Packet packet; packet << RESTART << seed;
    if (socket.send(packet) != sf::Socket::Done)
        std::cerr << "Error: Failed to send GAMEOVER event!\n";
    restart(seed);
}

void Player_Multiplayer::waitingComfirm(MessageCode code) {
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done)
        std:: cerr << "[WARNING] Failed to receive event! FROM competitor handler process\n";
    int messageCodeInt;
    packet >> messageCodeInt;
    if (messageCodeInt != code)
        std::cerr << "[WARNING] I don't understand message confirm! " + std::to_string(messageCodeInt) << '\n';
    else std::cerr << "[SUCESSFULLY] COMFIRM MessageCode!\n";
}