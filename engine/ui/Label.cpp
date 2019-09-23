/*
 * Label.cpp
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#include "engine/ui/Label.h"

namespace UI {

Label::Label(const std::string& text, Object *parent) :
        UI::Object(parent), text(text) {
    color =
    {	255,255,255,255};

}
Label::Label(Object *parent) :
        UI::Object(parent), text("") {
    color =
    {	255,255,255,255};
}

void Label::render(core::Renderer *pRender, graphics::Texture *pTexture) {
    int x = getX();
    int y = getY();
    if (getParent() != NULL) {
        graphics::Rect rect = getParent()->displayRect();
        x += rect.x;
        y += rect.y;
    }
    getFont()->render(pRender, text, color, x, y);
}

std::string Label::getText() {
    return text;
}
void Label::setText(const std::string& text) {
    this->text = text;
}

void Label::setColor(SDL_Color color) {
    this->color = color;
}

Label::~Label() {
    // TODO Auto-generated destructor stub
}

} /* namespace UI */
