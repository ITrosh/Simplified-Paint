#include "Paint.h"

using namespace std;

Paint::~Paint() {
    for (auto& paintTool: _availablePaintTools) {
        delete paintTool;
    }
}

void Paint::SwitchPaintTool(PaintToolType type) {
    _workingPaintTool = _availablePaintTools.at(static_cast<size_t>(type));
}

int Paint::Run() {
    bool isRun = true;
    bool isShowInterface = false;
    bool isButtonDown = false;
    bool isNewFile = true;
    bool isChangedCanvas = false;
    string name;

    SDL_Event event;
    SDL_Colour colour = {255, 255, 255, 255};
    SDL_Rect popupMenuArea;

    ErrorState errorState = Init();
    if (errorState != ErrorState::SUCCESS) {
        Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);
        return static_cast<int>(errorState);
    }

    errorState = MainMenu::GetInstance().Create("../resource/Menu.txt", _menuFont, _renderer);
    if (errorState != ErrorState::SUCCESS) {
        MainMenu::GetInstance().Free();
        Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);
        return static_cast<int>(errorState);
    }

    PaintToolType paintTool = PaintToolType::Pencil;
    SwitchPaintTool(paintTool);

    while (isRun) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRun = false;
                break;
            }
        }

        if (IsCursorInsideWindow()) {
            switch (MainMenu::GetInstance().Draw(&event, _renderer, name, popupMenuArea)) {
                case MenuState::Main_Button_Released:
                    isShowInterface = true;
                    ClearPopupMenu(popupMenuArea);
                    break;
                case MenuState::Popup_Button_Released:
                    if (name == "New") {
                        if (!Canvas::IsCanvasWhite() && isChangedCanvas) {
                            errorState = FileManager::GetInstance().SaveFile(_canvasHeight);
                            if (errorState != ErrorState::SUCCESS) {
                                MainMenu::GetInstance().Free();
                                Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);
                                return static_cast<int>(errorState);
                            }

                            Canvas::Clear();
                            isChangedCanvas = false;
                            colour = {255, 255, 255, 255};
                            isNewFile = true;
                        } else {
                            ClearPopupMenu(popupMenuArea);
                        }
                    } else if (name == "Open...") {
                        bool isOpen = false;
                        errorState = FileManager::GetInstance().OpenFile(_renderer, _canvasHeight, isChangedCanvas,
                                                                         isOpen);
                        if (errorState != ErrorState::SUCCESS) {
                            MainMenu::GetInstance().Free();
                            Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);
                            return static_cast<int>(errorState);
                        }

                        if (isOpen) {
                            if (!_zoom.IsScaleNormal())
                                _zoom.SetNormalScale();
                            Canvas::Redraw(_renderer, SCALE_TYPE_3);
                            isChangedCanvas = false;
                        } else {
                            ClearPopupMenu(popupMenuArea);
                        }
                    } else {
                        errorState = FileManager::GetInstance().SaveFile(_canvasHeight);
                        if (errorState != ErrorState::SUCCESS) {
                            MainMenu::GetInstance().Free();
                            Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);
                            return static_cast<int>(errorState);
                        }
                        ClearPopupMenu(popupMenuArea);
                        isChangedCanvas = false;
                    }
                    name.clear();
                    break;
                case MenuState::Default:
                    isShowInterface = true;
                    break;
                default:
                    isShowInterface = false;
                    break;
            }

            _zoom.HandleStates(_renderer);

            if (isNewFile) {
                SDL_SetRenderDrawColor(_renderer, colour.r, colour.g, colour.b, colour.a);
                SDL_RenderClear(_renderer);
                _interface->Draw(_renderer);
                isNewFile = false;
                isShowInterface = true;
            }

            if (isShowInterface) {
                PaintToolType tempPaintTool = paintTool;
                ResizingToolType resizingTool = ResizingToolType::None;
                SDL_Colour toolColour;

                _interface->Draw(_renderer);

                _interface->HandleEvents(&event, _renderer, isButtonDown, tempPaintTool, resizingTool, toolColour);
                if (tempPaintTool != paintTool) {
                    SwitchPaintTool(tempPaintTool);
                    paintTool = tempPaintTool;
                }

                if (resizingTool != ResizingToolType::None) {
                    bool isIncrease;
                    if (resizingTool == ResizingToolType::Increase)
                        isIncrease = true;
                    else
                        isIncrease = false;
                    _workingPaintTool->ChooseNewSize(isIncrease);
                    resizingTool = ResizingToolType::None;
                }
                if (!_workingPaintTool->IsSameColours(toolColour) && paintTool != PaintToolType::Eraser) {
                    _workingPaintTool->ChangeColour(toolColour);
                }
            }


            //SDL_GetMouseState(&point.x, &point.y);
            //SDL_GetGlobalMouseState(&point.x, &point.y);
            //SDL_GetWindowPosition(_window, &point.x, &point.y);
            //cout << point.x << " " << point.y << endl;
            if (/*Canvas::IsPointInCanvas(point.x, point.y) &&*/ _workingPaintTool != nullptr && !isButtonDown &&
                                                                 isShowInterface) {
                _workingPaintTool->HandleEvent(&event, _renderer, _zoom.GetCurrentScale(), isChangedCanvas);
            }

            SDL_RenderPresent(_renderer);
            SDL_Delay(25);
        }
    }

    MainMenu::GetInstance().Free();
    Library::Dispose(errorState, _window, _renderer, _menuFont, _inscriptionFont);

    return static_cast<int>(errorState);
}

