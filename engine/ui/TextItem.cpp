/*
 * TextItem.cpp
 *
 *  Created on: 05.02.2017
 *      Author: stefan
 */

#include <engine/ui/TextItem.h>

namespace UI {

TextItem::TextItem(Object *parent, int pWidth, int pHeight) :
        UI::Object(parent, pWidth, pHeight) {

    // TODO Auto-generated constructor stub
    SDL_StopTextInput();
    color = {0,0,0,0};
}

TextItem::~TextItem() {
    // TODO Auto-generated destructor stub
}

std::string TextItem::getText() {
    return text;
}
void TextItem::setText(const std::string& text) {
    this->text = text;
}

void TextItem::render(core::Renderer *pRender, graphics::Texture *texture) {
    graphics::Rect rect = displayRect();

    pRender->drawRect(rect);
    if (text.size() != 0)
        getFont()->render(pRender, text, color, rect.x, rect.y);

}
void TextItem::handleEvents(core::Input *pInput) {
    if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        if (displayRect().intersects(pInput->getMousePostion())) {
        SDL_Rect rect = displayRect().sdlRect();
        SDL_SetTextInputRect(&rect);
        SDL_StartTextInput();
        } else {
            SDL_StopTextInput();
        }

    } else if (pInput->isTextInput()) {
        std::string txt = pInput->getTextInput();
        text += txt;
    } else if (pInput->isKeyDown(SDLK_BACKSPACE)) {
        if (text.size() > 0)
            text.resize(text.size() - 1);
    }
}
void TextItem::setFont(const std::string& fontname, unsigned int font_size) {
    graphics::Text *text = new graphics::Text();
    text->openFont(fontname, font_size);
    Object::setFont(text);
}
void TextItem::setColor(int r, int g, int b) {
    color.r = r;
    color.g = g;
    color.b = b;
}
} /* namespace UI */
