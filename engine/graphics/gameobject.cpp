#include "engine/graphics/gameobject.h"

namespace graphics
{

GameObject::GameObject(Texture *pTexture, float pX, float pY) :
        _texture(pTexture), x(pX), y(pY), width(0), height(0)
{
    //ctor
}

GameObject::~GameObject()
{
    //dtor
}
void GameObject::render(core::Renderer *ren, int width, int height)
{
    this->width = width;
    this->height = height;
    _texture->renderResized(ren, static_cast<int>(x), static_cast<int>(y),
            width, height);
}

void GameObject::registerLeftClick(std::function<void(void)> handler_func)
{
    _leftClickFunction = handler_func;
}

void GameObject::registerRightClick(std::function<void(void)> handler_func)
{
    _rightClickFunction = handler_func;
}

void GameObject::handleEvents(core::Input *input)
{
    utils::Vector2 v = input->getMousePostion();
    if (collisionRect().intersects(v))
    {
        if (input->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            if (_leftClickFunction)
                _leftClickFunction();
        }
        else if (input->isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            if (_rightClickFunction)
                _rightClickFunction();
        }
    }

}

Rect GameObject::collisionRect()
{
    Rect r;
    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
    return r;
}

void GameObject::move(float pX, float pY)
{
    x += pX;
    y += pY;
}
GameObject::GameObject(const GameObject& pOriginal) {
    _texture = pOriginal._texture;
    x = pOriginal.x;
    y = pOriginal.y;
}

} // namespace graphics


