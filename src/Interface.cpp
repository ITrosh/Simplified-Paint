#include "Interface.h"

#include <utility>

using namespace std;

Interface::Interface(std::unique_ptr<std::map<IconName, TextureParameters>> iconParameters,
                     const ColourIcon& mainColour, std::vector<ColourRect>&& colourTable,
                     std::vector<Inscription>&& inscriptions, std::vector<SDL_Rect>&& blocks)
                     : _mainColour(mainColour), _colourTable(std::move(colourTable)),
                     _inscriptions(std::move(inscriptions)), _parts(std::move(blocks))
{
    auto it = iconParameters->begin();
    auto end = iconParameters->end();
    while (it != end) {
        _icons.emplace_back(it->first, it->second);
        it++;
    }
}

void Interface::Draw(SDL_Renderer* renderer) {
    DrawBlocks(renderer);
    DrawColourButton(renderer);
    DrawInscriptions(renderer);
    DrawDustInterfacePart(renderer);
    DrawLines(renderer);
}

void Interface::DrawBlocks(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 253, 217, 181, 0);
    for (const auto& part: _parts) {
        SDL_RenderFillRect(renderer, &part);
    }
}

void Interface::DrawColourButton(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 47, 79, 79, 0);
    SDL_RenderFillRect(renderer, &_mainColour.externalRect);
    if (_activeIcon == IconName::ERASER)
        SDL_SetRenderDrawColor(renderer, _eraserColour.r, _eraserColour.g, _eraserColour.b, _eraserColour.a);
    else
        SDL_SetRenderDrawColor(renderer, _toolColour.r, _toolColour.g, _toolColour.b, _toolColour.a);
    SDL_RenderFillRect(renderer, &_mainColour.colourRect);
}

void Interface::DrawInscriptions(SDL_Renderer* renderer) {
    for (const auto& inscription: _inscriptions)
        SDL_RenderCopy(renderer, inscription.text, nullptr, &inscription.textRect);
}

void Interface::DrawDustInterfacePart(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 253, 217, 181, 0);
    SDL_Rect rect;
    rect.x = _parts[_parts.size() - 1].x + _parts[_parts.size() - 1].w;
    rect.y = _parts[_parts.size() - 1].y;
    rect.w = SCREEN_WIDTH - rect.x;
    rect.h = _parts[_parts.size() - 1].h;
    SDL_RenderFillRect(renderer, &rect);
}

void Interface::DrawLines(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 0);
    int y = BUTTON_HEIGHT + INCREASE_BUTTON_SIZE;
    SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    y += 2 * ICON_HEIGHT - INCREASE_ICON_SIZE;
    SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
}

void Interface::HandleEvents(SDL_Event* event, SDL_Renderer* renderer,
                             bool& isButtonDown, PaintToolType& paintTool,
                             ResizingToolType& resizingTool, SDL_Colour& toolColour)
{
    IconName temp = _activeIcon;

    for (auto& icon: _icons) {
        icon.HandleIconEvent(event, temp, isButtonDown);
        if (temp != _activeIcon) {
            if (temp != IconName::INCREASE && temp != IconName::DECREASE) {
                _icons[static_cast<int>(_activeIcon)].BecomeDefault();
                if (static_cast<int>(_activeIcon) < static_cast<int>(temp))
                    _icons[static_cast<int>(_activeIcon)].DrawIcon(renderer);
                ChangePaintTool(temp, paintTool);
                _activeIcon = temp;
            }
            else {
                ResizePaintTool(temp, resizingTool);
                temp = _activeIcon;
            }
        }
        icon.DrawIcon(renderer);
    }

    if (_activeIcon != IconName::ERASER) {
        toolColour = _toolColour;
        for (auto& colourIcon: _colourTable) {
            colourIcon.HandleEvent(event, toolColour);
            if (!IsSameColours(toolColour)) {
                if (!_isChangeColour)
                    _isChangeColour = true;
                _toolColour = toolColour;
                DrawColourButton(renderer);
            }
            colourIcon.Draw(renderer);
        }
    } else {
        for (auto& colourIcon: _colourTable)
            colourIcon.Draw(renderer);
    }
}

void Interface::ChangePaintTool(IconName iconName, PaintToolType& paintTool) {
    switch (iconName) {
        case IconName::PENCIL:
            paintTool = PaintToolType::Pencil;
            break;
        case IconName::ERASER:
            paintTool = PaintToolType::Eraser;
            break;
        case IconName::LINE:
            paintTool = PaintToolType::Line;
            break;
        case IconName::RECTANGLE:
            paintTool = PaintToolType::Rectangle;
            break;
        default:
            paintTool = PaintToolType::Circle;
            break;
    }
}

void Interface::ResizePaintTool(IconName iconName, ResizingToolType& resizingTool) {
    if (iconName == IconName::INCREASE)
        resizingTool = ResizingToolType::Increase;
    else
        resizingTool = ResizingToolType::Decrease;
}

bool Interface::IsSameColours(const SDL_Color& colour) const {
    if (colour.r != _toolColour.r || colour.g != _toolColour.g ||
        colour.b != _toolColour.b || colour.a != _toolColour.a)
        return false;
    else
        return true;
}