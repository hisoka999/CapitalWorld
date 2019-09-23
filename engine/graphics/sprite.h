#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H

#include <engine/graphics/gameobject.h>
#include <engine/utils/vector2.h>

namespace graphics
{

enum class SpriteDirection
    : int
    {
        TOP = 3, BOTTOM = 0, RIGHT = 2, LEFT = 1, NONE = 4
};

struct Collision {
    bool left;
    bool right;
    bool up;
    bool down;
};

class Sprite: public graphics::GameObject
{
public:
    /** Default constructor */
    Sprite(Texture *pTexture, float pX, float pY, int pRows, int pCols,
            int pSpriteNum);

    Sprite(const Sprite& pOriginal);
    /** Default destructor */
    virtual ~Sprite();

    virtual void render(core::Renderer *pRender);
    void setDirection(SpriteDirection pDir)
    {
        direction = pDir;
    }
    void startAnimation();
    void stopAnimation();
    void collision(bool left, bool right, bool up, bool down) {
        collisionArea.left = left;
        collisionArea.right = right;
        collisionArea.down = down;
        collisionArea.up = up;
    }
    bool isAnimationRunning();
    virtual Rect collisionRect();
    virtual void update(double deltaTime);


protected:
    SpriteDirection direction;
    bool moveWithCamera;
    unsigned int sprite_width;
    unsigned int sprite_height;
    Collision collisionArea;

private:
    int rows;
    int cols;
    unsigned int spriteNum;
    float animFrame;
    int lastTime;
    bool animActive;

};

} // namespace graphics

#endif // GRAPHICS_SPRITE_H
