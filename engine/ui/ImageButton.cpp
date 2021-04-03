#include "engine/ui/ImageButton.h"
#include <engine/graphics/TextureManager.h>

namespace UI
{

    ImageButton::ImageButton(Object *parent, int width, int height, int x2,
                             int y2, bool pResized)
        : UI::Object(parent, width, height)
    {
        image = nullptr;
        this->x2 = x2;
        this->y2 = y2;
        resized = pResized;
        state = ButtonState::None;
        defaultColor = {255, 255, 255, 255};
        clickColor = {255, 255, 255, 255};
        hoverColor = {255, 255, 255, 255};
    }

    ImageButton::~ImageButton()
    {
    }

    void ImageButton::setText(std::string text)
    {
        this->text = text;
    }

    void ImageButton::loadImage(std::string pFilename)
    {

        image = graphics::TextureManager::Instance().loadTexture(pFilename);
    }
    void ImageButton::loadClickedImage(std::string pFilename)
    {
        clickedImage = graphics::TextureManager::Instance().loadTexture(pFilename);
    }

    void ImageButton::setDefaultColor(SDL_Color color)
    {
        defaultColor = color;
    }
    void ImageButton::setHoverColor(SDL_Color color)
    {
        hoverColor = color;
    }
    void ImageButton::setClickColor(SDL_Color color)
    {
        clickColor = color;
    }

    void ImageButton::handleEvents(core::Input *pInput)
    {
        if (image == nullptr)
            return;

        graphics::Rect rect = eventRect();
        rect.width = getWidth();
        rect.height = getHeight();

        utils::Vector2 pos = pInput->getMousePostion();
        if (rect.intersects(pos))
        {
            state = ButtonState::Hovered;
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {

                this->fireFuncionCall("buttonClick");
                this->fireFuncionCall("buttonClickPara",
                                      utils::Vector2(pos.getX() - displayRect().x, pos.getY() - displayRect().y));

                state = ButtonState::Clicked;
            }
        }
        else
        {
            state = ButtonState::None;
        }
    }

    void ImageButton::render(core::Renderer *pRender)
    {
        if (image == nullptr)
            return;
        graphics::Rect srcRect;
        graphics::Rect destRect;
        destRect.x = getX();
        destRect.y = getY();
        destRect.width = getWidth();
        destRect.height = getHeight();
        srcRect.x = x2;
        srcRect.y = y2;
        srcRect.width = getWidth();
        srcRect.height = getHeight();
        if (resized)
        {
            srcRect.width = image->getWidth();
            srcRect.height = image->getHeight();
        }

        if (this->getParent() != nullptr)
        {
            graphics::Rect displayRect = getParent()->displayRect();
            destRect.x += displayRect.x;
            destRect.y += displayRect.y;
        }

        switch (state)
        {
        case ButtonState::None:
            image->setColorKey(defaultColor.r, defaultColor.g, defaultColor.b);
            break;
        case ButtonState::Hovered:
            image->setColorKey(hoverColor.r, hoverColor.g, hoverColor.b);
            break;
        case ButtonState::Clicked:
            image->setColorKey(clickColor.r, clickColor.g, clickColor.b);
            break;
        }

        if (state == ButtonState::Clicked && clickedImage != nullptr)
        {
            clickedImage->render(pRender, srcRect, destRect);
        }
        else
        {
            image->render(pRender, srcRect, destRect);
        }

        int messageX = destRect.x + destRect.width + 5;
        int messageY = destRect.y + 2;

        SDL_Color white = {255, 255, 255, 255};
        switch (state)
        {
        case ButtonState::None:

            break;
        case ButtonState::Hovered:
            white = hoverColor;
            break;
        case ButtonState::Clicked:
            white = clickColor;
            break;
        }
        if (text.length() > 0)
            getParent()->getFont()->render(pRender, text, white, messageX, messageY);
        pRender->setDrawColor(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
    }

} // namespace UI
