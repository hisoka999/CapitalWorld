#include "gamemaprenderer.h"
#include "iso.h"
#include <engine/graphics/rect.h>
#include <engine/utils/os.h>
#include <engine/utils/string.h>

Uint32 ColourToUint(int R, int G, int B)
{
    return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

GameMapRenderer::GameMapRenderer(std::shared_ptr<GameMap> gameMap)
    : gameMap(gameMap), tileWidth(64), tileHeight(32)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "landscape.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 10);

    textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));
    cacheTexture = nullptr;

    grassHash = hasher("grass");
    grass1Hash = hasher("grass1");
    grasRockHash = hasher("grass_rocks");
    treesHash = hasher("trees");
    waterHash = hasher("water");
    sandHash = hasher("sand");
    resourceRockHash = hasher("resource_rock");
    resourceOilHash = hasher("resource_oil");
    fillAutoTileMap();
    generateTileDataFromMap();
}

const graphics::Rect &GameMapRenderer::getAutoTile(const TileType tile, size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit)
{

    return textureMap->getSourceRect(getAutoTileId(tile, baseTile, tileX, tileY, groundLimit));
}

const size_t GameMapRenderer::getAutoTileId(const TileType tile, size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit)
{
    const TileType leftTile = gameMap->getTile(tileX, tileY + 1);
    const TileType rightTile = gameMap->getTile(tileX, tileY - 1);
    const TileType topTile = gameMap->getTile(tileX - 1, tileY);
    const TileType bottomTile = gameMap->getTile(tileX + 1, tileY);

    const Autotile &autoTile = autoTileMap[baseTile];
    size_t resultTile = autoTile.baseHash;
    if (rightTile >= groundLimit && topTile >= groundLimit)
    {
        resultTile = autoTile.top_right;
    }
    else if (leftTile >= groundLimit && topTile >= groundLimit)
    {
        resultTile = autoTile.top_left;
    }
    else if (rightTile >= groundLimit && bottomTile >= groundLimit)
    {
        resultTile = autoTile.bottom_right;
    }
    else if (leftTile >= groundLimit && bottomTile >= groundLimit)
    {
        resultTile = autoTile.bottom_left;
    }
    else if (gameMap->getTile(tileX + 1, tileY - 1) >= groundLimit && bottomTile < groundLimit && rightTile < groundLimit)
    {
        resultTile = autoTile.bottom_right_corner;
    }
    else if (gameMap->getTile(tileX - 1, tileY + 1) >= groundLimit && topTile < groundLimit && leftTile < groundLimit)
    {
        resultTile = autoTile.top_left_corner;
    }
    else if (leftTile >= groundLimit)
    {
        resultTile = autoTile.left;
    }
    else if (rightTile >= groundLimit)
    {
        resultTile = autoTile.right;
    }
    else if (topTile >= groundLimit)
    {
        resultTile = autoTile.top;
    }
    else if (bottomTile >= groundLimit)
    {
        resultTile = autoTile.bottom;
    }
    return resultTile;
}

const size_t GameMapRenderer::getSourceTile(const TileType tile, const size_t tileX, const size_t tileY)
{
    const int groundLimit = 8;

    if (tile < groundLimit)
    {
        return getAutoTileId(tile, waterHash, tileX, tileY, groundLimit);
    }
    else if (tile > 12)
    {
        return treesHash;
    }
    else if (tile > groundLimit)
    {
        // find special tiles like rocks, single trees or mountains
        Decoration decoration = static_cast<Decoration>(gameMap->getDecoration(tileX, tileY));
        switch (decoration)
        {
        case Decoration::grass1:
            return grass1Hash;
            break;
        case Decoration::rocks:
            return grasRockHash;
            break;
        case Decoration::none:
            return grassHash;
            break;

        default:
            return grassHash;
            break;
        }
    }

    return getAutoTileId(tile, sandHash, tileX, tileY, groundLimit + 1);
}

