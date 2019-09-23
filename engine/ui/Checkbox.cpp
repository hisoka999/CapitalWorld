#include "engine/ui/Checkbox.h"

namespace UI {

Checkbox::Checkbox(UI::Object *parent) : UI::Object(parent)
{
    textCore = new graphics::Text();
    textCore->openFont("arial.ttf",12);
    checked = false;
    color = {0,0,0};
}

Checkbox::~Checkbox()
{
    //dtor
}

void Checkbox::handleEvents(core::Input *pInput)
{
    SDL_Event e = pInput->getEvent();
    int tx;
    int ty;
    int textwidth = 0;
    if (getParent() != NULL){
        tx = getParent()->getX() + getX();
        ty = getParent()->getY() + getY();
    }
    else{
        tx = getX();
        ty = getY();
    }

    if (e.button.x >= tx && e.button.x <= tx+textwidth+25
        && e.button.y >= ty && e.button.y <= ty+23 ){
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            std::cout<<"toggle checked" <<std::endl;
            toggleChecked();
        }
    }
}

void Checkbox::render(core::Renderer *pRender,graphics::Texture *pTexture)
{

    int tx = getX();
    int ty = getY();
    if (this->getParent()){
        tx += this->getParent()->getX();
        ty += this->getParent()->getY();
    }
    int x2 = 0;
    if (checked)
        x2 = 25;
    pTexture->render(pRender,tx,ty,25,23,x2,260);
    textCore->render(pRender,text,color,tx+30,ty);
}

} // namespace UI
