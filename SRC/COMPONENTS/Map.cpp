#include "Map.hpp"

#include "CurrentBlock.hpp"
#include "Block.hpp"

#define OFFSETX 4
#define OFFSETY 0
#define OFFRIGHT 12
#define NUMOFFSET 2
#define REALWIDTH 14
#define COLORWIDTH 4 // NUMBIT FOR COLOR -> pos[1] in ith line have color in bit 14th -> 17th

// **--------** REALWIDTH
// ** NUMOFFSET 
// **-------- OFFRIGHT

#define EMPTYLINE (FULLMASK(NUMOFFSET) ^ (FULLMASK(NUMOFFSET) << OFFRIGHT))

Map::Map() {
    for (int i = 0; i < HEIGHT; i++) map[i] = EMPTYLINE;
    map[HEIGHT] = FULLMASK(REALWIDTH);
}

Map::~Map() {}

void Map::drawOutline(sf::RenderWindow* window) {
    sf::RectangleShape line(sf::Vector2f(GRID_WIDTH * BLOCK_SIZE, 1));
    line.setFillColor(sf::Color(200, 200, 200, 150)); // Gray
    for (int i = 4; i <= GRID_HEIGHT; i++) {
        line.setPosition(GRID_POSITION_X, GRID_POSITION_Y + i * BLOCK_SIZE);
        window->draw(line);
    }

    line.setSize(sf::Vector2f(1, (GRID_HEIGHT - 4) * BLOCK_SIZE));
    for (int i = 0; i <= GRID_WIDTH; i++) {
        line.setPosition(GRID_POSITION_X + i * BLOCK_SIZE, 4 * BLOCK_SIZE + GRID_POSITION_Y);
        window->draw(line);
    }
}

void Map::remove(uint8_t pos) {
    for (int i = pos - 1; i >= 0; i--) 
        map[i + 1] = map[i];
    map[0] = EMPTYLINE;
}

// the function adds nlines to map 
// returns a value indicating whether the map is valid or not
bool Map::add(uint8_t nLines) {
    return false;
}

void Map::draw(sf::RenderWindow *window) {
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    block.setFillColor(sf::Color::White); 
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) if (getBit(map[i], j + NUMOFFSET)) {
            block.setPosition(GRID_POSITION_X + j * BLOCK_SIZE + 1, GRID_POSITION_Y + i * BLOCK_SIZE + 1);
            window->draw(block);
        }
    }
}

uint8_t Map::update(Block* block, int Y, int X) {
    uint8_t cnt = 0;
    uint16_t shape = block->getShape();
    uint8_t color = block->getShapeID();

    for (int i = 0; i < BLOCK_EDGE; i++) if (Y + i < HEIGHT) {
        map[Y + i] ^= (getLine(shape, i) << (X + NUMOFFSET));
        if (((map[Y + i] & FULLMASK(REALWIDTH)) ^ FULLMASK(REALWIDTH)) == 0) {
            remove(Y + i);
            cnt++;
        }
        else {
            map[Y + i] ^= (getLine(shape, i) << (X + REALWIDTH));
        }
    }
    return cnt;
}

bool Map::isValid(uint16_t shape, int Y, int X) {
    if (X < -NUMOFFSET || X + BLOCK_EDGE > WIDTH + NUMOFFSET) return false;

    for (int i = 0; i < BLOCK_EDGE; i++)
        if (((getMask(map[i + Y], X + NUMOFFSET) ^ getLine(shape, i)) & getMask(map[i + Y], X + NUMOFFSET)) != getMask(map[i + Y], X + NUMOFFSET))
            return false;
    return true;
}