#ifndef PAINT_ZOOM_H
#define PAINT_ZOOM_H

#include "Canvas.h"

enum class ScaleContext {
    Increase,
    Decrease,
    None,
};

class Zoom {
public:
    Zoom() = default;

    void HandleStates(SDL_Renderer* renderer);
    void ChangeScale(bool isBigger);
    void SetNormalScale();

    bool IsScaleNormal() const;

    double GetCurrentScale() const { return _currentScale; }

private:
    size_t _index = 2;
    double _currentScale = SCALE_TYPE_3;
    double _scaleSizes[NUM_OF_SCALE_SIZES] = {SCALE_TYPE_1, SCALE_TYPE_2, SCALE_TYPE_3, SCALE_TYPE_4, SCALE_TYPE_5};

    ScaleContext _scaleContext = ScaleContext::None;

};

#endif //PAINT_ZOOM_H
