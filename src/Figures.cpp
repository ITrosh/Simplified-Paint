#include "Figures.h"

using namespace std;

void Figure::CalculateScaledNormalEndPoint(double scaleSize) {
    if (scaleSize != SCALE_TYPE_3) {
        if (scaleSize > SCALE_TYPE_3) {
            _end.x = (int) (_end.x / scaleSize);
            _end.y = (int) ((_end.y - _yBeginningOfCanvas) / scaleSize) + _yBeginningOfCanvas;
        } else {
            _end.x = (int) (_end.x * (1 / scaleSize));
            _end.y = (int) ((_end.y - _yBeginningOfCanvas) * (1 / scaleSize)) + _yBeginningOfCanvas;
        }
    }
}

void Figure::RedrawRectArea(SDL_Renderer* renderer, double scaleSize) {
    SDL_Point begin, end;
    PrepareRectForRedraw(begin, end);

    SDL_Rect rect = { begin.x, begin.y, end.x - begin.x + 1, end.y - begin.y + 1 };
    Canvas::RenderRectangle(renderer, rect, scaleSize);
}

void Figure::PrepareRectForRedraw(SDL_Point& begin, SDL_Point& end) {
    int dx = (_end.x - _begin.x >= 0 ? 1 : -1);
    int dy = (_end.y - _begin.y >= 0 ? 1 : -1);

    if (dx == 1) {
        begin.x = _begin.x - (int) _normalSize;
        end.x = _end.x + (int) _normalSize;
        if (dy == 1) {
            begin.y = _begin.y - (int) _normalSize;
            end.y = _end.y + (int) _normalSize;
        } else {
            begin.y = _end.y - (int) _normalSize;
            end.y = _begin.y + (int) _normalSize;
        }
    } else {
        begin.x = _end.x - (int) _normalSize;
        end.x = _begin.x + (int) _normalSize;
        if (dy == 1) {
            begin.y = _begin.y - (int) _normalSize;
            end.y = _end.y + (int) _normalSize;
        } else {
            begin.y = _end.y - (int) _normalSize;
            end.y = _begin.y + (int) _normalSize;
        }
    }

    if (begin.x < 0)
        begin.x = 0;
    if (end.x >= SCREEN_WIDTH)
        end.x = SCREEN_WIDTH - 1;
    if (begin.y < BUTTON_HEIGHT + INCREASE_BUTTON_SIZE + 2 * ICON_HEIGHT - INCREASE_ICON_SIZE + 1)
        begin.y = BUTTON_HEIGHT + INCREASE_BUTTON_SIZE + 2 * ICON_HEIGHT - INCREASE_ICON_SIZE + 1;
    if (end.y >= SCREEN_HEIGHT)
        end.y = SCREEN_HEIGHT - 1;
}

