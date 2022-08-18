#ifndef PAINT_LIBRARY_H
#define PAINT_LIBRARY_H

#include "Constants.h"
#include "ErrorState.h"
#include "TextureParameters.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Library {
public:
    static ErrorState Init(SDL_Window*& window, SDL_Renderer*& renderer,
                           TTF_Font*& menuFont, TTF_Font*& inscriptionFont);

    static ErrorState CreateTextureFromText(TTF_Font* font, SDL_Renderer* renderer, const std::string& name,
                                            const SDL_Color& textColor, SDL_Rect& rect, SDL_Texture*& texture);

    static void Dispose(ErrorState rc, SDL_Window*& window, SDL_Renderer*& renderer,
                        TTF_Font*& menuFont, TTF_Font*& inscriptionFont);

    static ErrorState Load(const char* imageFile, SDL_Renderer*& renderer, SDL_Texture*& texture);
    static ErrorState CreateTexture(SDL_Renderer* renderer, SDL_Surface*& surface, SDL_Texture*& texture);
};

#endif //PAINT_LIBRARY_H