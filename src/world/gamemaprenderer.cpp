#include "gamemaprenderer.h"
#include <engine/graphics/rect.h>
#include <engine/utils/os.h>
#include <engine/utils/string.h>

GameMapRenderer::GameMapRenderer(std::shared_ptr<GameMap> gameMap)
    : gameMap(gameMap), tileWidth(64), tileHeight(32)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "landscape.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 10);
}

graphics::Rect GameMapRenderer::getSourceRect(TileType tile, size_t tileX, size_t tileY)
{
    graphics::Rect srcRect = {0, 0, static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

    int groundLimit = 10;

    if (tile < 8)
    {
        //render water
        srcRect.x = 64;
        srcRect.y = 0;
    }
    else
    {
        srcRect.x = 0;
        srcRect.y = 0;
    }

    return srcRect;
}

float GameMapRenderer::getTileYOffset(uint16_t tile, size_t tileX, size_t tileY)
{
    graphics::Rect srcRect = getSourceRect(tile, tileX, tileY);
    return tileHeight - srcRect.height;
}

void GameMapRenderer::renderTile(core::Renderer *renderer, uint16_t tile, int tileX, int tileY, const utils::Vector2 &pos)
{
    auto camera = renderer->getMainCamera();
    float xPos = pos.getX() - camera->getX();
    float yPos = pos.getY() - camera->getY();

    float width = static_cast<float>(tileWidth) * renderer->getZoomFactor();
    float height = static_cast<float>(tileHeight) * renderer->getZoomFactor();

    graphics::Rect srcRect = getSourceRect(tile, tileX, tileY);
    graphics::Rect destRect = {std::round((pos.getX() * renderer->getZoomFactor()) - camera->getX()), std::round(((pos.getY() + (tileHeight - srcRect.height)) * renderer->getZoomFactor()) - camera->getY()), srcRect.width * renderer->getZoomFactor(), srcRect.height * renderer->getZoomFactor()};
    graphics::Rect realRect = {(pos.getX() * renderer->getZoomFactor()), (pos.getY() + (tileHeight - srcRect.height)) * renderer->getZoomFactor(), srcRect.width * renderer->getZoomFactor(), srcRect.height * renderer->getZoomFactor()};

    if (!realRect.intersects(camera->getViewPortRect()))
    {
        return;
    }
    groundTexture->render(renderer, srcRect, destRect);
}

void GameMapRenderer::render(core::Renderer *renderer)
{
    SDL_Color color = {0, 0, 0, 255};
    for (size_t j = 0; j < gameMap->getWidth(); ++j)
    {
        for (size_t i = 0; i < gameMap->getHeight(); ++i)
        {
            float x = static_cast<float>(i) * tileWidth / 2.0f;
            float y = static_cast<float>(j) * tileHeight;
            utils::Vector2 vec(x, y);
            auto iso = gameMap->twoDToIso(vec);

            renderTile(renderer, gameMap->getTile(i, j), i, j, iso);

            //debugText->render(renderer,utils::string_format("%d/%d",i,j),color,iso.getX()+5,iso.getY()+5);
        }
    }
    auto camera = renderer->getMainCamera();

    for (auto &building : gameMap->getBuildings())
    {
        auto displayRect = building->getDisplayRect();

        displayRect.x = (displayRect.x * tileWidth / 2.0f);
        displayRect.y = (displayRect.y * tileHeight);
        utils::Vector2 vec(displayRect.x, displayRect.y);
        TileType tile = gameMap->getTile(building->getDisplayRect().x, building->getDisplayRect().y);

        auto pos = gameMap->twoDToIso(vec);

        displayRect.x = std::round(((pos.getX() + building->getXOffset()) * renderer->getZoomFactor()) - camera->getX());
        displayRect.width = displayRect.width * renderer->getZoomFactor();
        displayRect.height = displayRect.height * renderer->getZoomFactor();

        float tileYOffset = getTileYOffset(tile, building->getDisplayRect().x, building->getDisplayRect().y);

        displayRect.y = std::round(((pos.getY() + building->getYOffset()) * renderer->getZoomFactor()) - (camera->getY() + tileYOffset));
        groundTexture->render(renderer, building->getSourceRect(), displayRect);
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
