#include "engine/ui/Object.h"
#include <stdexcept>

namespace UI {

Object::Object(Object *parent) :
        parent(parent) {
    this->listener = NULL;
    x = y = 0;
    width = height = 0;
    font = nullptr;
    renderOrder = 0;
}
Object::Object(Object *parent, int pWidth, int pHeight) :
        parent(parent), width(pWidth), height(pHeight), x(0), y(0),renderOrder(0) {
    this->listener = NULL;

    font = nullptr;
}

Object::~Object() {
    //dtor
}
int Object::getRenderOrder()
{
    return renderOrder;
}


void Object::render(core::Renderer *pRender, graphics::Texture *texture) {
}
void Object::handleEvents(core::Input *pInput) {

}
graphics::Text *Object::getFont() {
    if (font != nullptr) {
        return font;
    } else if (parent != nullptr) {
        return parent->getFont();
    }
    throw std::runtime_error(
            "Object::getFont(): there was no font loaded for this object");
    return nullptr;
}
void Object::setFont(graphics::Text *pFont) {
    font = pFont;
}
graphics::Rect Object::displayRect() {
    graphics::Rect r;
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
    if (getParent() != nullptr) {
        r.x += getParent()->displayRect().x;
        r.y += getParent()->displayRect().y;
    }
    return r;
}
} // namespace UI
