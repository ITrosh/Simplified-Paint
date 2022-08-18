#include "Tools.h"

using namespace std;

void Tool::CalculateNormalScaledPoint(double scaleSize, SDL_Point& point) {
    if (scaleSize > SCALE_TYPE_3) {
        point.x = (int) (point.x / scaleSize);
        point.y = (int) ((point.y - _yBeginningOfCanvas) / scaleSize) + _yBeginningOfCanvas;
    } else if (scaleSize < SCALE_TYPE_3) {
        point.x = (int) (point.x * (1 / scaleSize));
        point.y = (int) ((point.y - _yBeginningOfCanvas) * (1 / scaleSize)) + _yBeginningOfCanvas;
    }

    point.x -= (int)_normalSize / 2;
    point.y -= (int)_normalSize / 2;
}

void Tool::Draw(SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
    SDL_Point mouseCursor;
    Uint32 mouseState = SDL_GetMouseState(&mouseCursor.x, &mouseCursor.y);

    if ((mouseState & SDL_BUTTON_LMASK) != 0 && Canvas::IsPointInCanvas(mouseCursor.x, mouseCursor.y)) {
        isChangedCanvas = true;
        SDL_Point point = {mouseCursor.x, mouseCursor.y};
        CalculateNormalScaledPoint(scaleSize, point);

        for (int i = point.x; i < point.x + _normalSize; i++) {
            for (int j = point.y; j < point.y + _normalSize; j++) {
                if (Canvas::IsPointInCanvas(i, j)) {
                    Canvas::RefFragments()[i][j - _yBeginningOfCanvas].colour = _colour;
                    DrawPointInScale(renderer, scaleSize, {i, j});
                }
            }
        }
    }
}

void Pencil::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    Draw(renderer, scaleSize, isChangedCanvas);
}

void Eraser::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    Draw(renderer, scaleSize, isChangedCanvas);
}