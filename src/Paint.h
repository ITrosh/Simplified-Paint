#ifndef PAINT_PAINT_H
#define PAINT_PAINT_H

#include "Library.h"
#include "MainMenu.h"
#include "FileManager.h"
#include "Interface.h"
#include "Tools.h"
#include "Figures.h"
#include "Zoom.h"
#include <memory>

class Paint {
public:
    static Paint& Instance() {
        static Paint _instance;
        return _instance;
    }

    Paint(const Paint& paint) = delete;
    Paint(Paint&& paint) = delete;
    Paint& operator=(const Paint& paint) = delete;
    Paint& operator=(Paint&& paint) = delete;

    int Run();

private:
    Paint() = default;
    ~Paint();

    int _canvasHeight = 0;
    SDL_Window* _window = nullptr;
    SDL_Rect _windowRect{};
    SDL_Renderer* _renderer = nullptr;
    TTF_Font* _menuFont = nullptr;
    TTF_Font* _inscriptionFont = nullptr;

    BasePaintTool* _workingPaintTool = nullptr;

    std::unique_ptr<Interface> _interface;
    std::vector<BasePaintTool*> _availablePaintTools;

    Zoom _zoom;

    ErrorState Init();
    void InitPaintTools();

    ErrorState CreateInterface();
    ErrorState CreateInscriptions(std::vector<Inscription>& inscriptions);

    bool IsCursorInsideWindow() const;

    void SwitchPaintTool(PaintToolType type);
    void ClearPopupMenu(const SDL_Rect& popupMenuArea);
};

#endif //PAINT_PAINT_H
