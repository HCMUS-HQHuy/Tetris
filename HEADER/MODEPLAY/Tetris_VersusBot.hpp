#ifndef TETRIS_VS_BOT_HPP
#define TETRIS_VS_BOT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

#include "Tetris.hpp"

class Player_VersusBot;
class Bot;

class Tetris_VersusBot: public Tetris {
private:
    int PLAYER_X_COORDINATE;
    int PLAYER_Y_COORDINATE;
    int COMPETITOR_X_COORDINATE;
    int COMPETITOR_Y_COORDINATE;

    Player_VersusBot* player;
    Bot* competitor;

    std::mt19937 gen;
public:
    Tetris_VersusBot(sf::RenderWindow*, Scene*);
    ~Tetris_VersusBot();
    STATUS_CODE start();
};

#endif // TETRIS_VS_BOT_HPP