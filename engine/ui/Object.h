#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <iostream>
#include <map>

#include "engine/core/renderer.h"
#include "engine/core/input.h"
#include "engine/core/dispatcher.h"
#include "engine/graphics/texture.h"
#include "engine/graphics/text.h"
#include "engine/graphics/rect.h"

namespace UI {

class BaseObject {

};

class Listener {

};

class Object: public BaseObject {

public:
    /** Default constructor */
    Object(Object *parent = 0);
    Object(Object *parent, int pWidth, int pHeight);
    /** Default destructor */
    virtual ~Object();
    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    void setPos(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }
    void setWidth(int pWidth) {
        width = pWidth;
    }
    void setHeight(int pHeight) {
        height = pHeight;
    }

    virtual void render(core::Renderer *pRender, graphics::Texture *texture);
    virtual void handleEvents(core::Input *pInput);
    Object *getParent() {
        return parent;
    }
    Listener *getListener() {
        return listener;
    }
    void setListener(Listener *listener) {
        this->listener = listener;
    }
    template<typename F>
    void connect(std::string const& event, F&& f) {
        _callbacks.emplace(event,
                core::make_dispatcher(std::forward < F > (f)));
    }

    template<typename F>
    void disconnect(std::string const& event, F&& f) {
        auto rng = _callbacks.equal_range(event);
        for (auto it = rng.first; it != rng.second; ++it) {
            if (it->second == f) {
                _callbacks.erase(it);
            }
        }
    }

    graphics::Text *getFont();
    void setFont(graphics::Text *pFont);
    virtual graphics::Rect displayRect();

protected:
    template<typename ... Args>
    void fireFuncionCall(std::string const& event, Args const&... args) {
        auto rng = _callbacks.equal_range(event);
        for (auto it = rng.first; it != rng.second; ++it) {
            call(it->second, args...);
        }

    }
private:
    template<typename F, typename ... Args>
    void call(F const& f, Args const&... args) {
        std::vector < boost::any > v { args... };
        f(v);
    }

    Object *parent;
    int x, y;
    int width, height;
    Listener *listener;
    std::multimap<std::string, core::dispatcher_type> _callbacks;
    graphics::Text *font;
};

} // namespace UI

#endif // UI_OBJECT_H
