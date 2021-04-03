#include "engine/ui/scrollarea.h"
#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <iostream>

namespace UI {

ScrollArea::ScrollArea(const unsigned int pWidth, const unsigned pHeight,
    Object* parent = 0)
    : Object(parent)
    , scrollWidth(0)
    , scrollHeight(0)
    , scrollX(0)
    , scrollY(0)
    , scrollPosX(
          0)
    , scrollPosY(0)
    , buttonPressed(false)
{
    renderRect.x = 0;
    renderRect.y = 0;
    renderRect.width = pWidth;
    renderRect.height = pHeight;
    renderArea = nullptr;

    uiText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 12);
}

ScrollArea::~ScrollArea()
{
    //dtor
    delete renderArea;
}

void ScrollArea::reset()
{
    scrollPosX = 0;
    scrollPosY = 0;
    delete renderArea;
    renderArea = nullptr;
}

void ScrollArea::render(core::Renderer* pRender)
{
    graphics::Rect parentRect;
    if (getParent() != nullptr)
        parentRect = getParent()->displayRect();

    //render scroolbar
    //82 354
    int lastScrollWidth = scrollWidth;
    int lastScrollHeight = scrollHeight;
    scrollWidth = 0;
    scrollX = 0;
    for (auto o : objects) {
        graphics::Rect r = o->displayRect();
        if (scrollWidth < r.x + r.width) {
            scrollWidth = r.x + r.width;
            scrollX = r.x;
        }

        if (scrollHeight < r.y + r.height) {
            scrollHeight = r.y + r.height;
            scrollY = r.y;
        }
    }
    if (scrollWidth == 0)
        scrollWidth = renderRect.width;
    if (scrollHeight == 0)
        scrollHeight = renderRect.height;

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
        std::for_each(objects.begin(), objects.end(), [&](const std::shared_ptr<Object> o) {
            o->render(pRender);
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

    pRender->setDrawColor(93, 103, 108, 255);
    graphics::Rect borderRect;
    borderRect.x = parentRect.x + getX();
    borderRect.y = parentRect.y + getY();
    borderRect.width = renderRect.width;
    borderRect.height = renderRect.height;

    pRender->drawRect(borderRect);
    //TODO render scrollbars
    if (renderArea->getHeight() > renderRect.height) {
        graphics::Rect scrollbarRect;
        scrollbarRect.width = 10;
        scrollbarRect.x = parentRect.x + getX() + renderRect.width - scrollbarRect.width - 1;
        scrollbarRect.y = parentRect.y + getY() + 2;

        scrollbarRect.height = renderRect.height - 2;

        pRender->drawRect(scrollbarRect);
        SDL_Color uiColor = { 93, 103, 108, 255 };
        uiText->render(pRender, "\uf0d7", uiColor, scrollbarRect.x + 2, scrollbarRect.y + scrollbarRect.height - 14);

        uiText->render(pRender, "\uf0d8", uiColor, scrollbarRect.x + 2, scrollbarRect.y + 2);
    }
}

graphics::Rect ScrollArea::displayRect()
{
    return renderRect;
}

graphics::Rect ScrollArea::eventRect()
{
    graphics::Rect r;
    r.x = renderRect.x + getX();
    r.y = renderRect.y + getY();
    r.width = renderRect.width;
    r.height = renderRect.height;
    if (getParent() != nullptr) {
        r.x += getParent()->eventRect().x;
        r.y += getParent()->eventRect().y;
    }
    return r;
}

void ScrollArea::handleEvents(core::Input* pInput)
{
    reset();
    Container::handleEvents(pInput);
    int i = 0;

    graphics::Rect parentRect;
    if (getParent() != nullptr)
        parentRect = getParent()->displayRect();

    if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        for (std::shared_ptr<Object> o : objects) {
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
                scrollPosY -= std::round(
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
                scrollPosY += std::round(
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
