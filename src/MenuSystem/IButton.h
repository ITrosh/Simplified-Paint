#ifndef MAIN_CPP_IBUTTON_H
#define MAIN_CPP_IBUTTON_H

#include "Constants.h"
#include <SDL.h>
#include <string>

enum class ButtonState {
    Default,
    Hover,
    Pressed,
};

enum class MenuState {
    Main_Button_Pressed,
    Main_Button_Released,
    Popup_Button_Pressed,
    Popup_Button_Released,
    Default,
};

class IButton {
public:
    explicit IButton(const std::string& name, SDL_Rect* rect, SDL_Texture* texture, int textureWidth) {
        _name = name;
        _rect.x = rect->x;
        _rect.y = rect->y;
        _rect.w = rect->w + INCREASE_BUTTON_SIZE;
        _rect.h = rect->h + INCREASE_BUTTON_SIZE;
        _textureRect.x = rect->x + INCREASE_BUTTON_SIZE / 2;
        _textureRect.y = rect->y + INCREASE_BUTTON_SIZE / 2;
        _textureRect.w = textureWidth;
        _textureRect.h = rect->h;
        _texture = texture;
        _currentState = ButtonState::Default;
    }

    virtual MenuState HandleEvent(SDL_Event* event, SDL_Renderer* renderer, std::string& name) = 0;

protected:

    std::string _name;
    SDL_Rect _rect{}, _textureRect{};
    SDL_Texture* _texture;
    ButtonState _currentState;
};

#endif //MAIN_CPP_IBUTTON_H