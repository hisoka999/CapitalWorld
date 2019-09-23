#include "engine/ui/Button.h"
#include <functional>

namespace UI {

Button::Button(UI::Object *parent) :
        Object(parent), static_width(0), hover(false), enabled(true) {
    color =
    {	0,0,0};
    width = 0;

}

Button::~Button() {
}

void Button::setStaticWidth(const int pWidth) {
    static_width = pWidth;
    if (static_width > width) {
        width = static_width;
    }
}

void Button::setFont(const std::string& fontname, unsigned int font_size) {
    //TODO error handling
    graphics::Text *text = new graphics::Text();
    text->openFont(fontname, font_size);
    Object::setFont(text);
}
void Button::setColor(SDL_Color color) {
    this->color = color;
}

void Button::setLabel(const std::string& label) {
    this->label = label;

    int w = 0;
    int h = 0;
    if (getFont()->size(label, &w, &h) != -1) {
        if (w > static_width) {
            width = w;
        } else {
            width = static_width;
        }
    }
}

void Button::handleEvents(core::Input *pInput) {
    if (!enabled) {
        hover = false;
        return;
    }
    graphics::Rect r;
    if (getParent() != NULL) {
        r = getParent()->displayRect();

        r.x += getX();
        r.y += getY();
    } else {
        r.x = getX();
        r.y = getY();
    }

    r.width = 25 + width;
    r.height = 35;
    if (r.intersects(pInput->getMousePostion())) {
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            this->fireFuncionCall(buttonClickCallback());
        }
        hover = true;
    } else {
        hover = false;
    }
    Object::handleEvents(pInput);
}

void Button::render(core::Renderer *pRender, graphics::Texture *texture) {

    int tx;
    SDL_Color displayColor = color;

    if (!enabled) {
        displayColor =
        {	204,204,204,255};
    }

    int ty;
    if (getParent() != NULL) {
        graphics::Rect r = getParent()->displayRect();

        r.x += getX();
        r.y += getY();
        tx = r.x;
        ty = r.y;
    } else {
        tx = getX();
        ty = getY();
    }

    //std::cout <<label<<" tx: "<<tx<<" ty:"<<ty<<std::endl;
    texture->render(pRender, tx, ty, 10, 35, (hover) ? 48 : 0, 420);
    // render background
    {
        graphics::Rect src;
        graphics::Rect dest;
        //background = texture->subSurface(20,420,10,35);
        if (hover) {
            src.x = 68;
            src.y = 420;
        } else {
            src.x = 20;
            src.y = 420;
        }
        src.width = 10;
        src.height = 35;

        dest.x = tx + 10;
        dest.y = ty;
        dest.width = width;
        dest.height = 35;
        texture->render(pRender, src, dest);
    }
    //texture->renderResized(pRender,tx+10,ty,)

    //texture->draw(pRender,tx+10,ty);
    texture->render(pRender, tx + 10 + width, ty, 15, 35, (hover) ? 78 : 30,
            420);
    //CFramework::getFramework()->drawSurface("ArkanaLook",tx,ty,10,35,0,420);
    //pRender->printText(tx+10,ty+10,label);
    getFont()->render(pRender, label, displayColor, tx + 10, ty + 10);
}

void Button::enable() {
    enabled = true;
}
void Button::disable() {
    enabled = false;
}
bool Button::isEnabled() {
    return enabled;
}

} // namespace UI
