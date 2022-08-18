#ifndef PAINT_ICON_H
#define PAINT_ICON_H

#include "TextureParameters.h"
#include "Constants.h"
#include <SDL.h>
#include <iostream>

enum class IconName {
    PENCIL,
    ERASER,
    LINE,
    CIRCLE,
    RECTANGLE,
    INCREASE,
    DECREASE,
};

enum class IconState {
    DEFAULT,
    HOVER,
    PRESSED,
};

enum class IconContext {
    DEFAULT,
    PRESSED,
    CHOSEN,
};

class Icon {
public:
    Icon(const IconName& name, const TextureParameters& textureParameters);

    void HandleIconEvent(SDL_Event* event, IconName& name, bool& isButtonDown);

    void DrawIcon(SDL_Renderer* renderer);
    void BecomeDefault();

    bool IsPointInIcon(const SDL_Point& point) const;

private:
    SDL_Rect _rect{}, _textureRect{};
    SDL_Texture* _texture;
    IconName _name;

    IconContext _context = IconContext::DEFAULT;
    IconState _state = IconState::DEFAULT;
};

#endif //PAINT_ICON_H