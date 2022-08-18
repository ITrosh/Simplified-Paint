#include "Icon.h"

using namespace std;

Icon::Icon(const IconName& name, const TextureParameters& textureParameters)
        : _name(name), _rect(textureParameters.icon), _textureRect(textureParameters.textureRect), _texture(textureParameters.texture)
{
    if (name == IconName::PENCIL) {
        _state = IconState::PRESSED;
        _context = IconContext::CHOSEN;
    }
}

void Icon::HandleIconEvent(SDL_Event* event, IconName& name, bool& isButtonDown) {
    SDL_Point mouseCursor;
    SDL_GetMouseState(&mouseCursor.x, &mouseCursor.y);
    if (!SDL_PointInRect(&mouseCursor, &_rect)) {
        if (name != _name)
            _state = IconState::DEFAULT;
        if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
            if (_context == IconContext::PRESSED && name != _name && isButtonDown) {
                //cout << "Out" << endl;
                _context = IconContext::DEFAULT;
                _state = IconState::DEFAULT;
                isButtonDown = false;
            }
        }
    }
    else {
        if (_context == IconContext::PRESSED || _context == IconContext::CHOSEN)
            _state = IconState::PRESSED;
        else if (SDL_PointInRect(&mouseCursor, &_rect) && !isButtonDown)
            _state = IconState::HOVER;
        else
            _state = IconState::DEFAULT;

        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
            if (_state == IconState::HOVER && _context == IconContext::DEFAULT) {
                _state = IconState::PRESSED;
                _context = IconContext::PRESSED;
                isButtonDown = true;
            }
        }
        if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
            if (_context == IconContext::PRESSED && isButtonDown) {
                //cout << "New Icon" << endl;
                isButtonDown = false;
                name = _name;
                _context = IconContext::CHOSEN;
                if (_name == IconName::INCREASE || _name == IconName::DECREASE)
                    _context = IconContext::DEFAULT;
            }
        }
    }
}

void Icon::DrawIcon(SDL_Renderer* renderer) {
    SDL_Colour colour;
    switch (_state) {
        case IconState::HOVER:
        {
            colour = {72, 110, 178, 0};
            break;
        }
        case IconState::PRESSED:
        {
            colour = {47, 79, 79, 0};
            break;
        }
        default:
        {
            colour = {103, 72, 70, 0};
            break;
        }
    }
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderFillRect(renderer, &_rect);
    SDL_RenderCopy(renderer, _texture, nullptr, &_textureRect);
}

void Icon::BecomeDefault() {
    _context = IconContext::DEFAULT;
    _state = IconState::DEFAULT;
}

bool Icon::IsPointInIcon(const SDL_Point& point) const {
    return SDL_PointInRect(&point, &_rect);
}
