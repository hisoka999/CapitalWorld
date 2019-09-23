#include "gamemaprenderer.h"
#include <engine/utils/os.h>
#include <engine/graphics/rect.h>
#include <engine/utils/string.h>

GameMapRenderer::GameMapRenderer(std::shared_ptr<GameMap> gameMap): gameMap(gameMap),tileWidth(58),tileHeight(28)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","tiles","iso_tiles.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts","arial.ttf"),10);
}

void GameMapRenderer::renderTile(core::Renderer* renderer,uint16_t tile,const utils::Vector2& pos)
{
    //TODO support other tiles
    auto camera = renderer->getMainCamera();
    float xPos = pos.getX()-camera->getX();
    float yPos = pos.getY()-camera->getY();
    graphics::Rect srcRect = {0,0,static_cast<float>(tileWidth),static_cast<float>(tileHeight+1)};
    graphics::Rect destRect = {xPos,yPos,static_cast<float>(tileWidth),static_cast<float>(tileHeight+1)};

    groundTexture->render(renderer,srcRect,destRect);
}

void GameMapRenderer::render(core::Renderer* renderer)
{
    SDL_Color color = {0,0,0,255};
    for(size_t i = 0; i< gameMap->getWidth();++i)
    {
        for(size_t j = 0;j < gameMap->getHeight();++j )
        {
            float x = static_cast<float>(j) * tileWidth/2.0f;
            float y = static_cast<float>(i) * tileHeight;
            utils::Vector2 vec(x,y);
            auto iso = gameMap->twoDToIso(vec);
            renderTile(renderer,gameMap->getTile(i,j), iso);

            //debugText->render(renderer,utils::string_format("%d/%d",i,j),color,iso.getX()+5,iso.getY()+5);
        }
    }
    auto camera = renderer->getMainCamera();

    for(auto& building : gameMap->getBuildings())
    {
        auto displayRect = building->getDisplayRect();

        displayRect.x = (displayRect.x * tileWidth/2.0f) ;
        displayRect.y = (displayRect.y * tileHeight);
        utils::Vector2 vec(displayRect.x,displayRect.y);
        auto pos = gameMap->twoDToIso(vec);
        displayRect.x = pos.getX()- camera->getX();
        displayRect.y = pos.getY() - camera->getY();
        groundTexture->render(renderer,building->getSourceRect(),displayRect);
    }
}

int GameMapRenderer::getTileWidth()
{
    return tileWidth;
}
int GameMapRenderer::getTileHeight()
{
    return tileHeight;
}
