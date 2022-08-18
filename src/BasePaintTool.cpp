#include "BasePaintTool.h"

using namespace std;

BasePaintTool::BasePaintTool(const ToolParameters& toolParameters)
        : _colour(toolParameters.colour), _normalSize(toolParameters.normalSize),
        _yBeginningOfCanvas(toolParameters.yBeginningOfCanvas)
{
    for (unsigned i = 0; i < NUM_OF_PAINT_TOOL_SIZES; i++) {
        _sizes[i] = toolParameters.sizes[i];
    }
}

void BasePaintTool::ChangeColour(const SDL_Colour& newColour) {
    _colour = newColour;
}

void BasePaintTool::ChooseNewSize(bool isIncrease) {
    if (isIncrease) {
        if (_normalSize == _sizes[NUM_OF_PAINT_TOOL_SIZES - 1])
            return;
        for (size_t i = 0; i < NUM_OF_PAINT_TOOL_SIZES; i++) {
            if (_normalSize == _sizes[i] && i < NUM_OF_PAINT_TOOL_SIZES - 1) {
                _normalSize = _sizes[i + 1];
                return;
            }
        }
    }
    else {
        if (_normalSize == _sizes[0])
            return;
        for (size_t i = NUM_OF_PAINT_TOOL_SIZES - 1; i >= 0; i--) {
            if (_normalSize == _sizes[i] && i > 0) {
                _normalSize = _sizes[i - 1];
                return;
            }
        }
    }
}

bool BasePaintTool::IsSameColours(const SDL_Color& newColour) const {
    if (newColour.r != _colour.r || newColour.g != _colour.g ||
        newColour.b != _colour.b || newColour.a != _colour.a)
        return false;
    else
        return true;
}

void BasePaintTool::DrawPointInScale(SDL_Renderer* renderer, double scaleSize, const SDL_Point& point) const {
    SDL_Point scaledPoint = {(int)(point.x * scaleSize),
                             (int)((point.y - _yBeginningOfCanvas) * scaleSize) + _yBeginningOfCanvas};

    if (scaleSize > SCALE_TYPE_3) {
        for (size_t col = 0; col < (size_t)scaleSize; col++) {
            for (size_t row = 0; row < (size_t)scaleSize; row++) {
                if (Canvas::IsPointInCanvas(scaledPoint.x, scaledPoint.y))
                    SDL_RenderDrawPoint(renderer, scaledPoint.x, scaledPoint.y);
                scaledPoint.y++;
            }
            scaledPoint.y = (int)((point.y - _yBeginningOfCanvas) * scaleSize) + _yBeginningOfCanvas;
            scaledPoint.x++;
        }
    } else  {
        if (Canvas::IsPointInCanvas(scaledPoint.x, scaledPoint.y))
            SDL_RenderDrawPoint(renderer, scaledPoint.x, scaledPoint.y);
    }
}