#include "Player.hpp"

#include "LinkListBlock.hpp"
#include "Map.hpp"
#include "CurrentBlock.hpp"
#include "Hold.hpp"

#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>

Player::Player(int X_COORDINATE, int Y_COORDINATE):Monitor(X_COORDINATE, Y_COORDINATE) {
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

#include <iostream>

Player::Player(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listener):Monitor(X_COORDINATE, Y_COORDINATE) {
    listener.accept(sendSock);
    std::cout << "New client connected: " << sendSock.getRemoteAddress() << std::endl;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

Player::Player(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port):Monitor(X_COORDINATE, Y_COORDINATE) {
    sendSock.connect(ipv4, port);
    std::cout << "New client connected: " << sendSock.getRemoteAddress() << std::endl;
    curBlock->setter(next->updateNext());
    curBlock->resetPosition(map);
}

Player::~Player() {
}

void Player::start() {
    // next->draw(window); 
    // curBlock->draw(window, map);

    // while (window->isOpen()) {
        // processEvents();
        // render();
        // update();
    // }
}

void Player::sendEvent(const sf::Event &event) {
    if (not(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)) return;
    if (not(event.key.code == sf::Keyboard::C || event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left))
        return;
    sf::Packet packet;
    
    // Pack the event type (e.g., KeyPressed) and the key code (e.g., Left, Right)
    packet << static_cast<int>(event.type) << static_cast<int>(event.key.code);

    if (sendSock.send(packet) != sf::Socket::Done)
        throw std::runtime_error("Failed to send event!");
    // std::cerr << "Sent event\n";
}