void GameMapRenderer::fillAutoTileMap()
{
    auto tile = generateAutoTile("water");
    autoTileMap[tile.baseHash] = tile;

    tile = generateAutoTile("sand");
    autoTileMap[tile.baseHash] = tile;
}

Autotile GameMapRenderer::generateAutoTile(std::string base)
{
    Autotile tile;
    tile.baseHash = hasher(base);
    tile.bottom = hasher(base + "_bottom");
    tile.bottom_left = hasher(base + "_bottom_left");
    tile.bottom_right = hasher(base + "_bottom_right");
    tile.bottom_right_corner = hasher(base + "_bottom_right_corner");
    tile.left = hasher(base + "_left");
    tile.right = hasher(base + "_right");
    tile.top_right = hasher(base + "_top_right");
    tile.top = hasher(base + "_top");
    tile.top_left = hasher(base + "_top_left");
    tile.top_left_corner = hasher(base + "_top_left_corner");

    return tile;
}

const graphics::Rect &GameMapRenderer::getSourceRect(const TileType tile, const size_t tileX, const size_t tileY)
{

    const int groundLimit = 8;

    if (tile < groundLimit)
    {
        return getAutoTile(tile, waterHash, tileX, tileY, groundLimit);
    }
    else if (tile > 12)
    {
        return textureMap->getSourceRect(treesHash);
    }
    else if (tile > groundLimit)
    {
        // find special tiles like rocks, single trees or mountains
        Decoration decoration = static_cast<Decoration>(gameMap->getDecoration(tileX, tileY));
        switch (decoration)
        {
        case Decoration::grass1:
            return textureMap->getSourceRect(grass1Hash);
            break;
        case Decoration::rocks:
            return textureMap->getSourceRect(grasRockHash);
            break;
        case Decoration::none:
            return textureMap->getSourceRect(grassHash);
            break;

        default:
            return textureMap->getSourceRect(grassHash);
            break;
        }
    }

    return getAutoTile(tile, sandHash, tileX, tileY, groundLimit + 1);
}

float GameMapRenderer::getTileYOffset(uint16_t tile, size_t tileX, size_t tileY)
{
    const graphics::Rect &srcRect = getSourceRect(tile, tileX, tileY);
    return tileHeight - srcRect.height;
}

void GameMapRenderer::clearCache()
{
    fillCache = true;
}

void GameMapRenderer::refreshMiniMap()
{
    updateMiniMap = true;
}

std::shared_ptr<graphics::Texture> &GameMapRenderer::getMiniMap()
{
    return miniMap;
}

std::shared_ptr<graphics::TextureMap> &GameMapRenderer::getTextureMap()
{
    return textureMap;
}

void GameMapRenderer::renderMiniMap(core::Renderer *renderer)
{

    if (!updateMiniMap)
        return;
    miniMap = nullptr;
    int width = gameMap->getWidth() * 2;
    int height = gameMap->getHeight();
    miniMap = std::make_shared<graphics::Texture>(renderer, width, height, SDL_TEXTUREACCESS_STREAMING);

    auto startTime = std::chrono::high_resolution_clock::now();

    miniMap->setBlendMode(SDL_BLENDMODE_BLEND);
    miniMap->lockTexture();

    for (int tempY = 0; tempY < height; ++tempY)
    {
        for (int tempX = 0; tempX < int(gameMap->getWidth()); ++tempX)
        {
            if (tempX > gameMap->getWidth() - 1 || tempY > height - 1)
                continue;
            float x = tempX;
            float y = tempY;
            utils::Vector2 vec(x, y);
            const auto &iso = iso::twoDToIso(vec);
            const TileType tileType = gameMap->getTile(vec);
            const std::shared_ptr<world::Building> &building = gameMap->getBuilding(tempX, tempY);
            size_t pos = (iso.getX()) + (iso.getY() * width);
            if (building != nullptr)
            {
                miniMap->setPixel(iso.getX() + gameMap->getWidth(), iso.getY(), {255, 255, 255, 255});
            }
            else if (tileType > 8)
            {
                miniMap->setPixel(iso.getX() + gameMap->getWidth(), iso.getY(), {125, 139, 46, 255});
            }
            else if (tileType == 8)
            {
                miniMap->setPixel(iso.getX() + gameMap->getWidth(), iso.getY(), {246, 226, 197, 255});
            }
            else
            {
                miniMap->setPixel(iso.getX() + gameMap->getWidth(), iso.getY(), {46, 80, 125, 255});
            }
        }
    }

    miniMap->unlockTexture();
    updateMiniMap = false;
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "create minimap time: " << microseconds << "ms" << std::endl;
}