ErrorState Paint::Init() {
    ErrorState errorState = Library::Init(_window, _renderer, _menuFont, _inscriptionFont);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    SDL_Surface* icon = IMG_Load("../resource/icons/simplified-paint-logo.png");
    if (icon == nullptr)
        return ErrorState::SURFACE;

    SDL_SetWindowIcon(_window, icon);

    SDL_GetWindowPosition(_window, &_windowRect.x, &_windowRect.y);
    SDL_GetWindowSize(_window, &_windowRect.w, &_windowRect.h);

    errorState = CreateInterface();
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    Canvas::InitCanvas(_canvasHeight);
    InitPaintTools();

    return errorState;
}

ErrorState Paint::CreateInterface() {
    vector<Inscription> inscriptions;
    ErrorState errorState = CreateInscriptions(inscriptions);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    SDL_Texture* texture = nullptr;
    SDL_Rect block = {0, BUTTON_HEIGHT + INCREASE_BUTTON_SIZE, 2 * ICON_WIDTH, 2 * ICON_HEIGHT - INCREASE_ICON_SIZE};
    vector<SDL_Rect> blocks;

    _canvasHeight = block.y + block.h + 1;

    SDL_Rect icon = {0, BUTTON_HEIGHT + INCREASE_BUTTON_SIZE, ICON_WIDTH, ICON_HEIGHT};
    SDL_Rect textureRect = {
            INCREASE_ICON_SIZE / 2,
            icon.y + INCREASE_ICON_SIZE / 2,
            icon.w - INCREASE_ICON_SIZE,
            icon.h - INCREASE_ICON_SIZE,
    };

    auto it = inscriptions.begin();

    block.w += INCREASE_ICON_SIZE;
    icon.x += INCREASE_ICON_SIZE / 2;
    textureRect.x += INCREASE_ICON_SIZE / 2;
    it->textRect.x = block.x + block.w / 2 - it->textRect.w / 2;
    it->textRect.y = block.y + ICON_HEIGHT + (ICON_HEIGHT - INCREASE_ICON_SIZE) / 2 - it->textRect.h / 2;
    it++;

    errorState = Library::Load("../resource/images/pencil.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    auto iconParameters = make_unique<map<IconName, TextureParameters>>();
    iconParameters->insert({IconName::PENCIL, {icon, textureRect, texture}});

    errorState = Library::Load("../resource/images/eraser.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    icon.x += icon.w;
    textureRect.x += icon.w;
    iconParameters->insert({IconName::ERASER, {icon, textureRect, texture}});

    blocks.push_back({block});

    block.x += block.w;
    block.w = 3 * (ICON_WIDTH - INCREASE_ICON_SIZE) + INCREASE_ICON_SIZE;
    icon.x = block.x + INCREASE_ICON_SIZE / 2;
    textureRect.x = block.x + INCREASE_ICON_SIZE;
    it->textRect.x = block.x + block.w / 2 - it->textRect.w / 2;
    it->textRect.y = block.y + ICON_HEIGHT + (ICON_HEIGHT - INCREASE_ICON_SIZE) / 2 - it->textRect.h / 2;
    it++;

    errorState = Library::Load("../resource/images/line.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    icon.w -= INCREASE_ICON_SIZE;
    textureRect.w -= INCREASE_ICON_SIZE;
    iconParameters->insert({IconName::LINE, {icon, textureRect, texture}});

    errorState = Library::Load("../resource/images/rectangle.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    icon.x += icon.w;
    textureRect.x += icon.w;
    iconParameters->insert({IconName::RECTANGLE, {icon, textureRect, texture}});

    errorState = Library::Load("../resource/images/circle.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    icon.x += icon.w;
    textureRect.x += icon.w;
    iconParameters->insert({IconName::CIRCLE, {icon, textureRect, texture}});

    blocks.push_back({block});

    block.x += block.w;
    block.w = it->textRect.w + INCREASE_ICON_SIZE;
    icon.x = block.x + INCREASE_ICON_SIZE / 2 + it->textRect.w / 2 - icon.w;
    textureRect.x = block.x + INCREASE_ICON_SIZE + it->textRect.w / 2 - icon.w;
    it->textRect.x = block.x + block.w / 2 - it->textRect.w / 2;
    it->textRect.y = block.y + ICON_HEIGHT + (ICON_HEIGHT - INCREASE_ICON_SIZE) / 2 - it->textRect.h / 2;
    it++;

    errorState = Library::Load("../resource/images/increase.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    iconParameters->insert({IconName::INCREASE, {icon, textureRect, texture}});

    errorState = Library::Load("../resource/images/decrease.png", _renderer, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    icon.x += icon.w;
    textureRect.x += icon.w;
    textureRect.h /= 5;
    textureRect.y += icon.h / 2 - INCREASE_ICON_SIZE / 2 - textureRect.h / 2;
    iconParameters->insert({IconName::DECREASE, {icon, textureRect, texture}});

    blocks.push_back({block});

    block.x += block.w;
    block.w = it->textRect.w + INCREASE_ICON_SIZE;
    icon.x = block.x + INCREASE_ICON_SIZE / 2 + it->textRect.w / 2 - icon.w / 2;
    it->textRect.x = block.x + block.w / 2 - it->textRect.w / 2;
    it->textRect.y = block.y + ICON_HEIGHT + (ICON_HEIGHT - INCREASE_ICON_SIZE) / 2 - it->textRect.h / 2;
    it++;

    SDL_Rect mainColour = {
            icon.x + INCREASE_ICON_SIZE / 4,
            icon.y + INCREASE_ICON_SIZE / 4,
            icon.w - INCREASE_ICON_SIZE / 2,
            icon.h - INCREASE_ICON_SIZE / 2,
    };
    ColourIcon colourIcon = {icon, mainColour};

    blocks.push_back({block});

    block.x += block.w;
    block.w = it->textRect.w + INCREASE_ICON_SIZE;
    icon.w /= 2;
    icon.h /= 2;
    icon.x = block.x + INCREASE_ICON_SIZE / 2 + (it->textRect.w - icon.w * NUM_OF_COLOUR_COLUMNS) / 2;
    it->textRect.x = block.x + block.w / 2 - it->textRect.w / 2;
    it->textRect.y = block.y + ICON_HEIGHT + (ICON_HEIGHT - INCREASE_ICON_SIZE) / 2 - it->textRect.h / 2;
    it++;

    SDL_Colour colour[NUM_OF_COLOUR_ROWS * NUM_OF_COLOUR_COLUMNS] = {
            {0, 0, 0, 255}, {255, 255, 255, 255},
            {128, 128, 128, 0}, {255, 0, 0, 255},
            {0, 255, 0, 255}, {0, 0, 255, 255},
            {255, 255, 0, 255}, {0, 255, 255, 255},
            {255, 0, 255, 255}, {128, 0, 0, 255},
    };
    vector<ColourRect> colourTable;

    for (size_t i = 0; i < NUM_OF_COLOUR_COLUMNS; i++) {
        for (size_t j = 0; j < NUM_OF_COLOUR_ROWS; j++) {
            colourTable.emplace_back(icon, colour[i * NUM_OF_COLOUR_ROWS + j]);
            icon.y += icon.h;
        }
        icon.y -= NUM_OF_COLOUR_ROWS * icon.h;
        icon.x += icon.w;
    }

    blocks.push_back({block});

    _interface = make_unique<Interface>(move(iconParameters), colourIcon, move(colourTable), move(inscriptions), move(blocks));
    return errorState;
}

ErrorState Paint::CreateInscriptions(std::vector<Inscription>& inscriptions) {
    SDL_Colour colour = {41, 49, 51, 0};
    SDL_Rect textRect;
    SDL_Texture* texture = nullptr;

    ErrorState errorState = Library::CreateTextureFromText(_inscriptionFont, _renderer, "Tools", colour, textRect, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    inscriptions.push_back({textRect, texture});

    errorState = Library::CreateTextureFromText(_inscriptionFont, _renderer, "Figures", colour, textRect, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    inscriptions.push_back({textRect, texture});

    errorState = Library::CreateTextureFromText(_inscriptionFont, _renderer, "Thickness", colour, textRect, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    inscriptions.push_back({textRect, texture});

    errorState = Library::CreateTextureFromText(_inscriptionFont, _renderer, "Colour", colour, textRect, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    inscriptions.push_back({textRect, texture});

    errorState = Library::CreateTextureFromText(_inscriptionFont, _renderer, "Colour table", colour, textRect, texture);
    if (errorState != ErrorState::SUCCESS)
        return errorState;

    inscriptions.push_back({textRect, texture});
    return errorState;
}

void Paint::ClearPopupMenu(const SDL_Rect& popupMenuArea) {
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(_renderer, &popupMenuArea);
    SDL_Rect rect = {0, _canvasHeight, popupMenuArea.w, popupMenuArea.h - _canvasHeight};
    Canvas::RenderRectangle(_renderer, rect, _zoom.GetCurrentScale());
}

void Paint::InitPaintTools() {
    ToolParameters toolParameters1 = {{0, 0, 0, 255}, P_SIZE_1, {P_SIZE_1, P_SIZE_2, P_SIZE_3, P_SIZE_4}, _canvasHeight};
    ToolParameters toolParameters2 = {{255, 255, 255, 255}, E_SIZE_1, {E_SIZE_1, E_SIZE_2, E_SIZE_3, E_SIZE_4}, _canvasHeight};
    ToolParameters toolParameters3 = {{0, 0, 0, 255}, F_SIZE_1, {F_SIZE_1, F_SIZE_2, F_SIZE_3, F_SIZE_4}, _canvasHeight};
    _availablePaintTools = vector<BasePaintTool*> {
            new Pencil(toolParameters1),
            new Eraser(toolParameters2),
            new Line(toolParameters3),
            new Rectangle(toolParameters3),
            new Circle(toolParameters3),
    };
}

bool Paint::IsCursorInsideWindow() const {
    SDL_Point mouseCursor;
    SDL_GetGlobalMouseState(&mouseCursor.x, &mouseCursor.y);
    return (mouseCursor.x >= _windowRect.x && mouseCursor.x < _windowRect.x + _windowRect.w &&
            mouseCursor.y >= _windowRect.y && mouseCursor.y < _windowRect.y + _windowRect.h);
}