void Line::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if ((event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION) &&
        event->button.button == SDL_BUTTON_LEFT && !_isReadyToDraw)
    {
        //cout << "Line" << endl;
        if (Canvas::IsPointInCanvas(x, y)) {
            if ((scaleSize >= SCALE_TYPE_3) || (scaleSize < SCALE_TYPE_3 && Canvas::IsPointInCanvasPart(x, y, scaleSize))) {
                _isReadyToDraw = true;
                _end = {x, y};
                CalculateScaledNormalEndPoint(scaleSize);
                _begin = {_end.x, _end.y};
            }
        }
    }
    if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {
        if (Canvas::IsPointInCanvas(x, y)) {
            if ((scaleSize >= SCALE_TYPE_3) || (scaleSize < SCALE_TYPE_3 && Canvas::IsPointInCanvasPart(x, y, scaleSize))) {
                if (_normalSize == _sizes[0]) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    DrawSimpleLine(renderer, scaleSize);
                    RedrawRectArea(renderer, scaleSize);
                } else {
                    thickLineRGBA(renderer, (short) _begin.x, (short) _begin.y,
                                  (short) _end.x, (short) _end.y, _normalSize, 255, 255, 255, 255);
                    InsertInCanvas(renderer, scaleSize);
                    RedrawRectArea(renderer, scaleSize);
                }

                _end = {x, y};
                CalculateScaledNormalEndPoint(scaleSize);

                if (_normalSize == _sizes[0]) {
                    SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
                    DrawSimpleLine(renderer, scaleSize);
                } else {
                    thickLineRGBA(renderer, (short) _begin.x, (short) _begin.y, (short) _end.x,
                                  (short) _end.y, _normalSize, _colour.r, _colour.g, _colour.b, _colour.a);
                    InsertInCanvas(renderer, scaleSize);
                    RedrawRectArea(renderer, scaleSize);
                }
            }
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {
        //cout << "Mouse button up!" << endl;
        if (Canvas::IsPointInCanvas(x, y)) {
            if ((scaleSize >= SCALE_TYPE_3) || (scaleSize < SCALE_TYPE_3 && Canvas::IsPointInCanvasPart(x, y, scaleSize))) {
                _isReadyToDraw = false;
                _end = {x, y};
                CalculateScaledNormalEndPoint(scaleSize);

                if (_begin.x != _end.x || _begin.y != _end.y) {
                    isChangedCanvas = true;

                    if (_normalSize == _sizes[0]) {
                        SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
                        DrawSimpleLine(renderer, scaleSize);
                    } else {
                        thickLineRGBA(renderer, (short) _begin.x, (short) _begin.y, (short) _end.x, (short) _end.y,
                                      _normalSize, _colour.r, _colour.g, _colour.b, _colour.a);
                        InsertInCanvas(renderer, scaleSize);
                    }
                }
            }
        }
    }
}

void Line::DrawSimpleLine(SDL_Renderer* renderer, double scaleSize) {
    int dx = (_end.x - _begin.x >= 0 ? 1 : -1);
    int dy = (_end.y - _begin.y >= 0 ? 1 : -1);

    int lengthX = abs(_end.x - _begin.x);
    int lengthY = abs(_end.y - _begin.y);

    int length = max(lengthX, lengthY);

    if (lengthY <= lengthX) {
        // Начальные значения
        int x = _begin.x;
        int y = _begin.y;
        int d = -lengthX;

        // Основной цикл
        length++;
        while(length--) {
            if (Canvas::IsPointInCanvas(x, y)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[x][y - _yBeginningOfCanvas].colour = _colour;

                DrawPointInScale(renderer, scaleSize, {x, y});
            }
            x += dx;
            d += 2 * lengthY;
            if (d > 0) {
                d -= 2 * lengthX;
                y += dy;
            }
        }
    } else {
        // Начальные значения
        int x = _begin.x;
        int y = _begin.y;
        int d = -lengthY;

        // Основной цикл
        length++;
        while(length--) {
            if (Canvas::IsPointInCanvas(x, y)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[x][y - _yBeginningOfCanvas].colour = _colour;

                DrawPointInScale(renderer, scaleSize, {x, y});
            }
            y += dy;
            d += 2 * lengthX;
            if (d > 0) {
                d -= 2 * lengthY;
                x += dx;
            }
        }
    }
}

void Line::InsertInCanvas(SDL_Renderer* renderer, double scaleSize) {
    SDL_Point begin, end;
    PrepareRectForRedraw(begin, end);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

    int bpp = surface->format->BytesPerPixel;
    SDL_Color colour;
    for (int i = begin.x; i <= end.x; i++) {
        for (int j = begin.y; j <= end.y; j++) {
            Uint32 p = *(Uint32*)((Uint8*)surface->pixels + j * surface->pitch  + i * bpp);
            SDL_GetRGBA(p, surface->format, &colour.r, &colour.g, &colour.b, &colour.a);
            if (Canvas::IsPointInCanvas(i, j)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[i][j - _yBeginningOfCanvas].colour = colour;

                if (scaleSize != SCALE_TYPE_3) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawPoint(renderer, i, j);
                    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
                    DrawPointInScale(renderer, scaleSize, {i, j});
                }
            }
        }
    }
    SDL_FreeSurface(surface);
}

