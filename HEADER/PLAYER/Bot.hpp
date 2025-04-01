#ifndef BOT_HPP
#define BOT_HPP

#include <unistd.h>
#include <mutex>
#include <queue>
#include <SFML/Graphics.hpp>

class Monitor;
class Monitor_VersusBot;
class MovementController_VersusBot;
class CurrentBlockController_Bot;
class Player_VersusBot;

class Bot {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    Monitor_VersusBot* monitor;
    MovementController_VersusBot *movementController;
    CurrentBlockController_Bot* curBlock;

    std::mutex mtx;
    std::queue<sf::Event> event;

public:
    Bot(int X_COORDINATE, int Y_COORDINATE);
    ~Bot();

    void setGameOver();
    bool isGameOver();

    void setCompetitor(Monitor*);

    void addEvent();

    void start(uint32_t seed, Player_VersusBot*);

    void handleAddLine(uint8_t nLines);

    void update(); // Cập nhật trạng thái bot theo thời gian
    void draw(sf::RenderWindow *window);

};

#endif