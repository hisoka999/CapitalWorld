#include "engine/ui/Checkbox.h"
#include <engine/graphics/TextureManager.h>

namespace UI
{

    Checkbox::Checkbox(Object *parent)
        : UI::Object(parent)
    {
        uiIconText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        //    textCore = new graphics::Text();
        //    textCore->openFont("arial.ttf", 12);
        checked = false;
        color = {255, 255, 255, 255};
    }

    Checkbox::~Checkbox()
    {
        uiIconText = nullptr;
    }

    void Checkbox::handleEvents(core::Input *pInput)
    {
        SDL_Event e = pInput->getEvent();
        int tx;
        int ty;
        int textwidth = 0;
        if (getParent() != nullptr)
        {
            tx = getParent()->getX() + getX();
            ty = getParent()->getY() + getY();
        }
        else
        {
            tx = getX();
            ty = getY();
        }

        if (e.button.x >= tx && e.button.x <= tx + textwidth + 25 && e.button.y >= ty && e.button.y <= ty + 23)
        {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                std::cout << "toggle checked" << std::endl;
                toggleChecked();
            }
        }
    }

    void Checkbox::render(core::Renderer *pRender)
    {

        int tx = getX();
        int ty = getY();
        if (this->getParent())
        {
            tx += this->getParent()->getX();
            ty += this->getParent()->getY();
        }
        int x2 = 0;
        if (checked)
            uiIconText->render(pRender, "\uf00c", color, tx, ty);
        //pTexture->render(pRender, tx, ty, 25, 23, x2, 260);
        graphics::Rect rect;
        rect.x = tx;
        rect.y = ty;
        rect.width = 25;
        rect.height = 25;

        pRender->drawRect(rect);
        getFont()->render(pRender, text, color, tx + 30, ty);
    }

} // namespace UI