void Rectangle::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if ((event->type == SDL_MOUSEBUTTONDOWN ||  event->type == SDL_MOUSEMOTION) &&
        event->button.button == SDL_BUTTON_LEFT && !_isReadyToDraw)
    {
        //cout << "Rectangle" << endl;
        if (Canvas::IsPointInCanvas(x, y)) {
            _isReadyToDraw = true;
            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);
            _begin = {_end.x, _end.y};
        }
    }
    if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {
        if (Canvas::IsPointInCanvas(x, y)) {

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            Draw(renderer, scaleSize);
            RedrawRectArea(renderer, scaleSize);

            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);

            SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
            Draw(renderer, scaleSize);
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {
        //cout << "Insert line in Canvas!" << endl;
        if (Canvas::IsPointInCanvas(x, y)) {
            _isReadyToDraw = false;

            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);
            
            if (_begin.x != _end.x && _begin.y != _end.y) {
                isChangedCanvas = true;
                
                SDL_SetRenderDrawColor(renderer, _colour.r, _colour.g, _colour.b, _colour.a);
                Draw(renderer, scaleSize);
            }
        }
    }
}

void Rectangle::Draw(SDL_Renderer* renderer, double scaleSize) {
    int dx = (_end.x - _begin.x >= 0 ? 1 : -1);
    int dy = (_end.y - _begin.y >= 0 ? 1 : -1);

    int margin = (int) _normalSize / 2;
    int beginX, beginY, endX, endY;

    if (dx == 1) {
        beginX = _begin.x - margin;
        endX = _end.x + margin;
        if (dy == 1) {
            beginY = _begin.y - margin;
            endY = _end.y - margin;
        } else {
            beginY = _end.y - margin;
            endY = _begin.y - margin;
        }
    } else {
        beginX = _end.x - margin;
        endX = _begin.x + margin;
        if (dy == 1) {
            beginY = _begin.y - margin;
            endY = _end.y - margin;
        } else {
            beginY = _end.y - margin;
            endY = _begin.y - margin;
        }
    }

    int tempX = beginX;
    int tempY = beginY;
    while (tempX <= endX) {
        for (size_t i = 0; i < _normalSize; i++, tempY++) {
            if (Canvas::IsPointInCanvas(tempX, tempY)) {
                //cout << beginX << " " << beginY << endl;
                if (!_isReadyToDraw) {
                    Canvas::RefFragments()[tempX][tempY - _yBeginningOfCanvas].colour = _colour;
                }

                DrawPointInScale(renderer, scaleSize, {tempX, tempY});
            }
        }
        tempY = beginY;
        tempX++;
    }

    tempX = beginX;
    tempY = beginY + (int) _normalSize;
    while (tempY < endY) {
        for (size_t i = 0; i < _normalSize; i++, tempX++) {
            if (Canvas::IsPointInCanvas(tempX, tempY)) {
                //cout << beginX << " " << beginY << endl;
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[tempX][tempY - _yBeginningOfCanvas].colour = _colour;

                DrawPointInScale(renderer, scaleSize, {tempX, tempY});
            }
        }
        tempX = beginX;
        tempY++;
    }

    tempX = endX - (int) _normalSize + 1;
    tempY = beginY + (int) _normalSize;
    while (tempY < endY) {
        for (size_t i = 0; i < _normalSize; i++, tempX++) {
            if (Canvas::IsPointInCanvas(tempX, tempY)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[tempX][tempY - _yBeginningOfCanvas].colour = _colour;

                DrawPointInScale(renderer, scaleSize, {tempX, tempY});
            }
        }
        tempX = endX - (int) _normalSize + 1;
        tempY++;
    }

    tempX = beginX;
    tempY = endY;
    while (tempX <= endX) {
        for (size_t i = 0; i < (int) _normalSize; i++, tempY++) {
            if (Canvas::IsPointInCanvas(tempX, tempY)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[tempX][tempY - _yBeginningOfCanvas].colour = _colour;

                DrawPointInScale(renderer, scaleSize, {tempX, tempY});
            }
        }
        tempY = endY;
        tempX++;
    }
}

