#ifndef PAINT_COLOURRECT_H
#define PAINT_COLOURRECT_H

#include <SDL.h>

class ColourRect {
public:
    ColourRect(const SDL_Rect& rect, const SDL_Colour& colour) : _rect(rect), _colour(colour) {}

    void HandleEvent(SDL_Event* event, SDL_Colour& colour);

    void Draw(SDL_Renderer* renderer);

private:
    bool _isPressed = false;
    SDL_Rect _rect;
    SDL_Colour _colour, _outlineColour = {255, 255, 255, 0};
};


#endif //PAINT_COLOURRECT_H
