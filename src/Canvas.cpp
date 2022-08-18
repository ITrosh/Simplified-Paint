#include "Canvas.h"

using namespace std;

void Canvas::InitCanvas(int yBeginningOfCanvas) {
    static bool isInitialized = false;

    _yBeginningOfCanvas = yBeginningOfCanvas;

    if (isInitialized)
        throw runtime_error("Already init");

    SDL_Colour colour = {255, 255, 255, 255};

    for (int i = 0; i < SCREEN_WIDTH; i++) {
        _fragments.emplace_back();
        for (int j = yBeginningOfCanvas; j < SCREEN_HEIGHT; j++) {
            _fragments[i].push_back({i, j, colour});
        }
    }

    isInitialized = true;
}

bool Canvas::IsPointInCanvas(int x, int y) {
    return (x >= 0 && x < SCREEN_WIDTH && y >= _fragments[0][0].y && y < SCREEN_HEIGHT);
}

void Canvas::Redraw(SDL_Renderer* renderer, double scaleSize) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (const auto& fragmentRow : _fragments) {
        for (const auto& fragment : fragmentRow) {
            if (!HasWhiteColour(fragment)) {
                SDL_SetRenderDrawColor(renderer, fragment.colour.r, fragment.colour.g,
                                       fragment.colour.b, fragment.colour.a);
                //cout << fragment.x << " " << fragment.y << endl;
                int newX = (int)(fragment.x * scaleSize);
                int newY = (int)((fragment.y - _yBeginningOfCanvas) * scaleSize);
                //cout << newX << " " << newY << endl;
                if (scaleSize > SCALE_TYPE_3) {
                    for (size_t col = 0; col < (size_t)scaleSize; col++) {
                        for (size_t row = 0; row < (size_t)scaleSize; row++) {
                            if (IsPointInCanvas(newX, newY + _yBeginningOfCanvas))
                                SDL_RenderDrawPoint(renderer, newX, newY + _yBeginningOfCanvas);
                            newY++;
                        }
                        newY = (int)((fragment.y - _yBeginningOfCanvas) * scaleSize);
                        newX++;
                    }
                }
                else {
                    if (IsPointInCanvas(newX, newY + _yBeginningOfCanvas))
                        SDL_RenderDrawPoint(renderer, newX, newY + _yBeginningOfCanvas);
                }
            }
        }
    }
}

void Canvas::RenderRectangle(SDL_Renderer* renderer, const SDL_Rect& rect, double scaleSize) {
    for (int i = rect.x; i < rect.x + rect.w; i++) {
        for (int j = rect.y - _yBeginningOfCanvas; j < rect.y - _yBeginningOfCanvas + rect.h; j++) {
            if (!HasWhiteColour(_fragments[i][j])) {
                SDL_SetRenderDrawColor(renderer, _fragments[i][j].colour.r, _fragments[i][j].colour.g,
                                       _fragments[i][j].colour.b, _fragments[i][j].colour.a);

                int newX = (int)(_fragments[i][j].x * scaleSize);
                int newY = (int)((_fragments[i][j].y - _yBeginningOfCanvas) * scaleSize);
                //cout << newX << " " << newY << endl;
                if (scaleSize > SCALE_TYPE_3) {
                    for (size_t col = 0; col < (size_t)scaleSize; col++) {
                        for (size_t row = 0; row < (size_t)scaleSize; row++) {
                            if (IsPointInCanvas(newX, newY + _yBeginningOfCanvas))
                                SDL_RenderDrawPoint(renderer, newX, newY + _yBeginningOfCanvas);
                            newY++;
                        }
                        newY = (int)((_fragments[i][j].y - _yBeginningOfCanvas) * scaleSize);
                        newX++;
                    }
                }
                else {
                    if (IsPointInCanvas(newX, newY + _yBeginningOfCanvas))
                        SDL_RenderDrawPoint(renderer, newX, newY + _yBeginningOfCanvas);
                }
            }
        }
    }
}

bool Canvas::HasWhiteColour(const Fragment& fragment) {
    if (fragment.colour.r == 255 && fragment.colour.g == 255 && fragment.colour.b == 255 && fragment.colour.a == 255)
        return true;
    else
        return false;
}

bool Canvas::IsCanvasWhite() {
    for (const auto& fragmentRow : _fragments) {
        for (const auto& fragment: fragmentRow) {
            if (!HasWhiteColour(fragment))
                return false;
        }
    }

    return true;
}

void Canvas::PaintSurface(SDL_Surface* surface, int canvasHeight) {
    int bpp = surface->format->BytesPerPixel;
    for (size_t i = 0; i < SCREEN_WIDTH; i++) {
        for (size_t j = 0; j < SCREEN_HEIGHT - canvasHeight; j++) {
            auto* surfacePixel = (Uint32*)((Uint8*)surface->pixels + j * surface->pitch  + i * bpp);
            *surfacePixel = SDL_MapRGBA(surface->format, _fragments[i][j].colour.r, _fragments[i][j].colour.g,
                                       _fragments[i][j].colour.b, _fragments[i][j].colour.a);
        }
    }
}

void Canvas::Clear() {
    for (auto& fragmentRow : _fragments) {
        for (auto& fragment : fragmentRow) {
            fragment.colour.r = 255;
            fragment.colour.g = 255;
            fragment.colour.b = 255;
            fragment.colour.a = 255;
        }
    }
}

void Canvas::FillCanvas(SDL_Surface* surface, int y0) {
    int bpp = surface->format->BytesPerPixel;
    for (size_t i = 0; i < surface->w; i++) {
        for (size_t j = y0; j < surface->h; j++) {
            Uint32 surfacePixel = *(Uint32*)((Uint8*)surface->pixels + j * surface->pitch  + i * bpp);
            SDL_Colour tempColour;
            SDL_GetRGBA(surfacePixel, surface->format, &tempColour.r, &tempColour.g, &tempColour.b, &tempColour.a);
            _fragments[i][j].colour = tempColour;
        }
    }
}

bool Canvas::IsPointInCanvasPart(int x, int y, double scaleSize) {
    SDL_Rect rect = {0, _yBeginningOfCanvas, (int)(SCREEN_WIDTH * scaleSize),
                     (int)((SCREEN_HEIGHT - _yBeginningOfCanvas) * scaleSize)};

    SDL_Point point = {x, y};
    return SDL_PointInRect(&point, &rect);
}
