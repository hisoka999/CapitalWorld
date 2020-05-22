#ifndef GAMEMAPRENDERER_H
#define GAMEMAPRENDERER_H

#include "world/gamemap.h"
#include <memory>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/TextureManager.h>

class GameMapRenderer
{
public:
    GameMapRenderer(std::shared_ptr<GameMap> gameMap);


    void render(core::Renderer* renderer);
    size_t getTileWidth();
    size_t getTileHeight();
    float getTileYOffset(uint16_t tile,size_t tileX,size_t tileY);


private:
    void renderTile(core::Renderer* renderer,uint16_t tile,int tileX,int tileY,const utils::Vector2& pos);
    graphics::Rect getSourceRect(TileType tile,size_t tileX,size_t tileY);
    std::shared_ptr<GameMap> gameMap;
    std::shared_ptr<graphics::Texture> groundTexture;
    std::shared_ptr<graphics::Text> debugText;
    size_t tileWidth;
    size_t tileHeight;

};

#endif // GAMEMAPRENDERER_H
