#include "Zoom.h"

using namespace std;

void Zoom::HandleStates(SDL_Renderer* renderer) {
    auto* keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates[SDL_SCANCODE_LCTRL] && keyStates[SDL_SCANCODE_KP_PLUS] && _scaleContext == ScaleContext::None) {
        //cout << "Pressed plus" << endl;
        _scaleContext = ScaleContext::Increase;
        return;
    }
    if (keyStates[SDL_SCANCODE_LCTRL] && !keyStates[SDL_SCANCODE_KP_PLUS] && _scaleContext == ScaleContext::Increase) {
        //cout << "Increase scale" << endl;
        _scaleContext = ScaleContext::None;
        ChangeScale(true);
        Canvas::Redraw(renderer, _currentScale);
        return;
    }

    if (keyStates[SDL_SCANCODE_LCTRL] && keyStates[SDL_SCANCODE_KP_MINUS] && _scaleContext == ScaleContext::None) {
        //cout << "Pressed minus" << endl;
        _scaleContext = ScaleContext::Decrease;
        return;
    }
    if (keyStates[SDL_SCANCODE_LCTRL] && !keyStates[SDL_SCANCODE_KP_MINUS] && _scaleContext == ScaleContext::Decrease) {
        //cout << "Decrease scale" << endl;
        _scaleContext = ScaleContext::None;
        ChangeScale(false);
        Canvas::Redraw(renderer, _currentScale);
        return;
    }
}

void Zoom::ChangeScale(bool isBigger) {
    if (isBigger && _index < NUM_OF_SCALE_SIZES - 1)
        _index++;
    else if (!isBigger && _index > 0)
        _index--;
    _currentScale = _scaleSizes[_index];
}

void Zoom::SetNormalScale() {
    _index = 2;
    _currentScale = _scaleSizes[_index];
}

bool Zoom::IsScaleNormal() const {
   return _currentScale == SCALE_TYPE_3;
}
