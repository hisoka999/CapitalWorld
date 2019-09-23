#ifndef GRAPHICS_GAMEOBJECT_H
#define GRAPHICS_GAMEOBJECT_H

#include <engine/graphics/texture.h>
#include <engine/graphics/rect.h>
#include <engine/core/input.h>
#include <engine/core/renderer.h>
#include <functional>

namespace graphics
{

class GameObject
{
public:
    /** Default constructor */
    GameObject(Texture *pTexture, float pX, float pY);

    GameObject(const GameObject& pOriginal);
    /** Default destructor */
    virtual ~GameObject();
    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
    void render(core::Renderer *ren, int width, int height);
    virtual void handleEvents(core::Input *input);
    void registerLeftClick(std::function<void(void)> handler_func);
    void registerRightClick(std::function<void(void)> handler_func);
    Texture* getTexture()
    {
        return _texture;
    }
    ;
    virtual Rect collisionRect();
    virtual void move(float pX, float pY);
    void setX(float pX)
    {
        x = pX;
    }
    void setY(float pY)
    {
        y = pY;
    }
protected:

private:
    Texture* _texture;
    float x, y;
    int width, height;
    std::function<void(void)> _leftClickFunction;
    std::function<void(void)> _rightClickFunction;
};

} // namespace graphics

#endif // GRAPHICS_GAMEOBJECT_H
