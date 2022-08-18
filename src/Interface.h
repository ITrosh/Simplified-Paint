#ifndef PAINT_INTERFACE_H
#define PAINT_INTERFACE_H

#include "InterfaceEnumerations.h"
#include "Icon.h"
#include "ColourRect.h"
#include <vector>
#include <map>
#include <iostream>
#include <memory>

struct ColourIcon {
    SDL_Rect externalRect;
    SDL_Rect colourRect;
};

struct Inscription {
    SDL_Rect textRect;
    SDL_Texture* text;
};

class Interface {
public:
    Interface(std::unique_ptr<std::map<IconName, TextureParameters>> iconParameters,
              const ColourIcon& mainColour, std::vector<ColourRect>&& colourTable,
              std::vector<Inscription>&& inscriptions, std::vector<SDL_Rect>&& blocks);

    void Draw(SDL_Renderer* renderer);

    void HandleEvents(SDL_Event* event, SDL_Renderer* renderer,
                      bool& isButtonDown, PaintToolType& paintTool,
                      ResizingToolType& resizingTool, SDL_Colour& toolColour);

    bool IsSameColours(const SDL_Colour& colour) const;

private:
    const SDL_Colour _eraserColour = {255, 255, 255, 255};

    IconName _activeIcon = IconName::PENCIL;
    ColourIcon _mainColour;

    SDL_Colour _toolColour = {0, 0, 0, 255};

    bool _isChangeColour = false;

    std::vector<SDL_Rect> _parts;
    std::vector<Icon> _icons;
    std::vector<ColourRect> _colourTable;
    std::vector<Inscription> _inscriptions;

    void DrawBlocks(SDL_Renderer* renderer);
    void DrawColourButton(SDL_Renderer* renderer);
    void DrawInscriptions(SDL_Renderer* renderer);
    void DrawDustInterfacePart(SDL_Renderer* renderer);
    static void DrawLines(SDL_Renderer* renderer);

    static void ResizePaintTool(IconName iconName, ResizingToolType& resizingTool);
    static void ChangePaintTool(IconName iconName, PaintToolType& paintTool);
};

#endif //PAINT_INTERFACE_H