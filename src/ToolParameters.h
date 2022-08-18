#ifndef PAINT_TOOLPARAMETERS_H
#define PAINT_TOOLPARAMETERS_H

#include "Constants.h"
#include <SDL.h>

struct ToolParameters {
    SDL_Colour colour{};
    uint32_t normalSize{};
    uint32_t sizes[NUM_OF_PAINT_TOOL_SIZES]{};
    const int yBeginningOfCanvas{};
};

#endif //PAINT_TOOLPARAMETERS_H
