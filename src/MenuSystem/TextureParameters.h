#ifndef PAINT_TEXTUREPARAMETERS_H
#define PAINT_TEXTUREPARAMETERS_H

#include <SDL.h>

struct TextureParameters {
    SDL_Rect icon, textureRect;
    SDL_Texture* texture;
};

#endif //PAINT_TEXTUREPARAMETERS_H
