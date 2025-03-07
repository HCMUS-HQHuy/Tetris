#ifndef BLOCK_HPP
#define BLOCK_HPP

// #include <ncurses.h>
#include <SFML/Graphics.hpp>
#include <cstdint>

class Block {
protected:
    uint16_t *state;
    uint8_t stateID;
    char symbol;
    int paddingX, paddingY;

public:
    Block();
    ~Block();

    void draw(sf::RenderWindow *window, int8_t y, int8_t x, int offsetY, int offsetX, bool padding);
    uint16_t getShape();
    uint16_t getRotateLeft();
    uint16_t getRotateRight();

    void rotateLeft();
    void rotateRight();

    void resetState();
};

class IBlock:public Block {
public:
    IBlock();
};

class LBlock:public Block {
public:
    LBlock();
};


class JBlock:public Block {
public:
    JBlock();
};


class OBlock:public Block {
public:
    OBlock();
};


class ZBlock:public Block {
public:
    ZBlock();
};


class SBlock:public Block {
public:
    SBlock();
};

class TBlock:public Block {
public:
    TBlock();
};

#endif