void Circle::HandleEvent(SDL_Event* event, SDL_Renderer* renderer, double scaleSize, bool& isChangedCanvas) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if ((event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION) &&
        event->button.button == SDL_BUTTON_LEFT && !_isReadyToDraw)
    {
        //cout << "Circle" << endl;
        if (Canvas::IsPointInCanvas(x, y)) {
            _isReadyToDraw = true;
            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);
            _begin = {_end.x, _end.y};
        }
    }
    if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {
        if (Canvas::IsPointInCanvas(x, y)) {
            Draw(renderer, 255, 255, 255, 255, scaleSize);
            RedrawRectArea(renderer, scaleSize);

            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);

            Draw(renderer, _colour.r, _colour.g, _colour.b, _colour.a, scaleSize);
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && _isReadyToDraw) {

        if (Canvas::IsPointInCanvas(x, y)) {
            _isReadyToDraw = false;
            _end = {x, y};
            CalculateScaledNormalEndPoint(scaleSize);
            
            if (_begin.x != _end.x && _begin.y != _end.y) {
                isChangedCanvas = true;
                Draw(renderer, _colour.r, _colour.g, _colour.b, _colour.a, scaleSize);
            }
        }
    }
}

void Circle::Draw(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, double scaleSize) {
    SDL_Point center;
    int radius;

    if (abs(_end.x - _begin.x) <= abs(_end.y - _begin.y)) {
        radius = (int) (abs(_end.x - _begin.x) / 2 + _normalSize / 2);
        if (_begin.x > _end.x &&
            ((_begin.x % 2 != 0 && _end.x % 2 == 0) || (_begin.x % 2 == 0 && _end.x % 2 != 0))) {
            center.x = (_begin.x + _end.x + 1) / 2;
        } else {
            center.x = (_begin.x + _end.x) / 2;
        }

        if (_end.y > _begin.y)
            center.y = _begin.y + abs(_end.x - _begin.x) / 2;
        else
            center.y = _begin.y - abs(_end.x - _begin.x) / 2;

    } else {
        radius = (int) (abs(_end.y - _begin.y) / 2 + _normalSize / 2);
        if (_begin.y > _end.y &&
            ((_begin.y % 2 != 0 && _end.y % 2 == 0) || (_begin.y % 2 == 0 && _end.y % 2 != 0))) {
            center.y = (_begin.y + _end.y + 1) / 2;
        } else {
            center.y = (_begin.y + _end.y) / 2;
        }

        if (_end.x > _begin.x)
            center.x = _begin.x + abs(_end.y - _begin.y) / 2;
        else
            center.x = _begin.x - abs(_end.y - _begin.y) / 2;
    }

    if (_normalSize == _sizes[0]) {
        DrawCircle(renderer, center.x, center.y, radius, r, g, b, a, false, scaleSize);
    } else {
        DrawCircle(renderer, center.x, center.y, radius, r, g, b, a, true, scaleSize);
        radius = radius - (int) _normalSize;
        DrawCircle(renderer, center.x, center.y, radius, 255, 255, 255, 255, true, scaleSize);
    }
}

