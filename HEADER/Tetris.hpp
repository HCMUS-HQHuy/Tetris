#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <SFML/Graphics.hpp>

class Player;

class Tetris {
private:
    Player* player;
    sf::RenderWindow *window;
public:
    Tetris();
    ~Tetris();
    void startGameOnePlayer();
};
#endif