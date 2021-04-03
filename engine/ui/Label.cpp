/*
 * Label.cpp
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#include "engine/ui/Label.h"

namespace UI
{

    Label::Label(const std::string &text, Object *parent)
        : UI::Object(parent), text(text)
    {
        color = {255, 255, 255, 255};
    }
    Label::Label(Object *parent)
        : UI::Object(parent), text("")
    {
        color = {255, 255, 255, 255};
    }

    void Label::render(core::Renderer *pRender)
    {
        int x = getX();
        int y = getY();
        if (getParent() != nullptr)
        {
            graphics::Rect rect = getParent()->displayRect();
            x += rect.x;
            y += rect.y;
        }
        getFont()->render(pRender, text, color, x, y);
    }

    std::string Label::getText()
    {
        return text;
    }
    void Label::setText(const std::string &text)
    {
        this->text = text;

        if (getFont() != nullptr)
        {
            int w, h = 0;
            getFont()->size(text, &w, &h);
            setWidth(w);
            setHeight(h);
        }
    }

    void Label::setColor(SDL_Color color)
    {
        this->color = color;
    }

    Label::~Label()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace UI */
