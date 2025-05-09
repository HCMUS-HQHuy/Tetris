#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <SFML/Graphics.hpp>

class Block;
class CurrentBlock;

class Map {
private:
    sf::Texture texture;
    
    void remove(uint8_t pos);

protected:
    int GRID_POSITION_X;
    int GRID_POSITION_Y;
    int GRID_WIDTH;
    int GRID_HEIGHT;
    uint64_t* map;
    
public:
    Map();
    virtual ~Map();
    void setPosition(int GRID_POSITION_X, int GRID_POSITION_Y, int GRID_WIDTH, int GRID_HEIGHT);
    
    virtual void reset();

    void draw(sf::RenderWindow* window);
    void drawCurrentBlock(sf::RenderWindow* window, CurrentBlock *curBlock);
    virtual void drawOutline(sf::RenderWindow* window);

    virtual uint8_t putBlockIntoMap(Block* block, int Y, int X);
    bool isValid(uint16_t shape, int Y, int X);
    bool isAllClear();
};

#endif