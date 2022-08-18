#ifndef MAIN_CPP_POPUPBUTTON_H
#define MAIN_CPP_POPUPBUTTON_H

#include "IButton.h"
#include <iostream>

enum PopupButtonContext {
    DEFAULT = 0,
    PRESSED = 1,
};

class PopupButton: public IButton {
public:
    explicit PopupButton(const std::string& name, SDL_Rect* rect, SDL_Texture* texture, int textureWidth);

    MenuState HandleEvent(SDL_Event* event, SDL_Renderer* renderer, std::string& name) override;

    void DrawButton(SDL_Renderer* renderer);

    bool IsInside(const SDL_Point* mousePos) const;

    void Free();

private:
    PopupButtonContext _context;
};

#endif //MAIN_CPP_POPUPBUTTON_H