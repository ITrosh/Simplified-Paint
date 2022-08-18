#include "MainButton.h"

using namespace std;

MainButton::MainButton(const string& name, SDL_Rect* rect, SDL_Texture* texture, int textureWidth, PopupMenu popupMenu)
            : IButton(name, rect, texture, textureWidth), _popupMenu(move(popupMenu))
{
    _context = MainButtonContext::DEFAULT;
}

 MenuState MainButton::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, string& name) {
    SDL_Point mouseCursor;
    SDL_GetMouseState(&mouseCursor.x, &mouseCursor.y);
    MenuState mainButtonState = MenuState::Default;
    if (_context == MainButtonContext::FIRST_PRESSED || _context == MainButtonContext::FIRST_RELEASED) {
        _currentState = ButtonState::Pressed;
        mainButtonState = MenuState::Main_Button_Pressed;
    } else if (SDL_PointInRect(&mouseCursor, &_rect)) {
        _currentState = ButtonState::Hover;
    } else {
        _currentState = ButtonState::Default;
    }

    if (_context == MainButtonContext::SECOND_PRESSED)
        mainButtonState = MenuState::Main_Button_Pressed;

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (_context == MainButtonContext::DEFAULT && _currentState == ButtonState::Hover) {
            _currentState = ButtonState::Pressed;
            _context = MainButtonContext::FIRST_PRESSED;
            mainButtonState = MenuState::Main_Button_Pressed;
        }
        if (_context == MainButtonContext::FIRST_RELEASED && !_popupMenu.IsInside(&mouseCursor)) {
            if (SDL_PointInRect(&mouseCursor, &_rect))
                _currentState = ButtonState::Hover;
            else
                _currentState = ButtonState::Default;
            _context = MainButtonContext::SECOND_PRESSED;
            mainButtonState = MenuState::Main_Button_Pressed;
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        if (_context == MainButtonContext::FIRST_PRESSED) {
            _context = MainButtonContext::FIRST_RELEASED;
            mainButtonState = MenuState::Main_Button_Pressed;
        }
        if (_context == MainButtonContext::SECOND_PRESSED && !_popupMenu.IsInside(&mouseCursor)) {
            _context = MainButtonContext::DEFAULT;
            mainButtonState = MenuState::Main_Button_Released;
        }
    }

    DrawButton(renderer);
    if (_currentState == ButtonState::Pressed) {
        if (_popupMenu.Draw(event, renderer, name) == MenuState::Popup_Button_Released) {
            _context = MainButtonContext::DEFAULT;
            _currentState = ButtonState::Default;
            return MenuState::Popup_Button_Released;
        } else if (_popupMenu.Draw(event, renderer, name) == MenuState::Popup_Button_Pressed) {
            return MenuState::Popup_Button_Pressed;
        }
    }

    return mainButtonState;
}

void MainButton::DrawButton(SDL_Renderer* renderer) {
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

void MainButton::Free() {
    _popupMenu.Free();
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

void MainButton::CalculatePopupMenuArea(SDL_Rect& popupMenuArea) {
    popupMenuArea.x = _rect.x;
    popupMenuArea.y = _rect.y + _rect.h;
    _popupMenu.CalculatePopupMenuDimensions(popupMenuArea.w, popupMenuArea.h);
}
