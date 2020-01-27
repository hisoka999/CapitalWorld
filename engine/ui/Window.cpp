#include "engine/ui/Window.h"
#include <functional>

namespace UI
{

int Window::windowCount = 0;

Window::Window() :
        Window(50, 50, 300, 400)
{
    //addObject()
}

Window::Window(int x, int y, int width, int height) :
        width(width), height(height)
{
    setX(x);
    setY(y);
    visible = false;
    windowCount++;
    //base = new CTexture("ArkanaLook.png");
    //bg_texture = base->subSurface(75,80,100,100);
    //bg_texture->zoomImage(width/100.0,height/100.0);

    closeButton = new UI::ImageButton(this, 20, 20, 75, 60);
    closeButton->setX(width - 23 - 5);
    closeButton->setY(10 - 40);

    this->addObject(closeButton);
    //closeButton->setListener(this);
    //closeButton->connect("buttonClick", std::bind(&Window::buttonClick, this));
    closeButton->connect("buttonClick", [&]()
    {	buttonClick();});
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

void Window::render(core::Renderer *pRender, graphics::Texture *pTexture)
{
    if (visible)
    {
        if (!closeButton->imageLoaded())
            closeButton->setImage(std::shared_ptr<graphics::Texture>(pTexture));
        // draw background
        int x = getX();
        int y = getY();
        {
            graphics::Rect src;
            graphics::Rect dest;
            //background = texture->subSurface(20,420,10,35);
            src.x = 75;
            src.y = 80;
            src.width = 100;
            src.height = 100;

            dest.x = x + 3;
            dest.y = y + 5;
            dest.width = width - 3;
            dest.height = height - 3;
            pTexture->render(pRender, src, dest);
        }
        // draw the window border
        int i = 0;

        pTexture->render(pRender, x, y, 10, 35, 73, 0);
        {
            graphics::Rect src;
            graphics::Rect dest;

            src.x = 105;
            src.y = 0;
            src.width = 10;
            src.height = 35;

            dest.x = x + 10;
            dest.y = y;
            dest.width = width - 46;
            dest.height = src.height;
            pTexture->render(pRender, src, dest);
        }

        //for (i = x + 10; i < x + width - 46; i += 10)
        //	pTexture->render(pRender, i, y, 10, 35, 105, 0);

        // close button background
        pTexture->render(pRender, x + width - 40, y, 46, 45, 145, 0);

        // left border
        for (i = y + 35; i < y + height - 10; i += 10)
        {
            pTexture->render(pRender, x, i, 10, 10, 73, 35);
            pTexture->render(pRender, x + width, i, 10, 12, 73, 35);
        }

        pTexture->render(pRender, x, i, 10, 12, 73, 45);
        // bottom border
        {
            graphics::Rect src;
            graphics::Rect dest;

            src.x = 79;
            src.y = 47;
            src.width = 9;
            src.height = 10;

            dest.x = x + 10;
            dest.y = y + height - 3;
            dest.width = width - 9;
            dest.height = src.height;
            pTexture->render(pRender, src, dest);
        }

        //for (i = x + 10; i < x + width - 9; i += 9)
        //	pTexture->render(pRender, i, y + height - 3, 9, 10, 79, 47);
        pTexture->render(pRender, x + width - 1, y + height - 3, 8, 10, 184,
                47);
        Container::render(pRender, pTexture);
        //pRender->printText(x+10,y+7,title);
        SDL_Color color =
        { 0, 0, 0 ,255};
        getFont()->render(pRender, title, color, x + 10, y + 7);
    }
}

Window::~Window()
{
    delete closeButton;
}

graphics::Rect Window::displayRect()
{
    graphics::Rect r;
    r.x = getX() + 5;
    r.y = getY() + 40;
    r.width = width - 5;
    r.height = height - 40;
    return r;
}
void Window::clear() {
    UI::Container::clear();
    addObject(closeButton);
}

} // namespace UI
