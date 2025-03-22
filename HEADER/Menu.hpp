#pragma once

#include <SFML/Graphics.hpp>

enum class STATUS_CODE {
    QUIT               = -1,

    SINGLEPLAYER       = 0, 
    VERSUSBOT          = 1, 
    MULTIPLAYER_SERVER = 2, 
    MULTIPLAYER_CLIENT = 3, 
    
    RESTART            = 0, 
    MENU               = 1, 
    
    RESUME             = 0
};

enum class MENU_CODE {
    MENU     = 0,
    ESCAPE   = 1,
    GAMEOVER = 2
};

class SoundManager;

class Menu {
private:
    sf::Font            font;
          
    SoundManager       *soundManager;

    bool                selected;
    int                 selectedItem;
    bool                mouseSelect;
    sf::Vector2i        mousePosPrev;

    int                 menuSize;
    sf::Text           *menuTexts;
    sf::RectangleShape *menuBars;

public:
    Menu(sf::RenderWindow *window, const std::string menuItems[], MENU_CODE menuCode);
    ~Menu();

    void processEvents(sf::RenderWindow *window, sf::Event event);

    void update(sf::RenderWindow *window, MENU_CODE menuCode);

    void draw(sf::RenderWindow *window, MENU_CODE menuCode);
};
