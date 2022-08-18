#include "Library.h"

using namespace std;

ErrorState Library::Init(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& menuFont, TTF_Font*& inscriptionFont)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return ErrorState::SDL_LIBRARY;

    int flag = IMG_INIT_PNG;
    if ((IMG_Init(flag) & flag) != flag)
        return ErrorState::IMG_LIBRARY;

    if (TTF_Init() < 0)
        return ErrorState::SDL_TTF_LIBRARY;

    window = SDL_CreateWindow("Simplified Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        return ErrorState::WINDOW;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
        return ErrorState::RENDERER;

    menuFont = TTF_OpenFont("../resource/fonts/consolas.ttf", 24);
    if (menuFont == nullptr)
        return ErrorState::MENU_FONT;

    inscriptionFont = TTF_OpenFont("../resource/fonts/News 705 Italic BT.ttf", 24);
    if (inscriptionFont == nullptr)
        return ErrorState::INSCRIPTION_FONT;

    return ErrorState::SUCCESS;
}

ErrorState Library::CreateTextureFromText(TTF_Font* font, SDL_Renderer* renderer, const string& name,
                                  const SDL_Color& textColor, SDL_Rect& rect, SDL_Texture*& texture)
{
    SDL_Surface* surface = TTF_RenderText_Blended(font, name.c_str(), textColor);
    if (surface == nullptr)
        return ErrorState::SURFACE;

    rect = {0, 0, surface->w, surface->h};

    return CreateTexture(renderer, surface, texture);
}

ErrorState Library::Load(const char* imageFile, SDL_Renderer*& renderer, SDL_Texture*& texture) {
    SDL_Surface* surface = IMG_Load(imageFile);
    if (surface == nullptr)
        return ErrorState::SURFACE;

    return CreateTexture(renderer, surface, texture);
}

ErrorState Library::CreateTexture(SDL_Renderer* renderer, SDL_Surface*& surface, SDL_Texture*& texture) {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
    if (texture == nullptr)
        return ErrorState::TEXTURE;
    else
        return ErrorState::SUCCESS;
}

void Library::Dispose(ErrorState rc, SDL_Window*& window, SDL_Renderer*& renderer,
                      TTF_Font*& menuFont, TTF_Font*& inscriptionFont)
{
    auto FreeResources = [&]() {
        TTF_CloseFont(inscriptionFont);
        inscriptionFont = nullptr;
        TTF_CloseFont(menuFont);
        menuFont = nullptr;
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        SDL_DestroyWindow(window);
        window = nullptr;
    };

    switch (rc) {
        case ErrorState::SUCCESS:
            FreeResources();
            break;
        case ErrorState::TEXTURE:
            cout << string("Can not create texture from surface: ") + SDL_GetError() << endl;
            FreeResources();
            break;
        case ErrorState::SURFACE:
            cout << string("Can not create surface: ") + SDL_GetError() << endl;
            FreeResources();
            break;
        case ErrorState::SYNTAX:
            cout << "Can not parse file with menu scheme!" << endl;
            FreeResources();
            break;
        case ErrorState::INSCRIPTION_FONT:
            cout << string("Can not open inscription font: ") + TTF_GetError() << endl;
            TTF_CloseFont(menuFont);
            menuFont = nullptr;
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
            SDL_DestroyWindow(window);
            window = nullptr;
            break;
        case ErrorState::MENU_FONT:
            cout << string("Can not open menu font: ") + TTF_GetError() << endl;
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
            SDL_DestroyWindow(window);
            window = nullptr;
            break;
        case ErrorState::RENDERER:
            cout << string("Can not create renderer: ") + SDL_GetError() << endl;
            SDL_DestroyWindow(window);
            window = nullptr;
            break;
        case ErrorState::WINDOW:
            cout << string("Can not create window: ") + SDL_GetError() << endl;
            break;
        case ErrorState::IMG_LIBRARY:
            cout << string("Can not init SDL_image: ") + IMG_GetError() << endl;
            break;
        case ErrorState::SDL_TTF_LIBRARY:
            cout << string("Can not init SDL_ttf: ") + TTF_GetError() << endl;
            break;
        default:
            cout << string("Can not init SDL: ") + SDL_GetError() << endl;
            break;
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}