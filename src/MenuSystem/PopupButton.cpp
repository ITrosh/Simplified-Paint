#include "PopupButton.h"

using namespace std;

PopupButton::PopupButton(const std::string& name, SDL_Rect* rect, SDL_Texture* texture, int textureWidth)
        : IButton(name, rect, texture, textureWidth)
{
    _context = PopupButtonContext::DEFAULT;
}

MenuState PopupButton::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, string& name) {
    SDL_Point mouseCursor;
    SDL_GetMouseState(&mouseCursor.x, &mouseCursor.y);
    MenuState menuState = MenuState::Default;
    if (_context == PRESSED) {
        _currentState = ButtonState::Pressed;
        menuState = MenuState::Popup_Button_Pressed;
    } else if (SDL_PointInRect(&mouseCursor, &_rect)) {
        _currentState = ButtonState::Hover;
    } else {
        _currentState = ButtonState::Default;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN && IsInside(&mouseCursor) && event->button.button == SDL_BUTTON_LEFT) {
        if (_currentState == ButtonState::Hover) {
            _currentState = ButtonState::Pressed;
            _context = PRESSED;
            menuState = MenuState::Popup_Button_Pressed;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && IsInside(&mouseCursor) && event->button.button == SDL_BUTTON_LEFT) {
        if (_context == PRESSED) {
            _context = DEFAULT;
            name = _name;
            return MenuState::Popup_Button_Released;
        }
    }

    DrawButton(renderer);

    return menuState;
}

void PopupButton::DrawButton(SDL_Renderer* renderer) {
    switch (_currentState) {
        case ButtonState::Hover:
        {
            SDL_SetRenderDrawColor(renderer, 72, 110, 178, 0);
            SDL_RenderFillRect(renderer, &_rect);
            break;
        }
        case ButtonState::Pressed:
        {
            SDL_SetRenderDrawColor(renderer, 68, 148, 74, 0);
            SDL_RenderFillRect(renderer, &_rect);
            break;
        }
        default:
        {
            SDL_SetRenderDrawColor(renderer, 60, 63, 66, 0);
            SDL_RenderFillRect(renderer, &_rect);
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(renderer, _rect.x + _rect.w - 1, _rect.y, _rect.x + _rect.w - 1, _rect.y + _rect.h - 1);
    SDL_RenderCopy(renderer, _texture, nullptr, &_textureRect);
}

void PopupButton::Free() {
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

bool PopupButton::IsInside(const SDL_Point* mousePos) const {
    if (SDL_PointInRect(mousePos, &_rect)) {
        return true;
    }
    return false;
}