int Circle::DrawCircle(SDL_Renderer* renderer, int x, int y, int rad, Uint8 r,
                       Uint8 g, Uint8 b, Uint8 a, bool isFilled, double scaleSize)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    int tempX = 0;
    int tempY = rad;
    int D = 2 * (1 - rad);
    int lim = 0;
    int delta;
    int result;

    while (true) {
        if (Canvas::IsPointInCanvas(x + tempX, y + tempY)) {
            if (!_isReadyToDraw)
                Canvas::RefFragments()[x + tempX][y + tempY - _yBeginningOfCanvas].colour = _colour;

            DrawPointInScale(renderer, scaleSize, {x + tempX, y + tempY});
        }
        if (Canvas::IsPointInCanvas(x - tempX, y + tempY)) {
            if (!_isReadyToDraw)
                Canvas::RefFragments()[x - tempX][y + tempY - _yBeginningOfCanvas].colour = _colour;

            DrawPointInScale(renderer, scaleSize, {x - tempX, y + tempY});
        }
        if (Canvas::IsPointInCanvas(x + tempX, y - tempY)) {
            if (!_isReadyToDraw)
                Canvas::RefFragments()[x + tempX][y - tempY - _yBeginningOfCanvas].colour = _colour;

            DrawPointInScale(renderer, scaleSize, {x + tempX, y - tempY});
        }

        if (Canvas::IsPointInCanvas(x - tempX, y - tempY)) {
            if (!_isReadyToDraw)
                Canvas::RefFragments()[x - tempX][y - tempY - _yBeginningOfCanvas].colour = _colour;

            DrawPointInScale(renderer, scaleSize, {x - tempX, y - tempY});
        }

//        cout << x + tempX << " " << y + tempY << endl;
//        cout << x - tempX << " " << y + tempY << endl;
//        cout << x + tempX << " " << y - tempY << endl;
//        cout << x - tempX << " " << y - tempY << endl;

        if (isFilled) {
            result |= VerticalLine(renderer, x + tempX, y - tempY, y + tempY, r, g, b, a, scaleSize);
            result |= VerticalLine(renderer, x - tempX, y - tempY, y + tempY, r, g, b, a, scaleSize);
        }

        if (tempY <= lim) {
            break;
        } else {
            if (D < 0) {
                delta = 2 * D + 2 * tempY - 1;
                if (delta <= 0) {
                    tempX -= - 1;
                    D = D + 2 * tempX + 1;
                } else {
                    tempX += 1;
                    tempY -= 1;
                    D = D + 2 * tempX - 2 * tempY + 2;
                }
            } else if (D > 0) {
                delta = 2 * D - 2 * tempX - 1;
                if (delta <= 0) {
                    tempX += 1;
                    tempY -= 1;
                    D = D + 2 * tempX - 2 * tempY + 2;
                } else {
                    tempY -= 1;
                    D = D - 2 * tempY + 1;
                }
            } else {
                tempX += 1;
                tempY -= 1;
                D = D + 2 * tempX - 2 * tempY + 2;
            }
        }
    }
    return result;
}

int Circle::VerticalLine(SDL_Renderer* renderer, int x, int y1, int y2,
                         Uint8 r, Uint8 g, Uint8 b, Uint8 a, double scaleSize)
{
    int result;
    SDL_Point point = {x, y1};

    if (point.y < y2) {
        while (point.y <= y2) {
            if (Canvas::IsPointInCanvas(x, point.y)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[point.x][point.y - _yBeginningOfCanvas].colour = {r, g, b, a};

                DrawPointInScale(renderer, scaleSize, point);
            }
            point.y++;
        }
    } else if (point.y > y2) {
        while (point.y >= y2) {
            if (Canvas::IsPointInCanvas(x, point.y)) {
                if (!_isReadyToDraw)
                    Canvas::RefFragments()[point.x][point.y - _yBeginningOfCanvas].colour = {r, g, b, a};

                DrawPointInScale(renderer, scaleSize, point);
            }
            point.y--;
        }
    } else {
        if (Canvas::IsPointInCanvas(point.x, point.y)) {
            if (!_isReadyToDraw)
                Canvas::RefFragments()[point.x][point.y - _yBeginningOfCanvas].colour = {r, g, b, a};

            DrawPointInScale(renderer, scaleSize, point);
        }
    }
    return result;
}