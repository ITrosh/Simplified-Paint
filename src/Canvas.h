#ifndef PAINT_CANVAS_H
#define PAINT_CANVAS_H

#include "Constants.h"
#include <SDL.h>
#include <vector>
#include <iostream>
#include <thread>

struct Fragment {
    int x, y;
    SDL_Colour colour;
};

class Canvas {
public:
    static std::vector<std::vector<Fragment>>& RefFragments() { return _fragments; }

    static void InitCanvas(int yBeginningOfCanvas);
    static bool IsPointInCanvas(int x, int y);
    static bool IsPointInCanvasPart(int x, int y, double scaleSize);
    static bool IsCanvasWhite();
    static void PaintSurface(SDL_Surface* surface, int canvasHeight);

    static void Redraw(SDL_Renderer* renderer, double scaleSize);
    static void RenderRectangle(SDL_Renderer* renderer, const SDL_Rect& rect, double scaleSize);
    static void Clear();
    static void FillCanvas(SDL_Surface* surface, int y0);

private:

    inline static std::vector<std::vector<Fragment>> _fragments;
    inline static int _yBeginningOfCanvas;

    static bool HasWhiteColour(const Fragment& fragment);
};


#endif //PAINT_CANVAS_H
