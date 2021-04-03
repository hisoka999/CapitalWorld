#include "engine/ui/Window.h"
#include <functional>

namespace UI
{

    int Window::windowCount = 0;

    Window::Window()
        : Window(50, 50, 300, 400)
    {
        //addObject()
    }

    Window::Window(int x, int y, int width, int height)
        : width(width), height(height)
    {
        setX(x);
        setY(y);
        visible = false;
        windowCount++;

        closeButton = std::make_shared<UI::Button>(this);
        closeButton->setX(width - 23 - 5 - 16);
        closeButton->setY(7 - 40);
        closeButton->setFont("fonts/fa-solid-900.ttf", 16);
        closeButton->setLabel("\uf00d");
        closeButton->setBorderless(true);
        SDL_Color buttonColor = {195, 129, 42, 255};
        closeButton->setColor(buttonColor);

        this->addObject(closeButton);
        closeButton->connect("buttonClick", [&]() { buttonClick(); });
        title = "Demo";
    }

    void Window::handleEvents(core::Input *pInput)
    {
        if (visible)
            UI::Container::handleEvents(pInput);
    }

    void Window::buttonClick()
    {
        this->setVisible(false);
    }

    void Window::render(core::Renderer *pRender)
    {
        if (visible)
        {

            //draw top
            graphics::Rect topRect;
            topRect.x = getX();
            topRect.y = getY();
            topRect.width = width;
            topRect.height = 2;
            pRender->setDrawColor(195, 129, 42, 255);
            pRender->fillRect(topRect);
            pRender->drawRect(topRect);

            graphics::Rect backgroundRect;
            backgroundRect.x = getX();
            backgroundRect.y = getY() + topRect.height;
            backgroundRect.width = width;
            backgroundRect.height = height - topRect.height;
            pRender->setDrawColor(18, 26, 32, 255);

            pRender->fillRect(backgroundRect);
            pRender->drawRect(backgroundRect);
            //draw title
            SDL_Color titleColor = {195, 129, 42, 255};
            getFont()->render(pRender, title, titleColor, backgroundRect.x + 16, backgroundRect.y + 5);

            pRender->setDrawColor(93, 103, 108, 255);
            utils::Vector2 start(backgroundRect.x + 16, getY() + 29);
            utils::Vector2 end(backgroundRect.x + width - 16, getY() + 29);
            pRender->drawLine(start, end);

            UI::Container::render(pRender);
        }
    }

    Window::~Window()
    {
        UI::Container::clear();
        closeButton = nullptr;
    }

    graphics::Rect Window::displayRect()
    {
        graphics::Rect r;
        r.x = getX() + 16;
        r.y = getY() + 30;
        r.width = width - 5;
        r.height = height - 40;
        return r;
    }

    graphics::Rect Window::eventRect()
    {
        return displayRect();
    }

    void Window::clear()
    {
        UI::Container::clear();
        addObject(closeButton);
    }

} // namespace UI
