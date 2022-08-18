cmake_minimum_required(VERSION 3.20)
project(Simplified-Paint)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/MenuSystem src/3rdparty)

add_executable(${PROJECT_NAME} resource/resource.rc src/main.cpp src/Paint.cpp src/Paint.h src/MenuSystem/ErrorState.h src/MenuSystem/IButton.h
        src/MenuSystem/Library.h src/MenuSystem/Library.cpp src/MenuSystem/MainMenu.h src/MenuSystem/MainMenu.cpp
        src/MenuSystem/MainButton.h src/MenuSystem/MainButton.cpp src/MenuSystem/PopupMenu.h
        src/MenuSystem/PopupMenu.cpp src/MenuSystem/PopupButton.h src/MenuSystem/PopupButton.cpp
        src/MenuSystem/PopupButtonParameters.h src/MenuSystem/Parser.h src/MenuSystem/Parser.cpp src/MenuSystem/Constants.h
        src/3rdparty/sfd.h src/3rdparty/sfd.cpp src/MenuSystem/TextureParameters.h
        src/InterfaceEnumerations.h src/Interface.cpp src/Interface.h src/Icon.cpp src/Icon.h
        src/ToolParameters.h src/BasePaintTool.cpp src/BasePaintTool.h src/Tools.h src/Tools.cpp
        src/Canvas.cpp src/Canvas.h  src/Figures.h src/Figures.cpp
        src/ColourRect.cpp src/ColourRect.h src/Zoom.cpp src/Zoom.h src/FileManager.h src/FileManager.cpp)

# Copying of all necessary dll files on which the executable depends
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/lib/3rdparty"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

target_link_options(${PROJECT_NAME} PRIVATE -mwindows) # To hide the console window at program startup on Windows

target_link_libraries(${PROJECT_NAME}
        mingw32
        SDL2main
        SDL2
        SDL2_ttf
        SDL2_image
        SDL2_gfx
)