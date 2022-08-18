#include "FileManager.h"

using namespace std;

ErrorState FileManager::SaveFile(int canvasHeight) {
    static sfd_Options opt = {
            .title        = "Save Image File",
            .filter_name  = "Image File",
            .filter       = "*.png",
    };

    string filename = sfd_save_dialog(&opt);

    if (!filename.empty()) {
        if (filename.rfind(".png") == std::string::npos) {
            filename.append(".png");
        }

        //cout << "Save file: " << filename.data() << endl;

        SDL_Surface* surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT - canvasHeight,
                                                    32, 0, 0, 0, 0);
        if (surface == nullptr)
            return ErrorState::SURFACE;
        Canvas::PaintSurface(surface, canvasHeight);
        IMG_SavePNG(surface, filename.data());
        SDL_FreeSurface(surface);
    } else {
        //cout << "Save canceled!" << endl;
    }
    return ErrorState::SUCCESS;
}

ErrorState FileManager::OpenFile(SDL_Renderer* renderer, int canvasHeight, bool isChangedCanvas, bool& isOpen) {
    static sfd_Options opt = {
            .title        = "Open Image File",
            .filter_name  = "Image File",
            .filter       = "*.png",
    };

    std::string filename = sfd_open_dialog(&opt);

    if (!filename.empty()) {
        isOpen = true;
        if (filename.rfind(".png") == std::string::npos) {
            filename.append(".png");
        }

        if (!Canvas::IsCanvasWhite() && isChangedCanvas)
            SaveFile(canvasHeight);

        //cout << "Open file: " << filename.data() << endl;

        SDL_Surface* surface = IMG_Load(filename.data());
        if (surface == nullptr)
            return ErrorState::SURFACE;

        if (surface->w >= 0 && surface->w <= SCREEN_WIDTH && surface->h >= 0 &&
            surface->h <= SCREEN_HEIGHT - canvasHeight) {
            Canvas::FillCanvas(surface, 0);
            SDL_FreeSurface(surface);
        } else {
            SDL_Rect rect = {0, canvasHeight, SCREEN_WIDTH, SCREEN_HEIGHT - canvasHeight};
            SDL_Texture* texture = nullptr;
            ErrorState errorState = Library::CreateTexture(renderer, surface, texture);
            if (errorState != ErrorState::SUCCESS)
                return errorState;

            SDL_RenderCopy(renderer, texture, nullptr, &rect);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(texture);
            texture = nullptr;

            SDL_Surface* tempSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
            if (tempSurface == nullptr)
                return ErrorState::SURFACE;

            SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, tempSurface->pixels, tempSurface->pitch);
            Canvas::FillCanvas(tempSurface, canvasHeight);
            SDL_FreeSurface(tempSurface);
        }
    } else {
        isOpen = false;
        //cout << "Open canceled!" << endl;
    }
    return ErrorState::SUCCESS;
}
