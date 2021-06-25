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

void GameMapRenderer::renderTile(core::Renderer *renderer, uint16_t tile, int tileX, int tileY, const utils::Vector2 &pos)
{
    const auto &camera = renderer->getMainCamera();
    float xPos = pos.getX() - camera->getX();
    float yPos = pos.getY() - camera->getY();

    float width = float(tileWidth) * renderer->getZoomFactor();
    float height = float(tileHeight) * renderer->getZoomFactor();

    const graphics::Rect &srcRect = getSourceRect(tile, tileX, tileY);
    graphics::Rect destRect = {std::round((pos.getX() * renderer->getZoomFactor()) - camera->getX()), std::round(((pos.getY() + (tileHeight - srcRect.height)) * renderer->getZoomFactor()) - camera->getY()), srcRect.width * renderer->getZoomFactor(), srcRect.height * renderer->getZoomFactor()};
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

    //todo testcode
    auto start = convertVec2(renderer->getZoomFactor(), utils::Vector2(viewPort.x, viewPort.y));
    auto end = convertVec2(renderer->getZoomFactor(), utils::Vector2(viewPort.height, viewPort.width));

    int startX = start.getX() - (end.getX() / 2);
    int startY = start.getY() - (end.getX() / 2);

    SDL_Color color = {0, 0, 0, 255};
    for (size_t j = std::max(startY, 0); j < std::max(end.getY() + start.getY(), float(gameMap->getHeight())); ++j)
    {
        for (size_t i = std::max(startX, 0); i < std::max(end.getX() + start.getX(), float(gameMap->getWidth())); ++i)
        {

            float x = static_cast<float>(i) * tileWidth / 2.0f;
            float y = static_cast<float>(j) * tileHeight;
            utils::Vector2 vec(x, y);
            const auto &iso = gameMap->twoDToIso(vec);

            renderTile(renderer, gameMap->getTile(i, j), i, j, iso);
        }
    }

    for (size_t y = std::max(startY, 0); y < std::min(end.getY() + start.getY(), float(gameMap->getHeight())); ++y)
    {
        for (size_t x = std::max(startX, 0); x < std::min(end.getX() + start.getX(), float(gameMap->getWidth())); ++x)
        {
            const std::shared_ptr<world::Building> &building = gameMap->getBuilding(x, y);
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
}

size_t GameMapRenderer::getTileWidth()
{
    return tileWidth;
}
size_t GameMapRenderer::getTileHeight()
{
    return tileHeight;
}
