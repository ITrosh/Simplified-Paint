#ifndef MAIN_CPP_MAINMENU_H
#define MAIN_CPP_MAINMENU_H

#include "Parser.h"
#include "Library.h"
#include "MainButton.h"

class MainMenu {
public:
    static MainMenu& GetInstance() {
        static MainMenu _instance;
        return _instance;
    }

    MainMenu(const MainMenu& other) = delete;
    MainMenu(MainMenu&& other) = delete;
    MainMenu& operator=(const MainMenu& other) = delete;
    MainMenu& operator=(MainMenu&& other) = delete;

    ErrorState Create(const std::string& filename, TTF_Font*& font, SDL_Renderer* renderer);
    MenuState Draw(SDL_Event* event, SDL_Renderer* renderer, std::string& name, SDL_Rect& popupMenuArea);

    void Free();

private:
    MainMenu() = default;
    ~MainMenu() = default;

    std::vector<MainButton> _buttons;
};

#endif //MAIN_CPP_MAINMENU_H