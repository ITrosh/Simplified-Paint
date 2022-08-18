#include "PopupMenu.h"

using namespace std;

PopupMenu::PopupMenu(const vector<PopupButtonParameters>& popupButtons,
                     int xOffset, int yOffset, int popupMenuWidth) : _popupMenuWidth(popupMenuWidth)
{
    int tempYOffset = yOffset;
    for (const auto& button : popupButtons) {
        SDL_Rect rect = {xOffset, tempYOffset, popupMenuWidth, BUTTON_HEIGHT};
        tempYOffset += BUTTON_HEIGHT + INCREASE_BUTTON_SIZE;

        PopupButton temp(button.name, &rect, button.texture, button.popupButtonWidth);
        _popupButtons.push_back(move(temp));
    }
}

MenuState PopupMenu::Draw(SDL_Event* event, SDL_Renderer* renderer, string& name) {
    MenuState popupButtonState = MenuState::Default;
    for (auto& popupButton : _popupButtons) {
        popupButtonState = popupButton.HandleEvent(event, renderer, name);
        if (popupButtonState == MenuState::Popup_Button_Released || popupButtonState == MenuState::Popup_Button_Pressed)
            return popupButtonState;
    }
    return popupButtonState;
}
bool PopupMenu::IsInside(const SDL_Point* mousePos) const {
    for (const auto& button : _popupButtons) {
        if (button.IsInside(mousePos))
            return true;
    }
    return false;
}
void PopupMenu::Free() {
    for (auto& button : _popupButtons)
        button.Free();
}

void PopupMenu::CalculatePopupMenuDimensions(int& width, int& height) {
    width = _popupMenuWidth + INCREASE_BUTTON_SIZE;
    height = (int)(_popupButtons.size() + 1) * (BUTTON_HEIGHT + INCREASE_BUTTON_SIZE);
}
