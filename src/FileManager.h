#ifndef PAINT_FILEMANAGER_H
#define PAINT_FILEMANAGER_H

#include "Library.h"
#include "Canvas.h"
#include "sfd.h"
#include <SDL_image.h>

class FileManager {
public:
    static FileManager& GetInstance() {
        static FileManager _instance;
        return _instance;
    }

    ErrorState SaveFile(int canvasHeight);
    ErrorState OpenFile(SDL_Renderer* renderer, int canvasHeight, bool isChangedCanvas, bool& isOpen);

    FileManager(const FileManager& other) = delete;
    FileManager(FileManager&& other) = delete;
    FileManager& operator=(const FileManager& other) = delete;
    FileManager& operator=(FileManager&& other) = delete;

private:
    FileManager() = default;
    ~FileManager() = default;
};


#endif //PAINT_FILEMANAGER_H
