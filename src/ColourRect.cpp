#include "ColourRect.h"

void ColourRect::HandleEvent(SDL_Event* event, SDL_Colour& colour) {
    SDL_Point mouseCursor;
    SDL_GetMouseState(&mouseCursor.x, &mouseCursor.y);
    if (!SDL_PointInRect(&mouseCursor, &_rect)) {
        _outlineColour = {255, 255, 255, 0};
        if (_isPressed && event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
            _isPressed = false;
        }
    }
    else {
        _outlineColour = {0, 191, 255, 0};
        if (!_isPressed && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
            _isPressed = true;
        }
        if (_isPressed && event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
            _isPressed = false;
            colour = _colour;
        }
    }
}

void ColourRect::Draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
    SDL_RenderFillRect(renderer, &_rect);
    SDL_SetRenderDrawColor(renderer, _outlineColour.r, _outlineColour.g, _outlineColour.b, _outlineColour.a);
    SDL_RenderDrawRect(renderer, &_rect);
}