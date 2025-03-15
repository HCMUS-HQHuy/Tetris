#include "CurrentBlock.hpp"

#include "Common.hpp"
#include "Block.hpp"
#include "Map.hpp"
#include "Hold.hpp"

CurrentBlock::CurrentBlock() : block(nullptr) {}

CurrentBlock::CurrentBlock(Block *a) : block(a), posX(WIDTH / 2 - BLOCK_EDGE/2), posY(0){}

CurrentBlock::~CurrentBlock() {
    delete block; block = nullptr;
}

bool CurrentBlock::isEmpty() {
    return (block == nullptr);
}

void CurrentBlock::setter(Block* p) {
    block = p;
}

bool CurrentBlock::resetPosition(Map *map) {
    posX = WIDTH / 2 - BLOCK_EDGE / 2; posY = 0;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::moveDown(Map* map) {
    if (not map->isValid(block->getShape(), posY + 1, posX)) 
        return false; 

    posY++;

    return true;
}

bool CurrentBlock::moveLeft(Map* map) {
    if (not map->isValid(block->getShape(), posY, posX - 1)) 
        return false; 

    posX--;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::moveRight(Map* map) {
    if (not map->isValid(block->getShape(), posY, posX + 1)) 
        return false; 
    
    posX++;

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::hardDrop(Map *map) {
    posY = shadowPosY;

    return true;
}

void CurrentBlock::shadowHardDrop(Map *map) {
    shadowPosY = posY;
    while (map->isValid(block->getShape(), shadowPosY + 1, posX)) {
        shadowPosY++;
    }
}

bool CurrentBlock::collisionBottom(Map *map) {
    return not map->isValid(block->getShape(), posY + 1, posX);
}

bool CurrentBlock::rotateLeft(Map* map) {
    if (not isValid(block->getRotateLeft(), map)) {
        posY++;
        if (isValid(block->getRotateLeft(), map)) goto rotate_success;
        posY--;
        
        posY--;
        if (posY >= 0 and isValid(block->getRotateLeft(), map)) goto rotate_success;
        posY++;

        posX++;
        if (isValid(block->getRotateLeft(), map)) goto rotate_success;
        posX--;

        posX--;
        if (posX >= 0 and isValid(block->getRotateLeft(), map)) goto rotate_success;
        posX++;        

        if (block->getShapeID() == 0) {
            posY += 2;
            if (isValid(block->getRotateLeft() , map)) goto rotate_success;
            if (isValid(block->getRotateRight(), map)) goto rotate_success;
            posY -= 2;

            posY -= 2;
            if (posY >= 0 and isValid(block->getRotateLeft() , map)) goto rotate_success;
            if (posY >= 0 and isValid(block->getRotateRight(), map)) goto rotate_success;
            posY += 2;
            
            posX += 2;
            if (isValid(block->getRotateLeft() , map)) goto rotate_success;
            if (isValid(block->getRotateRight(), map)) goto rotate_success;
            posX -= 2;

            posX -= 2;
            if (posX >= 0 and isValid(block->getRotateLeft() , map)) goto rotate_success;
            if (posX >= 0 and isValid(block->getRotateRight(), map)) goto rotate_success;
            posX += 2;
        }

        return false;
    }

rotate_success:
    block->rotateLeft();

    shadowHardDrop(map);

    return true;
}

bool CurrentBlock::rotateRight(Map* map) {

    return true;
}

void CurrentBlock::swap(Hold* hold) {
    block = hold->interchange(block);
}

void CurrentBlock::draw(sf::RenderWindow *window, Map *map) {
    map->drawCurrentBlock(window, block, posY, shadowPosY, posX);
}

bool CurrentBlock::isValid(uint16_t shape, Map* map) {
    return map->isValid(shape, posY, posX);
}

uint8_t CurrentBlock::put(Map* map) {
    return map->update(block, posY, posX);
}

bool CurrentBlock::gameOver(Map* map) {
    return not map->isValid(block->getShape(), posY, posX);
}