#ifndef PAINT_FIGURES_H
#define PAINT_FIGURES_H

#include "BasePaintTool.h"
#include "Canvas.h"
#include <SDL2_gfxPrimitives.h>

class Figure : public BasePaintTool {
public:
    explicit Figure(const ToolParameters& toolParameters) : BasePaintTool(toolParameters) {}

protected:
    SDL_Point _begin{}, _end{};
    bool _isReadyToDraw = false;

    void PrepareRectForRedraw(SDL_Point& begin, SDL_Point& end);
    void RedrawRectArea(SDL_Renderer* renderer, double scaleSize);
    void CalculateScaledNormalEndPoint(double scaleSize);
};

class Line : public Figure {
public:
    explicit Line(const ToolParameters& toolParameters) : Figure(toolParameters) {}
    virtual ~Line() {}

    void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) override;

private:
    void DrawSimpleLine(SDL_Renderer* renderer, double scaleSize);
    void InsertInCanvas(SDL_Renderer* renderer, double scaleSize);
};

class Rectangle : public Figure {
public:
    explicit Rectangle(const ToolParameters& toolParameters) : Figure(toolParameters) {}
    virtual ~Rectangle() {}

    void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) override;

private:
    void Draw(SDL_Renderer* renderer, double scaleSize);
};

class Circle : public Figure {
public:
    explicit Circle(const ToolParameters& toolParameters) : Figure(toolParameters) {}
    virtual ~Circle() {}

    void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) override;

private:
    int DrawCircle(SDL_Renderer* renderer, int x, int y, int rad, Uint8 r,
                   Uint8 g, Uint8 b, Uint8 a, bool isFilled, double scaleSize);
    int VerticalLine(SDL_Renderer* renderer, int x, int y1, int y2,
                     Uint8 r, Uint8 g, Uint8 b, Uint8 a, double scaleSize);

    void Draw(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, double scaleSize);
};

#endif //PAINT_FIGURES_H
