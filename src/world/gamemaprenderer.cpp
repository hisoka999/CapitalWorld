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

    int groundLimit = 10;

    if (tile < 8)
    {
        //render water
        srcRect.x = 64;
        srcRect.y = 0;
    }
    else if (tile > 12)
    {
        //render trees
        srcRect.x = 192;
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
    float xPos = pos.getX() - camera->getX();
    float yPos = pos.getY() - camera->getY();

    float width = float(tileWidth) * renderer->getZoomFactor();
    float height = float(tileHeight) * renderer->getZoomFactor();

    const graphics::Rect &srcRect = getSourceRect(tile, tileX, tileY);
    graphics::Rect destRect;
    destRect.x = std::round((pos.getX() * renderer->getZoomFactor()) - camera->getX());
    destRect.y = std::round(((pos.getY() + (tileHeight - srcRect.height)) * renderer->getZoomFactor()) - camera->getY());
    destRect.width = std::round(srcRect.width * renderer->getZoomFactor());
    destRect.height = std::round(srcRect.height * renderer->getZoomFactor());

    graphics::Rect realRect = {(pos.getX() * renderer->getZoomFactor()), (pos.getY() + (tileHeight - srcRect.height)) * renderer->getZoomFactor(), srcRect.width * renderer->getZoomFactor(), srcRect.height * renderer->getZoomFactor()};

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
    cacheTexture = std::make_shared<graphics::Texture>(renderer, viewPort.width, viewPort.height);

    renderer->setRenderTarget(cacheTexture->getSDLTexture());
    //todo testcode
    auto start = convertVec2(renderer->getZoomFactor(), utils::Vector2(viewPort.x, viewPort.y));
    //auto end = convertVec2(renderer->getZoomFactor(), utils::Vector2(viewPort.width, viewPort.height));

    float tempEndX = viewPort.width / (renderer->getZoomFactor() * getTileHeight() / 2.f);
    float tempEndY = viewPort.height / (renderer->getZoomFactor() * getTileHeight() / 2.f);

    utils::Vector2 end(tempEndX, tempEndY);

    int startX = start.getX() - (end.getX() / 2);
    int startY = start.getY() - (end.getY() / 2);

    SDL_Color color = {0, 0, 0, 255};
    size_t endY = std::min(end.getY() / 1.5f + start.getY(), float(gameMap->getHeight()));
    size_t endX = std::min(end.getX() / 1.5f + start.getX(), float(gameMap->getWidth()));

    for (size_t tempY = std::max(startY, 0); tempY < endY; ++tempY)
    {
        for (size_t tempX = std::max(startX, 0); tempX < endX; ++tempX)
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

            displayRect.x = (displayRect.x * tileWidth / 2.0f);
            displayRect.y = (displayRect.y * tileHeight);
            utils::Vector2 vec(displayRect.x, displayRect.y);
            TileType tile = gameMap->getTile(building->getDisplayRect().x, building->getDisplayRect().y);

            auto pos = gameMap->twoDToIso(vec);

            displayRect.x = std::round(((pos.getX() - building->getXOffset()) * renderer->getZoomFactor()) - camera->getX());
            displayRect.width = displayRect.width * renderer->getZoomFactor();
            displayRect.height = displayRect.height * renderer->getZoomFactor();

            float tileYOffset = getTileYOffset(tile, building->getDisplayRect().x, building->getDisplayRect().y);

            displayRect.y = std::round(((pos.getY() - building->getYOffset()) * renderer->getZoomFactor()) - (camera->getY() + tileYOffset));

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
