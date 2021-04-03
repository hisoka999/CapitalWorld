#include "iconbutton.h"
#include <engine/graphics/TextureManager.h>
namespace UI {
IconButton::IconButton(Object* parent)
    : UI::Button(parent)
{
    iconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
}

void IconButton::render(core::Renderer* pRender)
{
    int tx;
    SDL_Color displayColor = color;

    if (!isEnabled()) {
        displayColor = getDisabledColor();
    } else if (isHovered()) {
        displayColor = getHoverColor();
    }

    int ty;
    if (getParent() != nullptr) {
        graphics::Rect r = getParent()->displayRect();

        r.x += getX();
        r.y += getY();
        tx = int(r.x);
        ty = int(r.y);
    } else {
        tx = getX();
        ty = getY();
    }
    renderBackground(pRender);
    iconFont->render(pRender, iconText, displayColor, tx + 10, ty + 10);
    getFont()->render(pRender, label, displayColor, tx + 35, ty + 12);
}

void IconButton::setIconText(const std::string& value)
{
    iconText = value;
}

}
