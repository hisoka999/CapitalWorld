#include "engine/graphics/sprite.h"
#include <iostream>

namespace graphics {

Sprite::Sprite(Texture* pTexture, float pX, float pY, int pRows, int pCols,
    int pSpriteNum)
    : GameObject(pTexture, pX, pY)
    , rows(pRows)
    , cols(pCols)
    , spriteNum(
          pSpriteNum)
{
    sprite_width = pTexture->getWidth() / pCols;
    sprite_height = pTexture->getHeight() / pRows;
    direction = SpriteDirection::TOP;
    animFrame = 1.0;
    lastTime = 0;
    animActive = false;
    moveWithCamera = true;
    collisionArea = { false, false, false, false };
}

Sprite::~Sprite()
{
    //dtor
}

void Sprite::render(core::Renderer* pRender)
{
    graphics::Rect srcRect, destRect;

    if (sprite_width == 0) {
        sprite_width = getTexture()->getWidth() / cols;
        sprite_height = getTexture()->getHeight() / rows;
    }
    if (animActive)
        lastTime += pRender->getTimeDelta();
    if (lastTime >= 200) {
        lastTime = 0;
        animFrame += 1.0f;
        if (animFrame > 2)
            animFrame = 0;
    }

    int spriteRow = spriteNum / (rows / 2);
    int spriteCol = (spriteNum - (spriteRow * (cols / 3)));
    int dir = (int)direction;

    int dir_x = ((spriteCol * 3) + abs(animFrame)) * sprite_width;
    int dir_y = sprite_height * (dir + spriteRow * 4);
    float x = getX()
        - ((moveWithCamera) ? pRender->getMainCamera()->getX() : 0);
    float y = getY()
        - ((moveWithCamera) ? pRender->getMainCamera()->getY() : 0);
    float zoom = pRender->getZoomFactor();

    srcRect.x = dir_x;
    srcRect.y = dir_y;
    srcRect.width = sprite_width;
    srcRect.height = sprite_height;

    destRect.x = x * zoom;
    destRect.y = y * zoom;
    destRect.width = sprite_width * zoom;
    destRect.height = sprite_height * zoom;
    if (x >= 0 && y >= 0)
        this->getTexture()->render(pRender, srcRect, destRect);
}
void Sprite::startAnimation()
{
    animActive = true;
    lastTime = 0;
}
void Sprite::stopAnimation()
{
    animActive = false;
    animFrame = 1.0;
}
bool Sprite::isAnimationRunning()
{
    return animActive;
}

Rect Sprite::collisionRect()
{
    Rect r;
    r.x = getX();
    r.y = getY();
    r.width = sprite_width;
    r.height = sprite_height;
    return r;
}

} // namespace graphics

void graphics::Sprite::update(double deltaTime)
{
}

graphics::Sprite::Sprite(const Sprite& pOriginal)
    : GameObject(pOriginal)
{
    rows = pOriginal.rows;
    cols = pOriginal.cols;

    sprite_width = pOriginal.sprite_width;
    sprite_height = pOriginal.sprite_height;
    direction = pOriginal.direction;
    animFrame = 1.0;
    lastTime = 0;
    animActive = false;
    moveWithCamera = true;
    collisionArea = { false, false, false, false };
    spriteNum = pOriginal.spriteNum;
    std::cout << "sprite was copied" << std::endl;
}
