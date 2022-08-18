#include "MainMenu.h"

using namespace std;

ErrorState MainMenu::Create(const std::string& filename, TTF_Font*& font, SDL_Renderer* renderer) {
    dictionary dict;

    ErrorState errorState = Parser::TrySyntaxParse(filename, dict);
    if (errorState != ErrorState::SUCCESS) {
        return errorState;
    }

    auto it = dict.begin(), end = dict.end();
    vector<vector<PopupButtonParameters>> popupMenus;
    int tempXOffset = 0, i = 0;
    int popupMenuWidth = 0;

    while (it != end) {
        SDL_Texture* texture = nullptr;
        SDL_Rect tempRect;
        errorState = Library::CreateTextureFromText(font, renderer, it->first, {187, 187, 187}, tempRect, texture);
        if (errorState != ErrorState::SUCCESS) {
            return errorState;
        }
        popupMenus.emplace_back();
        auto popupMenuIt = it->second.begin();
        while (popupMenuIt != it->second.end()) {
            SDL_Texture* popupButtonTexture = nullptr;
            SDL_Rect popupButtonRect;
            errorState = Library::CreateTextureFromText(font, renderer, *popupMenuIt, {187, 187, 187}, popupButtonRect, popupButtonTexture);
            if (errorState != ErrorState::SUCCESS) {
                return errorState;
            }

            if (popupMenuWidth < popupButtonRect.w)
                popupMenuWidth = popupButtonRect.w;

            PopupButtonParameters popupButton = {popupButtonTexture, *popupMenuIt, popupButtonRect.w};
            popupMenus[i].push_back(move(popupButton));
            popupMenuIt++;
        }
        SDL_Rect rect = {tempXOffset, 0, tempRect.w, BUTTON_HEIGHT};
        MainButton temp(it->first, &rect, texture, tempRect.w,
                        PopupMenu(popupMenus[i], tempXOffset, rect.h + INCREASE_BUTTON_SIZE, popupMenuWidth));
        _buttons.push_back(move(temp));
        tempXOffset += rect.w + INCREASE_BUTTON_SIZE;
        it++;
        i++;
    }

    return ErrorState::SUCCESS;
}

MenuState MainMenu::Draw(SDL_Event* event, SDL_Renderer* renderer, string& name, SDL_Rect& popupMenuArea) {
    MenuState menuState = MenuState::Default;
    for (auto& button : _buttons) {
        menuState = button.HandleEvent(event, renderer, name);
        if (menuState != MenuState::Default) {
            if (menuState == MenuState::Main_Button_Released || menuState == MenuState::Popup_Button_Released) {
                button.CalculatePopupMenuArea(popupMenuArea);
            }
            return menuState;
        }
    }

    return menuState;
}

void MainMenu::Free() {
    for (auto& button : _buttons)
        button.Free();
}