void GameMapRenderer::generateTileDataFromMap()
{
    tileData.resize(gameMap->getWidth() * gameMap->getHeight());

    for (size_t y = 0; y < gameMap->getHeight(); ++y)
    {
        for (size_t x = 0; x < gameMap->getWidth(); ++x)
        {
            const TileType tile = gameMap->getTile(x, y);
            size_t source = getSourceTile(tile, x, y);
            tileData[x + (y * gameMap->getWidth())] = source;
        }
    }
}

void GameMapRenderer::renderTile(core::Renderer *renderer, const core::Camera *camera, const float factor, const uint16_t tile, const int tileX, const int tileY, const utils::Vector2 &pos)
{
    size_t hash = tileData[tileX + (gameMap->getWidth() * tileY)];
    const graphics::Rect &srcRect = textureMap->getSourceRect(hash);
    float x = (pos.getX() * factor) - camera->getX();
    float y = ((pos.getY() - (srcRect.height - tileHeight)) * factor) - camera->getY();
    const graphics::Rect destRect{x, y, srcRect.width * factor, srcRect.height * factor};

    // // groundTexture->render(renderer, srcRect, destRect);
    textureMap->render(hash, destRect, renderer);
}

void GameMapRenderer::renderResource(core::Renderer *renderer, const core::Camera *camera, const float factor, const int tileX, const int tileY, const utils::Vector2 &pos)
{
    auto resource = gameMap->getRawResource(tileX, tileY);
    size_t hash = 0;
    switch (resource)
    {
    case RawResource::Coal:
    case RawResource::Iron:
    case RawResource::Silicon:
    case RawResource::Copper:
    case RawResource::Gold:
    case RawResource::Aluminum:
        hash = resourceRockHash;
        break;

    case RawResource::Oil:
        hash = resourceOilHash;
        break;
    }

    if (hash != 0)
    {
        const graphics::Rect &srcRect = textureMap->getSourceRect(hash);
        float x = (pos.getX() * factor) - camera->getX();
        float y = ((pos.getY() - (srcRect.height - tileHeight)) * factor) - camera->getY();
        const graphics::Rect destRect{x, y, srcRect.width * factor, srcRect.height * factor};

        textureMap->render(hash, destRect, renderer);
        }
}

