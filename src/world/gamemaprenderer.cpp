#include "gamemaprenderer.h"
#include <engine/graphics/rect.h>
#include <engine/utils/os.h>
#include <engine/utils/string.h>

GameMapRenderer::GameMapRenderer(std::shared_ptr<GameMap> gameMap)
    : gameMap(gameMap), tileWidth(64), tileHeight(32)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "landscape.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 10);

    textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.tm"));
    cacheTexture = nullptr;
}

graphics::Rect GameMapRenderer::getSourceRect(TileType tile, size_t tileX, size_t tileY)
{
    graphics::Rect srcRect = {0, 0, static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

    const int groundLimit = 8;
    TileType leftTile = gameMap->getTile(tileX, tileY + 1);
    TileType rightTile = gameMap->getTile(tileX, tileY - 1);
    TileType topTile = gameMap->getTile(tileX - 1, tileY);
    TileType bottomTile = gameMap->getTile(tileX + 1, tileY);

    if (tile < groundLimit)
    {
        if (rightTile >= groundLimit && topTile >= groundLimit)
        {
            srcRect.x = 896;
            srcRect.y = 0;
        }
        else if (leftTile >= groundLimit && topTile >= groundLimit)
        {
            srcRect.x = 960;
            srcRect.y = 0;
        }
        else if (rightTile >= groundLimit && bottomTile >= groundLimit)
        {
            srcRect.x = 960;
            srcRect.y = 32;
        }
        else if (leftTile >= groundLimit && bottomTile >= groundLimit)
        {
            srcRect.x = 960;
            srcRect.y = 64;
        }
        else if (gameMap->getTile(tileX + 1, tileY - 1) >= groundLimit && bottomTile < groundLimit && rightTile < groundLimit)
        {
            srcRect.x = 960;
            srcRect.y = 128;
        }
        else if (gameMap->getTile(tileX - 1, tileY + 1) >= groundLimit && topTile < groundLimit && leftTile < groundLimit)
        {
            srcRect.x = 896;
            srcRect.y = 128;
        }
        else if (leftTile >= groundLimit)
        {
            //render water with land
            srcRect.x = 896;
            srcRect.y = 32;
        }
        else if (rightTile >= groundLimit)
        {
            srcRect.x = 896;
            srcRect.y = 64;
        }
        else if (topTile >= groundLimit)
        {
            srcRect.x = 896;
            srcRect.y = 96;
        }
        else if (bottomTile >= groundLimit)
        {
            srcRect.x = 960;
            srcRect.y = 96;
        }
        else
        {
            //render water
            srcRect.x = 64;
            srcRect.y = 0;
        }
    }
    else if (tile > 12)
    {
        //render trees
        srcRect.x = 320;
        srcRect.y = 0;
        srcRect.height = tileHeight * 2;
    }
    else
    {
        //grass
        srcRect.x = 0;
        srcRect.y = 0;
    }

    return srcRect;
}

float GameMapRenderer::getTileYOffset(uint16_t tile, size_t tileX, size_t tileY)
{
    const graphics::Rect &srcRect = getSourceRect(tile, tileX, tileY);
    return tileHeight - srcRect.height;
}

void GameMapRenderer::clearCache()
{
    cacheTexture = nullptr;
}

void GameMapRenderer::renderTile(core::Renderer *renderer, uint16_t tile, int tileX, int tileY, const utils::Vector2 &pos)
{
    const auto &camera = renderer->getMainCamera();
    // float xPos = pos.getX() - camera->getX();
    // float yPos = pos.getY() - camera->getY();

    // float width = float(tileWidth) * renderer->getZoomFactor();
    // float height = float(tileHeight) * renderer->getZoomFactor();
    float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

    const graphics::Rect &srcRect = getSourceRect(tile, tileX, tileY);
    graphics::Rect destRect;
    destRect.x = (pos.getX() * factor) - camera->getX();
    destRect.y = ((pos.getY() - (srcRect.height - tileHeight)) * factor) - camera->getY();
    destRect.width = srcRect.width * factor;
    destRect.height = srcRect.height * factor;

    graphics::Rect realRect = {(pos.getX() * renderer->getZoomFactor()), (pos.getY() + (tileHeight - srcRect.height)) * factor, srcRect.width * factor, srcRect.height * factor};

    if (!realRect.intersects(camera->getViewPortRect()))
    {
        return;
    }
    groundTexture->render(renderer, srcRect, destRect);
}

utils::Vector2 GameMapRenderer::convertVec2(float zoomFactor, utils::Vector2 input)
{
    const utils::Vector2 &pt = gameMap->isoTo2D(input);

    float x, y = 0.0;

    float tx = pt.getX() / static_cast<float>(getTileHeight() * zoomFactor);
    float ty = pt.getY() / static_cast<float>(getTileHeight() * zoomFactor);

    x = std::floor(tx);
    if (x > gameMap->getWidth())
        x = gameMap->getWidth();
    y = std::floor(ty);
    if (y > gameMap->getHeight())
        y = gameMap->getHeight();

    return utils::Vector2(x, y);
}

void GameMapRenderer::render(core::Renderer *renderer)
{
    auto camera = renderer->getMainCamera();
    auto viewPort = camera->getViewPortRect();

    if (cacheTexture != nullptr)
    {
        cacheTexture->render(renderer, 0, 0);
        return;
    }

    float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

    cacheTexture = std::make_shared<graphics::Texture>(renderer, viewPort.width, viewPort.height);

    renderer->setRenderTarget(cacheTexture->getSDLTexture());
    //todo testcode
    auto start = convertVec2(factor, utils::Vector2(viewPort.x, viewPort.y));
    //auto end = convertVec2(renderer->getZoomFactor(), utils::Vector2(viewPort.width, viewPort.height));

    float tempEndX = viewPort.width / (factor * getTileHeight() / 2.f);
    float tempEndY = viewPort.height / (factor * getTileHeight() / 2.f);

    utils::Vector2 end(tempEndX, tempEndY);

    int startX = std::round(start.getX() - (end.getX() / 2.f));
    int startY = std::round(start.getY() - (end.getY() / 2.f));

    SDL_Color color = {0, 0, 0, 255};
    int endY = std::min(end.getY() / 1.5f + start.getY(), float(gameMap->getHeight()));
    int endX = std::min(end.getX() / 1.5f + start.getX(), float(gameMap->getWidth()));

    for (int tempY = std::max(startY, 0); tempY < endY; ++tempY)
    {
        for (int tempX = std::max(startX, 0); tempX < endX; ++tempX)
        {
            if (tempX > gameMap->getWidth() - 1 || tempY > gameMap->getHeight() - 1)
                continue;
            float x = static_cast<float>(tempX) * tileWidth / 2.0f;
            float y = static_cast<float>(tempY) * tileHeight;
            utils::Vector2 vec(x, y);
            const auto &iso = gameMap->twoDToIso(vec);

            renderTile(renderer, gameMap->getTile(tempX, tempY), tempX, tempY, iso);
        }
    }

    for (size_t tempY = std::max(startY, 0); tempY < endY; ++tempY)

    {
        for (size_t tempX = std::max(startX, 0); tempX < endX; ++tempX)

        {
            if (tempX > gameMap->getWidth() - 1 || tempY > gameMap->getHeight() - 1)
                continue;

            const std::shared_ptr<world::Building> &building = gameMap->getBuilding(tempX, tempY);
            if (building == nullptr)
                continue;
            auto displayRect = building->getDisplayRect();
            float x = static_cast<float>(tempX) * tileWidth / 2.0f;
            float y = static_cast<float>(tempY) * tileHeight;
            utils::Vector2 vec(x, y);
            const auto &pos = gameMap->twoDToIso(vec);
            TileType tile = gameMap->getTile(tempX, tempY);

            // displayRect.x = ((pos.getX() - building->getXOffset()) * factor) - camera->getX();
            // displayRect.width = displayRect.width * factor;
            // displayRect.height = displayRect.height * factor;

            float tileYOffset = getTileYOffset(tile, tempX, tempY);

            // displayRect.y = ((pos.getY() - building->getYOffset()) * factor) - (camera->getY() + tileYOffset);

            auto &srcRect = building->getSourceRect();

            displayRect.x = ((pos.getX() - building->getXOffset()) * factor) - camera->getX();
            displayRect.y = ((pos.getY() - building->getYOffset() - tileYOffset) * factor) - camera->getY();
            displayRect.width = srcRect.width * factor;
            displayRect.height = srcRect.height * factor;

            if (!building->getSubTexture().empty())
            {
                textureMap->render(building->getSubTexture(), displayRect, renderer);
            }
            else
            {
                groundTexture->render(renderer, building->getSourceRect(), displayRect);
            }
        }
    }

    renderer->setRenderTarget(nullptr);
    cacheTexture->render(renderer, 0, 0);
}

size_t GameMapRenderer::getTileWidth()
{
    return tileWidth;
}
size_t GameMapRenderer::getTileHeight()
{
    return tileHeight;
}
