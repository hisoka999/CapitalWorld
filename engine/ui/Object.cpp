#include "engine/ui/Object.h"
#include <stdexcept>
#include <engine/graphics/TextureManager.h>

namespace UI
{

    Object::Object(Object *parent)
        : parent(parent)
    {
        x = y = 0;
        width = height = 0;
        font = nullptr;
        renderOrder = 0;
    }
    Object::Object(Object *parent, int pWidth, int pHeight)
        : parent(parent), width(pWidth), height(pHeight), x(0), y(0), renderOrder(0)
    {

        font = nullptr;
    }

    Object::~Object()
    {
        //dtor
    }
    int Object::getRenderOrder()
    {
        return renderOrder;
    }

    void Object::render(core::Renderer *pRender)
    {
    }
    void Object::handleEvents(core::Input *pInput)
    {
    }
    graphics::Text *Object::getFont()
    {
        if (font != nullptr)
        {
            return font;
        }
        else if (parent != nullptr)
        {
            return parent->getFont();
        }
        throw std::runtime_error(
            "Object::getFont(): there was no font loaded for this object");
        return nullptr;
    }
    void Object::setFont(graphics::Text *pFont)
    {
        font = pFont;
    }

    void Object::setFont(const std::string &fontname, unsigned int font_size)
    {
        auto text = graphics::TextureManager::Instance().loadFont(fontname, font_size);
        font = text.get();
    }
    graphics::Rect Object::eventRect()
    {
        graphics::Rect r;
        r.x = x;
        r.y = y;
        r.width = width;
        r.height = height;
        if (getParent() != nullptr)
        {
            r.x += getParent()->eventRect().x;
            r.y += getParent()->eventRect().y;
        }
        return r;
    }

    graphics::Rect Object::displayRect()
    {
        graphics::Rect r;
        r.x = x;
        r.y = y;
        r.width = width;
        r.height = height;
        if (getParent() != nullptr)
        {
            r.x += getParent()->displayRect().x;
            r.y += getParent()->displayRect().y;
        }
        return r;
    }
} // namespace UI
