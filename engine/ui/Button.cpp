#include "engine/ui/Button.h"
#include <engine/graphics/TextureManager.h>
#include <functional>

namespace UI
{

    Button::Button(Object *parent)
        : Object(parent), static_width(0), hover(false), enabled(true), borderless(false)
    {
        color = {255, 255, 255, 0};
        width = 0;

        disabledColor = {204, 204, 204, 255};

        hoverColor = {93, 103, 108, 255};

        texture = graphics::TextureManager::Instance().loadTexture("images/Button01.png");
    }

    Button::~Button()
    {
    }

    void Button::setStaticWidth(const int pWidth)
    {
        static_width = pWidth;
        if (static_width > width)
        {
            width = static_width;
            setWidth(width);
        }
    }

    void Button::setFont(const std::string &fontname, unsigned int font_size)
    {
        //TODO error handling
        auto text = graphics::TextureManager::Instance().loadFont(fontname, font_size);
        Object::setFont(text.get());
    }
    void Button::setColor(SDL_Color color)
    {
        this->color = color;
    }

    void Button::setLabel(const std::string &label)
    {
        this->label = label;

        int w = 0;
        int h = 0;
        if (getFont()->size(label, &w, &h) != -1)
        {
            if (w > static_width)
            {
                width = w;
            }
            else
            {
                width = static_width;
            }
            setWidth(width);
            setHeight(35);
        }
    }
    graphics::Rect Button::displayRect()
    {
        graphics::Rect r;

        if (getParent() != nullptr)
        {
            r = getParent()->displayRect();

            r.x += getX();
            r.y += getY();
        }
        else
        {
            r.x = getX();
            r.y = getY();
        }
        r.width = width + 25;
        r.height = getHeight();
        return r;
    }

    graphics::Rect Button::eventRect()
    {
        graphics::Rect r;

        if (getParent() != nullptr)
        {
            r = getParent()->eventRect();

            r.x += getX();
            r.y += getY();
        }
        else
        {
            r.x = getX();
            r.y = getY();
        }
        r.width = width + 25;
        r.height = getHeight();
        return r;
    }

    bool Button::getBorderless() const
    {
        return borderless;
    }

    void Button::setBorderless(bool value)
    {
        borderless = value;
    }
    void Button::handleEvents(core::Input *pInput)
    {
        if (!enabled)
        {
            hover = false;
            return;
        }
        graphics::Rect r = eventRect();

        if (r.intersects(pInput->getMousePostion()))
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                this->fireFuncionCall(buttonClickCallback());
            }
            hover = true;
        }
        else
        {
            hover = false;
        }
        Object::handleEvents(pInput);
    }

    void Button::renderBackground(core::Renderer *pRenderer)
    {
        int tx, ty;
        if (getParent() != nullptr)
        {
            graphics::Rect r = getParent()->displayRect();

            r.x += getX();
            r.y += getY();
            tx = int(r.x);
            ty = int(r.y);
        }
        else
        {
            tx = getX();
            ty = getY();
        }
        if (!borderless)
        {
            //draw background rect
            graphics::Rect backgroundRect;
            backgroundRect.x = tx;
            backgroundRect.y = ty;
            backgroundRect.width = getWidth() + 25;
            backgroundRect.height = getHeight();

            pRenderer->setDrawColor(12, 21, 24, 255);
            pRenderer->fillRect(backgroundRect);

            //left top corner

            texture->render(pRenderer, tx, ty, 9, 9, 0, 0);
            //left bottom corner
            texture->render(pRenderer, tx, ty + getHeight() - 9, 9, 9, 0, 20);

            texture->render(pRenderer, tx + getWidth() + 25 - 9, ty, 9, 9, 174, 0);
            texture->render(pRenderer, tx + getWidth() + 25 - 9, ty + getHeight() - 9, 9, 9, 174, 20);
        }
    }

    SDL_Color Button::getDisabledColor() const
    {
        return disabledColor;
    }

    void Button::setDisabledColor(const SDL_Color &value)
    {
        disabledColor = value;
    }

    SDL_Color Button::getHoverColor() const
    {
        return hoverColor;
    }

    void Button::setHoverColor(const SDL_Color &value)
    {
        hoverColor = value;
    }

    bool Button::isHovered() const
    {
        return hover;
    }

    void Button::render(core::Renderer *pRender)
    {

        int tx;
        SDL_Color displayColor = color;

        if (!enabled)
        {
            displayColor = disabledColor;
        }
        else if (hover)
        {
            displayColor = hoverColor;
        }

        int ty;
        graphics::Rect r = displayRect();

        tx = int(r.x);
        ty = int(r.y);
        renderBackground(pRender);

        getFont()->render(pRender, label, displayColor, tx + 10, ty + 10);
    }

    void Button::enable()
    {
        enabled = true;
    }
    void Button::disable()
    {
        enabled = false;
    }
    bool Button::isEnabled()
    {
        return enabled;
    }

} // namespace UI
