#include "engine/ui/ComboBox.h"

namespace UI
{

ComboBox::ComboBox(UI::Object *parent) :
        UI::Object(parent), selection(0)
{
    width = 100;
    //texture = new CTexture("ArkanaLook.png");
    //border = texture->subSurface(7,217,6,6);
    //background = texture->subSurface(250,80,50,50);
    //border->zoomImage(width/6.0,28.0/6.0);
    mouseDown = false;
}

ComboBox::~ComboBox()
{
}

void ComboBox::handleEvents(core::Input *pInput)
{
    SDL_Event e = pInput->getEvent();
    int tx = getX();

    int ty = getY();

    if (this->getParent()) {
        graphics::Rect dsp = getParent()->displayRect();
        tx += dsp.x;
        ty += dsp.y;
    }
    int height = 28;
    if (mouseDown && elements.size() > 0)
    {
        height *= elements.size();
    }
    if (e.button.x >= tx && e.button.x <= tx + width + 28 && e.button.y >= ty
            && e.button.y <= ty + height)
    {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {

            mouseDown = true;
        }
        else if (e.type == SDL_MOUSEBUTTONUP
                && e.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = false;
            selection = (e.button.y - ty) / 28;
        }
    }
    else
    {
        mouseDown = false;
    }
}

void ComboBox::render(core::Renderer *pRender, graphics::Texture *pTexture)
{

    int tx = getX();

    int ty = getY();


    if (this->getParent()) {
        graphics::Rect dsp = getParent()->displayRect();
        tx += dsp.x;
        ty += dsp.y;
    }

    //std::cout <<label<<" tx: "<<tx<<" ty:"<<ty<<std::endl;
    pTexture->render(pRender, tx + width, ty, 28, 28, 97, 258);
    // border->draw(pRender,tx,ty,2,2);
    graphics::Rect src;
    graphics::Rect dest;

    double height = 28.0;
    double bgwidth = width;
    height *= elements.size();
    SDL_Color color = { 0, 0, 0, 0 };

    if (mouseDown && elements.size() > 0) {
        src.x = 250;
        src.y = 80;
        src.width = 50;
        src.height = 50;
        dest.x = tx;
        dest.y = ty;
        dest.width = bgwidth;
        dest.height = height;
        pTexture->render(pRender, src, dest);

        for (unsigned int i = 0; i < elements.size(); ++i)
            getFont()->render(pRender, elements[i], color, tx + 3, ty + 28 * i);
    }
    if (selection >= 0 && elements.size() > 0 && !mouseDown) {
        src.x = 250;
        src.y = 80;
        src.width = 50;
        src.height = 50;
        dest.x = tx;
        dest.y = ty;
        dest.width = bgwidth;
        dest.height = 28;
        pTexture->render(pRender, src, dest);
        getFont()->render(pRender, elements[selection], color, tx + 3, ty + 3);
    }
//    for (int i =2; i<height;i+=2){
//        border->draw(pRender,tx,ty+i,2,2,0,2);
//        border->draw(pRender,tx+width-2,ty+i,2,2,0,2);
//        //border->draw(tx+width-2,ty+i,2,2,4,2);
//    }
//    for (int i = 2;i<width-2;i+=2){
//        border->draw(pRender,tx+i,ty,2,2,2,0);
//        border->draw(pRender,tx+i,ty+height,2,2,2,0);
//    }

}

} // namespace UI
