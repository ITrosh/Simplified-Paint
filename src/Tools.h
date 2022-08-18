#ifndef PAINT_TOOLS_H
#define PAINT_TOOLS_H

#include "BasePaintTool.h"

class Tool : public BasePaintTool {
public:
    explicit Tool(const ToolParameters& toolParameters) : BasePaintTool(toolParameters) {}

protected:
    void Draw(SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas);

private:
    void CalculateNormalScaledPoint(double scaleSize, SDL_Point& point);
};

class Pencil: public Tool {
public:
    explicit Pencil(const ToolParameters& toolParameters) : Tool(toolParameters) {}
    virtual ~Pencil() {}

    void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangeCanvas) override;
};

class Eraser: public Tool {
public:
    explicit Eraser(const ToolParameters& toolParameters) : Tool(toolParameters) {}
    virtual ~Eraser() {}

    void HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) override;
};

#endif //PAINT_TOOLS_H