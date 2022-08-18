#ifndef MAIN_CPP_POPUPMENU_H
#define MAIN_CPP_POPUPMENU_H

#include "Library.h"
#include "PopupButtonParameters.h"
#include "PopupButton.h"
#include <vector>

class PopupMenu {
public:
    explicit PopupMenu(const std::vector<PopupButtonParameters>& popupButtons,
                       int xOffset, int yOffset, int popupMenuWidth);

    bool IsInside(const SDL_Point* mousePos) const;
    MenuState Draw(SDL_Event* event, SDL_Renderer* renderer, std::string& name);

    void CalculatePopupMenuDimensions(int& width, int& height);

    void Free();

private:
    std::vector<PopupButton> _popupButtons;
    int _popupMenuWidth;
};

#endif //MAIN_CPP_POPUPMENU_H