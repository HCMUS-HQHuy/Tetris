#include "Tetris.hpp"

#include "Player.hpp"
#include "PlayerWithNetwork.hpp"
#include "Competitor.hpp"

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <thread>
#include <iostream>

const int WINDOW_WIDTH  = 1050;
const int WINDOW_HEIGHT = 700;

Tetris::Tetris() {}

Tetris::~Tetris() {
    delete window;
}

void Tetris::startGameOnePlayer() {
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH / 2, WINDOW_HEIGHT), "Tetr.io");

    Player* player = new Player(50, 10);
    player->start();
    while (window->isOpen()) {
        sf::Event event;

        if (player->isGameOver()) {
            player->restart();
        }

        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
            player->processEvents(event);
        }

        player->autoDown();

        window->clear();
        player->draw(window);
        window->display();
    }
}

void Tetris::makeConnection(bool isHost, Competitor* &competitor,PlayerWithNetwork* &player) {
    if (isHost) {
        sf::TcpListener listener;
        listener.listen(55001);
        std::random_device rd;
        int seed = rd();
        player = new PlayerWithNetwork(50, 10, listener, seed);
        listener.listen(55000);
        competitor = new Competitor(550, 10, listener, seed);
    }
    else {
        competitor = new Competitor(550, 10, "127.0.0.1", 55001);
        player = new PlayerWithNetwork(50, 10, "127.0.0.1", 55000);
    }
    isFinish.store(true);
}

void Tetris::startGameTwoPlayer(bool isHost) {
    Competitor* competitor;
    PlayerWithNetwork* player;

    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tetr.io");

    isFinish.store(false);
    std::thread connectThread(&Tetris::makeConnection, this, isHost, 
                                    std::ref(competitor), std::ref(player));
    connectThread.detach();

    while (window->isOpen() && !isFinish) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
        }
    }

    competitor->initPollEvent();

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                return;
            player->processEvents(event);
            player->sendEvent(event);
        }
        
        player->autoDown();
        competitor->start();

        window->clear();
        player->draw(window);
        competitor->draw(window);
        window->display();
    }
}