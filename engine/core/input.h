#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include <SDL2/SDL.h>
#include "engine/utils/vector2.h"
#include <map>
#include <string>

namespace core
{

    class Input
    {
    public:
        /** Default constructor */
        Input();
        /** Default destructor */
        virtual ~Input();
        bool isKeyDown(SDL_Keycode key);
        bool isKeyUp(SDL_Keycode key);
        bool isMouseButtonPressed(int button);
        bool isMouseButtonUp(int button);
        bool isMouseMoving();
        bool isMouseDoubleClick();
        bool isQuit();
        utils::Vector2 getMousePostion();
        bool poll();
        //void clearEvents();
        SDL_Event getEvent()
        {
            return event;
        }
        utils::Vector2 getMouseWheelPosition();
        bool isScrollWheel();
        std::string getTextInput();
        bool isTextInput();

    protected:
    private:
        SDL_Event event;

        std::map<SDL_Keycode, bool> pressedKeys;
        utils::Vector2 mousePosition;
    };

} // namespace core

#endif // CORE_INPUT_H
