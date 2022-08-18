#ifndef PAINT_BASEPAINTTOOL_H
#define PAINT_BASEPAINTTOOL_H

#include "Constants.h"
#include "Canvas.h"
#include "ToolParameters.h"
#include <SDL.h>
#include <vector>

class BasePaintTool {
public:
    explicit BasePaintTool(const ToolParameters& toolParameters);
    virtual ~BasePaintTool() = default;

    void ChangeColour(const SDL_Colour& newColour);
    void ChooseNewSize(bool isIncrease);

    bool IsSameColours(const SDL_Colour& newColour) const;

    virtual void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangeCanvas) = 0;

protected:
    SDL_Colour _colour;
    uint32_t _normalSize;
    uint32_t _sizes[NUM_OF_PAINT_TOOL_SIZES]{};
    const int _yBeginningOfCanvas;

    void DrawPointInScale(SDL_Renderer* renderer, double scaleSize, const SDL_Point& point) const;
};

#endif //PAINT_BASEPAINTTOOL_H