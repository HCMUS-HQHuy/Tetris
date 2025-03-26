#ifndef CURRENTBLOCK_HPP

#define CURRENTBLOCK_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

class Map;
class Block;
class Hold;

class CurrentBlock {
protected:
    Block *block;
    int8_t posX, posY;
    int8_t shadowPosY;

    bool spin; // check piece spin
public:
    CurrentBlock();
    CurrentBlock(Block *p);
    ~CurrentBlock();

    bool isJustSpin();
    void setSpin();
    void resetSpin();
    
    bool isEmpty();
    
    Block* setter(Block* p);
    void setState(int stateID, int posX, int posY, int sh);

    void freeAndSetter(Block* p);

    void swap(Hold* hold);
    bool isValid(Map* map) const;
    uint8_t putIntoMap(Map* map);
    void draw(sf::RenderWindow* window, int GRID_POSITION_Y, int GRID_POSITION_X);

    // void compress(sf::Packet &packet);
    // void compressWithSpin(sf::Packet &packet);

    friend class CurrentBlockController;
};

#endif