#include "CurrentBlock.hpp"

#include <iostream>

#include "CommonMap.hpp"
#include "CurrentBlock.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Hold.hpp"
#include "SoundManager.hpp"

CurrentBlock::CurrentBlock() : block(nullptr){}

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH_MAP / 2 - BLOCK_EDGE/2), posY(0), spin(false){}

CurrentBlock::~CurrentBlock() { delete block; block = nullptr; }

bool CurrentBlock::isEmpty() { return (block == nullptr); }

bool CurrentBlock::isJustSpin() { return spin; }
void CurrentBlock::setSpin() { spin = true;}
void CurrentBlock::resetSpin() { spin = false; }

Block* CurrentBlock::setter(Block* p) {
    Block* tmp = block; block = p; spin = false;
    return tmp;
}

void CurrentBlock::freeAndSetter(Block* p) {
    delete block; block = p; spin = false;
}

void CurrentBlock::setState(int stateID, int x, int y, int sh) {
    block->setStateID(stateID); posX = x; posY = y; shadowPosY = sh;
}

void CurrentBlock::swap(Hold* hold) {
    block = hold->interchange(block);
}

bool CurrentBlock::isValid(Map* map) const {
    return map->isValid(block->getShape(), posY, posX);
}

uint8_t CurrentBlock::putIntoMap(Map* map) {
    return map->putBlockIntoMap(block, posY, posX);;
}

void CurrentBlock::draw(sf::RenderWindow* window, int GRID_POSITION_Y, int GRID_POSITION_X) {
    block->drawGhost(window, shadowPosY, posX, GRID_POSITION_Y, GRID_POSITION_X);
    block->draw(window, posY, posX, GRID_POSITION_Y, GRID_POSITION_X);
}

char CurrentBlock::getTypeBlock() {
    switch (block->getShapeID()) {
        case 1:
            return 'I';
        case 2:
            return 'L';
        case 3:
            return 'J';
        case 4:
            return 'O';
        case 5:
            return 'Z';
        case 6:
            return 'S';
        case 7:
            return 'T';
        default:
            throw std::invalid_argument("Invalid getTypeBlock()");
    }

    return char();
}

void CurrentBlock::resetPosition(Map* map) {
    posX = WIDTH_MAP / 2 - BLOCK_EDGE / 2; posY = 0;
    updateShadow(map);
}

void CurrentBlock::updateShadow(Map* map) {

    uint16_t shape = block->getShape();
    shadowPosY = posY;
    while (map->isValid(shape, shadowPosY + 1, posX)) {
        shadowPosY++;
    }
}