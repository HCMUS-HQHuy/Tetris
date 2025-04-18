#ifndef COMPETITOR_HPP
#define COMPETITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <mutex>
#include <queue>
#include <random>

class CurrentBlock;
class SoundManager;
class Player_Multiplayer;
class Monitor_Multiplayer;

class Competitor{
private:
    Monitor_Multiplayer* monitor;
    sf::TcpSocket socket;

    std::mutex mtx;
    
    CurrentBlock* curBlock;
    SoundManager *soundManager;

    std::mt19937 gen;
public:
    // Competitor(int X_COORDINATE, int Y_COORDINATE);
    Competitor(int X_COORDINATE, int Y_COORDINATE, sf::TcpListener &listenner, uint32_t seed);
    Competitor(int X_COORDINATE, int Y_COORDINATE, const char* ipv4, int port);
    ~Competitor();
    void start(Player_Multiplayer* &player);
    void ready(int& seed);

    bool isGameOver();

    void draw(sf::RenderWindow* window);
};

#endif