utils::Vector2 GameMapRenderer::convertVec2(float zoomFactor, utils::Vector2 input)
{
    const utils::Vector2 &pt = iso::isoTo2D(input);

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

    if (!fillCache && cacheTexture != nullptr)
    {
        cacheTexture->render(renderer, 0, 0);
        // cacheBuildingTexture->render(renderer, 0, 0);
        return;
    }
    fillCache = false;
    auto startTime = std::chrono::high_resolution_clock::now();

    float factor = ceilf(renderer->getZoomFactor() * 100) / 100;
    if (cacheTexture == nullptr)
        cacheTexture = std::make_shared<graphics::Texture>(renderer, viewPort.width, viewPort.height);
    // cacheBuildingTexture = std::make_shared<graphics::Texture>(renderer, viewPort.width, viewPort.height);
    renderer->setRenderTarget(cacheTexture->getSDLTexture());

    renderer->clear();
    auto start = convertVec2(factor, utils::Vector2(viewPort.x, viewPort.y));

    float tempEndX = viewPort.width / (factor * getTileHeight() / 2.f);
    float tempEndY = viewPort.height / (factor * getTileHeight() / 2.f);

    utils::Vector2 end(tempEndX, tempEndY);

    int startX = std::round(start.getX() - (end.getX() / 2.f));
    int startY = std::round(start.getY() - (end.getY() / 2.f));

    int endY = std::min(end.getY() / 1.8f + start.getY(), float(gameMap->getHeight()));
    int endX = std::min(end.getX() / 1.8f + start.getX(), float(gameMap->getWidth()));

    for (int tempY = std::max(startY, 0); tempY < endY; ++tempY)
    {
        for (int tempX = std::max(startX, 0); tempX < endX; ++tempX)
        {
            if (tempX > gameMap->getWidth() - 1 || tempY > gameMap->getHeight() - 1)
                continue;

            float x = static_cast<float>(tempX) * tileWidth / 2.0f;
            float y = static_cast<float>(tempY) * tileHeight;
            const utils::Vector2 vec(x, y);
            const auto &iso = iso::twoDToIso(vec);
            const auto tile = gameMap->getTile(tempX, tempY);

            renderTile(renderer, camera, factor, tile, tempX, tempY, iso);

            renderResource(renderer, camera, factor, tempX, tempY, iso);
        }
    }
    graphics::Rect displayRect;
    for (size_t tempY = std::max(startY, 0); tempY < endY; ++tempY)

    {
        for (size_t tempX = std::max(startX, 0); tempX < endX; ++tempX)

        {
            if (tempX > gameMap->getWidth() - 1 || tempY > gameMap->getHeight() - 1)
                continue;

            const std::shared_ptr<world::Building> &building = gameMap->getBuilding(tempX, tempY);
            if (building == nullptr)
                continue;
            displayRect = building->getDisplayRect();
            float x = static_cast<float>(tempX) * tileWidth / 2.0f;
            float y = static_cast<float>(tempY) * tileHeight;
            const utils::Vector2 vec(x, y);
            const auto &pos = iso::twoDToIso(vec);
            const TileType tile = gameMap->getTile(tempX, tempY);

            // displayRect.x = ((pos.getX() - building->getXOffset()) * factor) - camera->getX();
            // displayRect.width = displayRect.width * factor;
            // displayRect.height = displayRect.height * factor;

            const float tileYOffset = getTileYOffset(tile, tempX, tempY);

            // displayRect.y = ((pos.getY() - building->getYOffset()) * factor) - (camera->getY() + tileYOffset);

            const auto &srcRect = building->getSourceRect();

            displayRect.x = ((pos.getX() - building->getXOffset()) * factor) - camera->getX();
            displayRect.y = ((pos.getY() - building->getYOffset() - tileYOffset) * factor) - camera->getY();
            displayRect.width = srcRect.width * factor;
            displayRect.height = srcRect.height * factor;

            if (building->getSubTextureHash())
            {
                textureMap->render(building->getSubTextureHash(), displayRect, renderer);
            }
            else
            {
                groundTexture->render(renderer, building->getSourceRect(), displayRect);
            }
        }
    }

    renderer->setRenderTarget(nullptr);
    cacheTexture->render(renderer, 0, 0);
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    if (milliseconds >= 10)
        std::cout << "update map time: " << milliseconds << "ms" << std::endl;
    // std::cout << "x num:" << endX - std::max(startX, 0) << " y num: " << endY - std::max(startY, 0) << std::endl;
    // std::cout << "start x: " << std::max(startX, 0) << " end x: " << endX << std::endl;
    // std::cout << " viewPort.x: " << viewPort.x << " viewPort.y: " << viewPort.y << std::endl;
    if (!SDL_GetHintBoolean(SDL_HINT_RENDER_BATCHING, SDL_TRUE))
    {
        std::cout << "batch rendering is disabled" << std::endl;
    }
    renderMiniMap(renderer);
}

size_t GameMapRenderer::getTileWidth()
{
    return tileWidth;
}
size_t GameMapRenderer::getTileHeight()
{
    return tileHeight;
}