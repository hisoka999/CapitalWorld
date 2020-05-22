#include "gamemaprenderer.h"
#include <engine/utils/os.h>
#include <engine/graphics/rect.h>
#include <engine/utils/string.h>

GameMapRenderer::GameMapRenderer(std::shared_ptr<GameMap> gameMap): gameMap(gameMap),tileWidth(58),tileHeight(29)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","tiles","iso_tiles.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts","arial.ttf"),10);
}

graphics::Rect GameMapRenderer::getSourceRect(TileType tile,size_t tileX,size_t tileY)
{
    graphics::Rect srcRect = {0,0,static_cast<float>(tileWidth),static_cast<float>(tileHeight+2)};

    int groundLimit = 10;

    if(tile< 8){
        //render water
        srcRect.x = 116;
        srcRect.y = 100;
        srcRect.height = 29;
    }else if(tile > groundLimit){
        //render mountain


        srcRect.height = 46;
        srcRect.x = 986;
        srcRect.y = 100;

    }else{
        //ground
        srcRect.height = 50;
        if(gameMap->getTile(tileX-1,tileY) > groundLimit &&
                gameMap->getTile(tileX,tileY-1) > groundLimit){
            srcRect.x = 754;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX-1,tileY) > groundLimit &&
                 gameMap->getTile(tileX,tileY+1) > groundLimit){
             srcRect.x = 696;
             srcRect.y = 0;
         }else if(gameMap->getTile(tileX,tileY-1) > groundLimit){
            srcRect.x = 290;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX,tileY+1) > groundLimit){
            srcRect.x = 348;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX-1,tileY) > groundLimit){
            srcRect.x = 232;
            srcRect.y = 0;

        }else if(gameMap->getTile(tileX+1,tileY) > groundLimit){
            srcRect.x = 406;
            srcRect.y = 0;
        }else
        if(gameMap->getTile(tileX-1,tileY-1) > groundLimit){
            srcRect.x = 464;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX-1,tileY+1) > groundLimit){
            srcRect.x = 580;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX+1,tileY+1) > groundLimit){
            srcRect.x = 640;
            srcRect.y = 0;
        }else if(gameMap->getTile(tileX+1,tileY-1) > groundLimit){
            srcRect.x = 522;
            srcRect.y = 0;
        }else{
            srcRect.height = tileHeight+2;
            srcRect.x = 0;
            srcRect.y = 0;
        }

    }

    return srcRect;
}

float GameMapRenderer::getTileYOffset(uint16_t tile,size_t tileX,size_t tileY)
{
    graphics::Rect srcRect = getSourceRect(tile,tileX,tileY);
    return tileHeight - srcRect.height;
}

void GameMapRenderer::renderTile(core::Renderer* renderer,uint16_t tile,int tileX,int tileY,const utils::Vector2& pos)
{
    auto camera = renderer->getMainCamera();
    float xPos = pos.getX()-camera->getX();
    float yPos = pos.getY()-camera->getY();
    graphics::Rect srcRect = getSourceRect(tile,tileX,tileY);
    graphics::Rect destRect = {xPos,yPos+(tileHeight-srcRect.height),srcRect.width,srcRect.height};

    groundTexture->render(renderer,srcRect,destRect);
}

void GameMapRenderer::render(core::Renderer* renderer)
{
    SDL_Color color = {0,0,0,255};
    for(size_t j = 0; j< gameMap->getWidth();++j)
    {
        for(size_t i = 0;i < gameMap->getHeight();++i )
        {
            float x = static_cast<float>(i) * tileWidth/2.0f;
            float y = static_cast<float>(j) * tileHeight;
            utils::Vector2 vec(x,y);
            auto iso = gameMap->twoDToIso(vec);
            renderTile(renderer,gameMap->getTile(i,j),i,j, iso);

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
        TileType tile = gameMap->getTile(building->getDisplayRect().x,building->getDisplayRect().y);

        auto pos = gameMap->twoDToIso(vec);

        displayRect.x = pos.getX()- camera->getX()+building->getXOffset();

        float tileYOffset = getTileYOffset(tile,building->getDisplayRect().x,building->getDisplayRect().y);


        displayRect.y = pos.getY() + building->getYOffset() - (camera->getY() + tileYOffset);
        groundTexture->render(renderer,building->getSourceRect(),displayRect);

    }
}

size_t GameMapRenderer::getTileWidth()
{
    return tileWidth;
}
size_t GameMapRenderer::getTileHeight()
{
    return tileHeight;
}
