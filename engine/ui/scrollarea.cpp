#include "engine/ui/scrollarea.h"
#include <algorithm>
#include <iostream>

namespace UI {

ScrollArea::ScrollArea(const unsigned int pWidth, const unsigned pHeight,
        UI::Object* parent = 0) :
        Object(parent), scrollWidth(0), scrollHeight(0), scrollX(0), scrollY(0), scrollPosX(
                0), scrollPosY(0), buttonPressed(false) {
    renderRect.x = 0;
    renderRect.y = 0;
    renderRect.width = pWidth;
    renderRect.height = pHeight;
    renderArea = nullptr;
}

ScrollArea::~ScrollArea() {
    //dtor
    delete renderArea;
}

void ScrollArea::reset() {
    scrollPosX = 0;
    scrollPosY = 0;
    delete renderArea;
    renderArea = nullptr;
}

void ScrollArea::render(core::Renderer *pRender, graphics::Texture *pTexture) {
    graphics::Rect parentRect;
    if (getParent() != nullptr)
        parentRect = getParent()->displayRect();

    //render scroolbar
    //82 354
    int lastScrollWidth = scrollWidth;
    int lastScrollHeight = scrollHeight;
    scrollWidth = 0;
    scrollX = 0;
    std::for_each(objects.begin(), objects.end(), [&](Object* o)
    {
        graphics::Rect r = o->displayRect();
        if(scrollWidth < r.x + r.width)
        {
            scrollWidth = r.x + r.width;
            scrollX = r.x;
        }

        if(scrollHeight < r.y + r.height)
        {
            scrollHeight = r.y + r.height;
            scrollY = r.y;
        }

    });
    if (scrollWidth == 0)
        scrollWidth = renderRect.width;
    if (scrollHeight == 0)
        scrollHeight = renderRect.height;
    pTexture->render(pRender, parentRect.x + getX(),
            parentRect.y + getY() + renderRect.height - 10, 10, 10, 82, 354);

    pTexture->render(pRender, parentRect.x + getX() + renderRect.width - 10,
            parentRect.y + getY() + renderRect.height - 10, 10, 10, 82, 354);

    if (lastScrollWidth != scrollWidth || lastScrollHeight != scrollHeight) {
        delete renderArea;
        renderArea = nullptr;
    }

    //render Objects to Texture
    if (renderArea == nullptr) {

        renderArea = new graphics::Texture(pRender, scrollWidth, scrollHeight);
        SDL_SetTextureBlendMode(renderArea->getSDLTexture(),
                SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(pRender->getRenderer(), 0, 0, 0, 0);

        pRender->setRenderTarget(renderArea->getSDLTexture());
        std::for_each(objects.begin(), objects.end(), [&](Object* o)
        {
            o->render(pRender,pTexture);
        });
        pRender->setRenderTarget(nullptr);
    }
    graphics::Rect srcRect;
    graphics::Rect destRect;

    srcRect.width = renderArea->getWidth();
    srcRect.height = renderArea->getHeight();
    srcRect.x = scrollPosX;
    srcRect.y = scrollPosY;
    if (renderArea->getWidth() < renderRect.width) {
        destRect.width = renderArea->getWidth();
    } else {
        destRect.width = renderRect.width;
        srcRect.width = renderRect.width;
    }
    if (renderArea->getHeight() < renderRect.height) {
        destRect.height = renderArea->getHeight();
    } else {
        srcRect.height = renderRect.height;
        destRect.height = renderRect.height;
    }
    destRect.x = parentRect.x + getX();
    destRect.y = parentRect.y + getY();

    renderArea->render(pRender, srcRect, destRect);
    //render scrollbars
    //render horizontal(wagerecht)
    //render left edge
    if (scrollWidth - renderRect.width > 0) {
        srcRect.x = 82;
        srcRect.y = 354;
        srcRect.width = 14;
        srcRect.height = 14;
        destRect.width = 14;
        destRect.height = 14;
        destRect.x = parentRect.x + getX();
        destRect.y = parentRect.y + renderRect.height + getY();
        pTexture->render(pRender, srcRect, destRect);
        //render right edge
        srcRect.x = 64;
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        pTexture->render(pRender, srcRect, destRect);
        //render bar
        srcRect.x = 0;
        srcRect.y = 403;
        srcRect.width = 3;
        srcRect.height = 5;
        destRect.width = renderRect.width - 28;
        destRect.height = 5;
        destRect.x = parentRect.x + 14 + getX();
        destRect.y = parentRect.y + renderRect.height + 4 + getY();
        pTexture->render(pRender, srcRect, destRect);
        //render slider
        srcRect.x = 52;
        srcRect.y = 384;
        srcRect.width = 10;
        srcRect.height = 14;
        destRect.width = 10;
        destRect.height = 14;
        destRect.y = parentRect.y + renderRect.height + getY();
        destRect.x =
                parentRect.x + 14 + getX()
                        + (static_cast<float>(scrollPosX)
                                / static_cast<float>(scrollWidth
                                        - renderRect.width - 14)
                                * (static_cast<float>(renderRect.width) - 28.0));
    pTexture->render(pRender, srcRect, destRect);
    }
    //render vertical(senkrecht)
    //render left edge
    if (scrollHeight - renderRect.height) {
        srcRect.x = 18;
        srcRect.y = 354;
        srcRect.width = 14;
        srcRect.height = 14;
        destRect.width = 14;
        destRect.height = 14;
        destRect.x = parentRect.x + getX() + renderRect.width - 14;
        destRect.y = parentRect.y + getY();
        pTexture->render(pRender, srcRect, destRect);
        //render right edge
        srcRect.x = 34;
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        destRect.y = parentRect.y + renderRect.height + getY() - 14;
        pTexture->render(pRender, srcRect, destRect);
        //render bar
        srcRect.x = 0;
        srcRect.y = 399;
        srcRect.width = 4;
        srcRect.height = 3;
        destRect.width = 4;
        destRect.height = renderRect.height - 24;
        destRect.x = parentRect.x + renderRect.width - 8 + getX();
        destRect.y = parentRect.y + 14 + getY();
        pTexture->render(pRender, srcRect, destRect);
        //render slider
        srcRect.x = 0;
        srcRect.y = 354;
        srcRect.width = 14;
        srcRect.height = 10;
        destRect.width = 14;
        destRect.height = 10;
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        destRect.y = parentRect.y + 14 + getY()
                + (static_cast<float>(scrollPosY)
                        / static_cast<float>(scrollHeight - renderRect.height
                                - 14)
                        * (static_cast<float>(renderRect.height) - 28.0));
        pTexture->render(pRender, srcRect, destRect);
    }
}

graphics::Rect ScrollArea::displayRect() {
    return renderRect;
}

void ScrollArea::handleEvents(core::Input *pInput) {
    Container::handleEvents(pInput);
    int i = 0;

    graphics::Rect parentRect;
    if (getParent() != NULL)
        parentRect = getParent()->displayRect();


    if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        for (Object *o : objects) {
            graphics::Rect r = o->displayRect();
            r.x -= scrollPosX;
            r.x += getX() + parentRect.x;
            r.y -= scrollPosY;
            r.y += getY() + parentRect.y;
            if (r.intersects(pInput->getMousePostion())) {
                fireFuncionCall("selectItem", i);
                break;
            }
            i++;

        }
        buttonPressed = true;
    } else if (pInput->isMouseButtonUp(SDL_BUTTON_LEFT)) {
        buttonPressed = false;
    }
    if (buttonPressed) {
        graphics::Rect destRect;


        destRect.width = 14;
        destRect.height = 14;
        destRect.x = parentRect.x + getX();
        destRect.y = parentRect.y + renderRect.height + getY();

        if (destRect.intersects(pInput->getMousePostion())) {
            std::cout << " left edge clicked" << std::endl;
        }

        //render right edge
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        if (destRect.intersects(pInput->getMousePostion())) {
            std::cout << " right edge clicked" << std::endl;
        }
        //render bar
        destRect.width = renderRect.width - 28;
        destRect.height = 5;
        destRect.x = parentRect.x + 14 + getX();
        destRect.y = parentRect.y + renderRect.height + 4 + getY();
        if (destRect.intersects(pInput->getMousePostion())) {
            std::cout << " bar clicked" << std::endl;
        }

        //check horizontal

        destRect.width = 14;
        destRect.height = 14;
        destRect.x = parentRect.x + getX() + renderRect.width - 14;
        destRect.y = parentRect.y + getY();
        if (destRect.intersects(pInput->getMousePostion())) {
            if (scrollPosY > 0) {
                scrollPosY -= round(
                        (scrollHeight - renderRect.height - 14) / 100);
                if (scrollPosY < 0)
                    scrollPosY = 0;
            }
        }
        //render right edge
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        destRect.y = parentRect.y + renderRect.height + getY() - 14;
        if (destRect.intersects(pInput->getMousePostion())) {
            if (scrollPosY < scrollHeight - renderRect.height - 14)
                scrollPosY += round(
                        (scrollHeight - renderRect.height - 14) / 100);
        }
        //render bar

        destRect.width = 4;
        destRect.height = renderRect.height - 24;
        destRect.x = parentRect.x + renderRect.width - 8 + getX();
        destRect.y = parentRect.y + 14 + getY();
        if (destRect.intersects(pInput->getMousePostion())) {
            std::cout << " bar 2 clicked" << std::endl;
        }
        //render slider
        destRect.width = 14;
        destRect.height = 10;
        destRect.x = parentRect.x + renderRect.width - 14 + getX();
        destRect.y = parentRect.y + 14 + getY();


    }


    graphics::Rect r = renderRect;
    r.x = getX() + parentRect.x;
    r.y = getY() + parentRect.y;
    if (pInput->isScrollWheel()) {
        if (!r.intersects(pInput->getMousePostion()))
                return;
        utils::Vector2 pos = pInput->getMouseWheelPosition();
        if (scrollWidth - renderRect.width > 0) {
            if ((scrollPosX + (pos.getY() * 5) >= 0 && pos.getY() == -1)
                    || (pos.getY() == 1
                            && scrollPosX < scrollWidth - renderRect.width - 14)) {
                scrollPosX += pos.getY() * 5;
                std::cout << "wheel x: " << pos.getX() << " y: " << pos.getY()
                        << " scrollPosX: " << scrollPosX << std::endl;
            }
        } else {
            if ((scrollPosY + (pos.getY() * 5) >= 0 && pos.getY() == -1)
                    || (pos.getY() == 1
                            && scrollPosY
                                    < scrollHeight - renderRect.height - 14)) {
                scrollPosY += pos.getY() * 5;
                std::cout << "wheel y: " << pos.getY() << " x: " << pos.getX()
                        << " scrollPosY: " << scrollPosY << std::endl;
            }

        }
    }

}

} // namespace UI

