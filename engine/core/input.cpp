#include "engine/core/input.h"

namespace core {

Input::Input()
    : mousePosition(-1, -1)
{
    //ctor
}

bool Input::poll()
{
    bool retval = SDL_PollEvent(&event);
    if (retval) {
        if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED) {
            pressedKeys[event.key.keysym.sym] = true;

        } else if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED) {
            pressedKeys[event.key.keysym.sym] = false;
        }
        if (isMouseMoving()) {
            mousePosition = utils::Vector2(getEvent().motion.x,
                getEvent().motion.y);
        }
    }
    return retval;
}

bool Input::isQuit()
{
    return (event.type == SDL_QUIT);
}

bool Input::isKeyDown(SDL_Keycode key)
{
    return pressedKeys.count(key) > 0 && pressedKeys[key];
    if (event.type == SDL_KEYDOWN && key == event.key.keysym.sym
        && event.key.state == SDL_PRESSED)
        return true;
    return false;
}
bool Input::isKeyUp(SDL_Keycode key)
{
    if (event.type == SDL_KEYUP && key == event.key.keysym.sym
        && event.key.state == SDL_RELEASED)
        return true;
    return false;
}

bool Input::isMouseButtonPressed(int button)
{
    return (getEvent().button.button == button
        && event.type == SDL_MOUSEBUTTONDOWN);
}
bool Input::isMouseButtonUp(int button)
{
    return (getEvent().button.button == button
        && event.type == SDL_MOUSEBUTTONUP);
}

bool Input::isMouseMoving()
{
    return (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN);
}
bool Input::isMouseDoubleClick()
{
    return isMouseButtonPressed(SDL_BUTTON_LEFT) && getEvent().button.clicks >= 2;
}
bool Input::isScrollWheel()
{
    return event.type == SDL_MOUSEWHEEL;
}

utils::Vector2 Input::getMouseWheelPosition()
{
    return utils::Vector2(event.wheel.x, event.wheel.y);
}

utils::Vector2 Input::getMousePostion()
{

    return mousePosition;
}

Input::~Input()
{
    //dtor
}

std::string Input::getTextInput()
{
    return std::string(event.edit.text);
}
bool Input::isTextInput()
{
    return SDL_IsTextInputActive() && event.type == SDL_TEXTINPUT;
}
} // namespace core
