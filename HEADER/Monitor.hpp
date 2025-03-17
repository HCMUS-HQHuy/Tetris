#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <SFML/Graphics.hpp>

class Hold;
class LinkListBlock;
class Map;
class Infor;
class CurrentBlock;

class Monitor {
private:
    int X_COORDINATE;
    int Y_COORDINATE;
    
protected:
    Hold             *hold;
    CurrentBlock     *curBlock;
    Map              *map;
    Infor            *infor;
    LinkListBlock    *next;

    bool gameOver;
    void clearScreen(uint32_t seed);
public:
    Monitor(int X_COORDINATE, int Y_COORDINATE);
    virtual ~Monitor();
    
    bool isGameOver();
    void draw(sf::RenderWindow* window);
};

#endif