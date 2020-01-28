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
    int getTileWidth();
    int getTileHeight();

private:
    void renderTile(core::Renderer* renderer,uint16_t tile,int tileX,int tileY,const utils::Vector2& pos);
    graphics::Rect getSourceRect(TileType tile,int tileX,int tileY);
    std::shared_ptr<GameMap> gameMap;
    std::shared_ptr<graphics::Texture> groundTexture;
    std::shared_ptr<graphics::Text> debugText;
    int tileWidth;
    int tileHeight;

};

#endif // GAMEMAPRENDERER_H
