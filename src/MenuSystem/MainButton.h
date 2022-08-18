#ifndef MAIN_CPP_MAINBUTTON_H
#define MAIN_CPP_MAINBUTTON_H

#include "PopupMenu.h"

enum class MainButtonContext {
    DEFAULT,
    FIRST_PRESSED,
    FIRST_RELEASED,
    SECOND_PRESSED,
};

class MainButton: public IButton {
public:
    MainButton(const std::string& name, SDL_Rect* rect, SDL_Texture* texture, int textureWidth, PopupMenu popupMenu);

    MenuState HandleEvent(SDL_Event* event, SDL_Renderer* renderer, std::string& name) override;

    void CalculatePopupMenuArea(SDL_Rect& popupMenuArea);
    void DrawButton(SDL_Renderer* renderer);
    void Free();

private:
    PopupMenu _popupMenu;
    MainButtonContext _context;
};

#endif //MAIN_CPP_MAINBUTTON_H