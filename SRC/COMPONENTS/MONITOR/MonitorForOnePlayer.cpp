#include "MonitorForOnePlayer.hpp"

#include "Common.hpp"
#include "Hold.hpp"
#include "Map.hpp"
#include "Infor.hpp"
#include "CurrentBlock.hpp"
#include "LinkListBlock.hpp"

#include <iostream>

MonitorForOnePlayer::MonitorForOnePlayer(int x, int y):Monitor() {
    X_COORDINATE = x; Y_COORDINATE = y;
    CreateMonitor(x, y);
}

void MonitorForOnePlayer::CreateMonitor(int x, int y) {
    X_COORDINATE = x, Y_COORDINATE = y;
    hold = new Hold();
    map  = new Map();
    next = new LinkListBlock();
    infor = new Infor();
    setPosition(x, y);
}

void MonitorForOnePlayer::setPosition(int X_COORDINATE, int Y_COORDINATE) {
    int HOLD_WIDTH         = 5;
    int HOLD_HEIGHT        = 3;
    int HOLD_POSITION_X    = X_COORDINATE;
    int HOLD_POSITION_Y    = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int GRID_WIDTH         = 10;
    int GRID_HEIGHT        = 24;
    int GRID_POSITION_X    = HOLD_POSITION_X + HOLD_WIDTH * BLOCK_SIZE + BLOCK_SIZE + BLOCK_SIZE;
    int GRID_POSITION_Y    = Y_COORDINATE;
    
    int NEXT_WIDTH         = 5;
    int NEXT_HEIGHT        = 15;
    int NEXT_POSITION_X    = GRID_POSITION_X + GRID_WIDTH * BLOCK_SIZE + BLOCK_SIZE;
    int NEXT_POSITION_Y    = Y_COORDINATE + 5 * BLOCK_SIZE;
    
    int INFOR_WIDTH        = 5;
    int INFOR_POSITION_X   = HOLD_POSITION_X;
    int INFOR_POSITION_Y   = HOLD_POSITION_Y + HOLD_HEIGHT * BLOCK_SIZE + BLOCK_SIZE;

    int AUDIO_WIDTH        = 5;
    int AUDIO_HEIGHT       = 1;
    int AUDIO_POSITION_X   = GRID_POSITION_X + GRID_WIDTH  * BLOCK_SIZE + BLOCK_SIZE;
    int AUDIO_POSITION_Y   = GRID_POSITION_Y + GRID_HEIGHT * BLOCK_SIZE - BLOCK_SIZE;

    int GARBAGE_WIDTH = 1;
    int GARBAGE_HEIGHT = GRID_HEIGHT;
    int GARBAGE_POSITION_X = GRID_POSITION_X - BLOCK_SIZE;
    int GARBAGE_POSITION_Y = Y_COORDINATE + GRID_HEIGHT * BLOCK_SIZE;

    hold->setPosition(HOLD_POSITION_X, HOLD_POSITION_Y, HOLD_WIDTH, HOLD_HEIGHT);
    map->setPosition(GRID_POSITION_X, GRID_POSITION_Y, GRID_WIDTH, GRID_HEIGHT);
    next->setPosition(NEXT_POSITION_X, NEXT_POSITION_Y, NEXT_WIDTH, NEXT_HEIGHT);
    infor->setPosition(INFOR_POSITION_X, INFOR_POSITION_Y, INFOR_WIDTH * BLOCK_SIZE, AUDIO_POSITION_X, AUDIO_POSITION_Y, AUDIO_WIDTH, AUDIO_HEIGHT, GARBAGE_POSITION_X, GARBAGE_POSITION_Y, GARBAGE_WIDTH, GARBAGE_HEIGHT);
}