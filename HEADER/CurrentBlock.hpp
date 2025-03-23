#ifndef CURRENTBLOCK_HPP

#define CURRENTBLOCK_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

class Map;
class Block;
class Hold;

class CurrentBlock {
private:
    Block *block;
    int8_t posX, posY;
    int8_t shadowPosY;
public:
    CurrentBlock();
    CurrentBlock(Block *p);
    ~CurrentBlock();

    bool isEmpty();
    
    Block* setter(Block* p);
    void setPosition(int posX, int posY, int shadowPosy);
    void freeAndSetter(Block* p);

    void swap(Hold* hold);

    bool isValid(Map* map) const;

    uint8_t putIntoMap(Map* map);

    void draw(sf::RenderWindow* window, Map* map) const;
    // void compress(sf::Packet &packet);
    // void compressWithSpin(sf::Packet &packet);

    friend class CurrentBlockController;
};

#endif