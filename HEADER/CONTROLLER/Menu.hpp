#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class STATUS_CODE { 
    SINGLEPLAYER,
    MULTIPLAYER,
    OPTION,
         
    PRACTICE,
    VERSUSBOT,

    MULTIPLAYER_SERVER,
    MULTIPLAYER_CLIENT,
    
    RESUME,

    RESTART,
    MENU,

    BACK,

    QUIT
};

enum class MENU_CODE {
    MAIN,
    SINGLEPLAYER,
    MULTIPLAYER,

    OPTION,

    PAUSE,

    GAMEOVER
};

class Menu;
class SoundManager;
class KeyConfiguration;

class Menu {
private:
    sf::Font            font;
    
    KeyConfiguration   *keyConfiguration;

    sf::Clock           selectedTimeout;

    SoundManager       *soundManager;

    bool                optionSelected;
    bool                audioSelected;
    int                 optionSelectedIndex;
    int                 optionSelectedItem;
    std::string         optionKeyStrings[2];
    sf::Text            optionKeyTexts[2];

    bool                optionWaitForKey;
    bool                optionWaitForReleaseKey;

    bool                selected;
    int                 selectedItem;
    bool                mouseSelect;
    sf::Vector2i        mousePosPrev;

    int                 menuSize;
    sf::Text           *menuTexts;
    
    sf::Text            menuTitle;
    sf::RectangleShape  menuTitleBar;

    sf::Text            footerVersion;
    sf::Text            footerOwner;
    sf::RectangleShape  footerBar;

    sf::RectangleShape *menuBars;
    sf::VertexArray    *gradient;
    float              *originalPositionX;
    float              *currentBarPositionX;
    float              *targetBarPositionX;

    MENU_CODE           menuCode;

    std::vector<Menu *> subMenus;

public:
    Menu(sf::RenderWindow *window, const std::vector<std::string> &menuItems, MENU_CODE menuCode);
    ~Menu();

    bool notSelected();

    STATUS_CODE getSelectedItem();

    void processEvents(sf::RenderWindow *window, sf::Event event);

    void update(sf::RenderWindow *window);

    void draw(sf::RenderWindow *window);

    Menu * getSubMenu(MENU_CODE menuCode);
};
