#include "Tetris.hpp"

#include "Player.hpp"

Tetris::Tetris() {
    int h, w;
    getmaxyx(stdscr, h, w);
    WINDOW *win = newwin(30, 35, h/2 - 25/2, w/2 - 30/2); // WINDOW * win = newwin(nlines, ncols, y0, x0);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    player = new Player(win);
}

Tetris::~Tetris() {
    delete player;
    delwin(win);
}

void Tetris::startGameOnePlayer() {
    player->start();
}