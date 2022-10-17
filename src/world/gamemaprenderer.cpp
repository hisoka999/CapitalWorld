#include "gamemaprenderer.h"
#include "iso.h"
#include <engine/graphics/rect.h>
#include <engine/utils/os.h>
#include <engine/utils/string.h>
#include <algorithm>
#include <engine/utils/color.h>
#include <engine/utils/logger.h>
#include <cassert>
#include <list>

Uint32 ColourToUint(int R, int G, int B)
{
    return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

struct RenderCommand
{
    size_t hashId;
    graphics::Rect dstRect;
    graphics::Rect srcRect;
    std::shared_ptr<world::graphics::Sprite> sprite;
};

bool compareRenderCommand(const RenderCommand &c1, const RenderCommand &c2)
{
    int y1 = c1.dstRect.y + c1.dstRect.height;
    int y2 = c2.dstRect.y + c2.dstRect.height;
    if (y1 < y2)
    {
        return true;
    }
    else if (y1 == y2)
    {
        return c1.dstRect.x < c2.dstRect.x;
    }
    return false;
}

GameMapRenderer::GameMapRenderer(std::shared_ptr<world::GameState> gameState)
    : gameState(gameState), tileWidth(64), tileHeight(32)
{
    groundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "landscape.png"));
    debugText = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 10);

    textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));

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

graphics::Rect GameMapRenderer::getAutoTile(size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit)
{

    graphics::Rect rect;
    textureMap->getSourceRect(getAutoTileId(baseTile, tileX, tileY, groundLimit), &rect);
    return rect;
}

size_t GameMapRenderer::getAutoTileId(size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit)
{
    const TileType leftTile = gameState->getGameMap()->getTile(tileX, tileY + 1);
    const TileType rightTile = gameState->getGameMap()->getTile(tileX, tileY - 1);
    const TileType topTile = gameState->getGameMap()->getTile(tileX - 1, tileY);
    const TileType bottomTile = gameState->getGameMap()->getTile(tileX + 1, tileY);

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
    else if (gameState->getGameMap()->getTile(tileX + 1, tileY - 1) >= groundLimit && bottomTile < groundLimit && rightTile < groundLimit)
    {
        resultTile = autoTile.bottom_right_corner;
    }
    else if (gameState->getGameMap()->getTile(tileX - 1, tileY + 1) >= groundLimit && topTile < groundLimit && leftTile < groundLimit)
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

size_t GameMapRenderer::getSourceTile(const TileType tile, const size_t tileX, const size_t tileY)
{
    const int groundLimit = 8;

    if (tile < groundLimit)
    {
        return getAutoTileId(waterHash, tileX, tileY, groundLimit);
    }
    else if (tile > 12)
    {
        return treesHash;
    }
    else if (tile > groundLimit)
    {
        // find special tiles like rocks, single trees or mountains
        Decoration decoration = static_cast<Decoration>(gameState->getGameMap()->getDecoration(tileX, tileY));
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

    return getAutoTileId(sandHash, tileX, tileY, groundLimit + 1);
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

const graphics::Rect GameMapRenderer::getSourceRect(const TileType tile, const size_t tileX, const size_t tileY)
{

    const int groundLimit = 8;
    graphics::Rect result;
    if (tile < groundLimit)
    {
        return getAutoTile(waterHash, tileX, tileY, groundLimit);
    }
    else if (tile > 12)
    {
        textureMap->getSourceRect(treesHash, &result);
        return result;
    }
    else if (tile > groundLimit)
    {
        // find special tiles like rocks, single trees or mountains
        Decoration decoration = static_cast<Decoration>(gameState->getGameMap()->getDecoration(tileX, tileY));
        switch (decoration)
        {
        case Decoration::grass1:
            textureMap->getSourceRect(grass1Hash, &result);
            break;
        case Decoration::rocks:
            textureMap->getSourceRect(grasRockHash, &result);
            break;
        case Decoration::none:
            textureMap->getSourceRect(grassHash, &result);
            break;

        default:
            textureMap->getSourceRect(grassHash, &result);
            break;
        }
        return result;
    }

    return getAutoTile(sandHash, tileX, tileY, groundLimit + 1);
}

float GameMapRenderer::getTileYOffset(size_t tileX, size_t tileY)
{
    graphics::Rect srcRect;
    size_t hash = tileData[tileX + (gameState->getGameMap()->getWidth() * tileY)];
    textureMap->getSourceRect(hash, &srcRect);

    return tileHeight - srcRect.height;
}

void GameMapRenderer::clearCache()
{
    fillCache = true;
    for (auto change : gameState->getGameMap()->getChangedTiles())
    {
        int x = int(change.getX());
        int y = int(change.getY());
        const TileType tile = gameState->getGameMap()->getTile(x, y);
        size_t source = getSourceTile(tile, x, y);
        tileData[x + (y * gameState->getGameMap()->getWidth())] = source;
    }
    gameState->getGameMap()->clearChangedTiles();
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

void GameMapRenderer::toggleDebug()
{
    debugRender = !debugRender;
}

void GameMapRenderer::update(double deltaTime, int gameSpeed)
{
    int i = 0;
    for (auto it = currentAnimations.begin(); it != currentAnimations.end(); ++it)
    {
        i++;
        if ((*it) == nullptr)
        {
            APP_LOG_ERROR("animation irterator is null");
            it = currentAnimations.erase(it);
        }
        else
        {
            (*it)->update(deltaTime, gameSpeed / 2);
        }
    }
    auto it = currentAnimations.begin();
    while (it != currentAnimations.end())
    {
        if ((*it)->isFinished())
        {
            it = currentAnimations.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void GameMapRenderer::addAnimation(std::unique_ptr<world::AnimatedMovement> animation)
{
    currentAnimations.push_back(std::move(animation));
}

void GameMapRenderer::renderMiniMap(core::Renderer *renderer)
{

    if (!updateMiniMap)
        return;
    miniMap = nullptr;
    size_t width = gameState->getGameMap()->getWidth() * 2;
    size_t height = gameState->getGameMap()->getHeight();
    miniMap = std::make_shared<graphics::Texture>(renderer, width, height, SDL_TEXTUREACCESS_STREAMING);

    auto startTime = std::chrono::high_resolution_clock::now();

    miniMap->setBlendMode(SDL_BLENDMODE_BLEND);
    miniMap->lockTexture();

    for (size_t tempY = 0; tempY < height; ++tempY)
    {
        for (size_t tempX = 0; tempX < gameState->getGameMap()->getWidth(); ++tempX)
        {
            if (tempX > gameState->getGameMap()->getWidth() - 1 || tempY > height - 1)
                continue;
            float x = float(tempX);
            float y = float(tempY);

            const auto &iso = iso::twoDToIso(x, y);
            const TileType tileType = gameState->getGameMap()->getTile(tempX, tempY);
            const std::shared_ptr<world::Building> &building = gameState->getGameMap()->getBuilding(tempX, tempY);

            if (building != nullptr)
            {
                miniMap->setPixel(int(iso.getX()) + gameState->getGameMap()->getWidth(), int(iso.getY()), {255, 255, 255, 255});
            }
            else if (tileType > 8)
            {
                miniMap->setPixel(int(iso.getX()) + gameState->getGameMap()->getWidth(), int(iso.getY()), {125, 139, 46, 255});
            }
            else if (tileType == 8)
            {
                miniMap->setPixel(int(iso.getX()) + gameState->getGameMap()->getWidth(), int(iso.getY()), {246, 226, 197, 255});
            }
            else
            {
                miniMap->setPixel(int(iso.getX()) + gameState->getGameMap()->getWidth(), int(iso.getY()), {46, 80, 125, 255});
            }
        }
    }

    miniMap->unlockTexture();
    updateMiniMap = false;
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    APP_LOG_TRACE("create minimap time: " + std::to_string(microseconds) + "ms");
}

void GameMapRenderer::generateTileDataFromMap()
{
    tileData.resize(gameState->getGameMap()->getWidth() * gameState->getGameMap()->getHeight());

    for (size_t y = 0; y < gameState->getGameMap()->getHeight(); ++y)
    {
        for (size_t x = 0; x < gameState->getGameMap()->getWidth(); ++x)
        {
            const TileType tile = gameState->getGameMap()->getTile(x, y);
            size_t source = getSourceTile(tile, x, y);
            tileData[x + (y * gameState->getGameMap()->getWidth())] = source;
        }
    }
}

void GameMapRenderer::renderTile(core::Renderer *renderer, const core::Camera *camera, const float factor, const int tileX, const int tileY, const utils::Vector2 &pos)
{
    size_t hash = tileData[tileX + (gameState->getGameMap()->getWidth() * tileY)];
    graphics::Rect srcRect;
    textureMap->getSourceRect(hash, &srcRect);
    float x = (pos.getX() * factor) - camera->getX();
    float y = ((pos.getY() - (srcRect.height - tileHeight)) * factor) - camera->getY();
    const graphics::Rect destRect{x, y, srcRect.width * factor, srcRect.height * factor};

    // // groundTexture->render(renderer, srcRect, destRect);
    textureMap->render(hash, destRect, renderer);
}

void GameMapRenderer::renderResource(core::Renderer *renderer, const core::Camera *camera, const float factor, const int tileX, const int tileY, const utils::Vector2 &pos)
{
    auto resource = gameState->getGameMap()->getRawResource(tileX, tileY);
    size_t hash = 0;
    switch (resource)
    {
    case world::RawResource::Coal:
    case world::RawResource::Iron:
    case world::RawResource::Silicon:
    case world::RawResource::Copper:
    case world::RawResource::Gold:
    case world::RawResource::Aluminum:
        hash = resourceRockHash;
        break;

    case world::RawResource::Oil:
        hash = resourceOilHash;
        break;
    default:
        break;
    }

    if (hash != 0)
    {
        graphics::Rect srcRect;
        textureMap->getSourceRect(hash, &srcRect);
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
    if (x > gameState->getGameMap()->getWidth())
        x = gameState->getGameMap()->getWidth();
    y = std::floor(ty);
    if (y > gameState->getGameMap()->getHeight())
        y = gameState->getGameMap()->getHeight();

    return utils::Vector2(x, y);
}

void GameMapRenderer::render(core::Renderer *renderer)
{
    auto camera = renderer->getMainCamera();
    graphics::Rect viewPort = camera->getViewPortRect();
    auto startTime = std::chrono::high_resolution_clock::now();
    float factor = ceilf(renderer->getZoomFactor() * 100) / 100;
    auto &font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);

    int gameMapWidth = gameState->getGameMap()->getWidth();
    int gameMapHeight = gameState->getGameMap()->getHeight();

    auto start = convertVec2(factor, utils::Vector2(viewPort.x + (viewPort.width / 2), viewPort.y));
    int tilesX = (viewPort.width / getTileWidth()) * 2.5f / factor;
    int tilesY = viewPort.height / tileHeight * 2 / factor;
    int startX = start.getX() - (tilesX / 4);
    int startY = start.getY() - (tilesY / 4);

    int endX = startX + tilesX;
    int endY = startY + tilesY;

    auto textureMapPtr = textureMap.get();
    size_t oldHash = 0;
    graphics::Rect srcRect;
    graphics::Rect srcRectRes;
    graphics::Rect destRect;

    for (int tempY = std::max(startY, 0); tempY < endY; ++tempY)
    {
        if (tempY > gameMapHeight - 1)
            break;

        for (int tempX = std::max(startX, 0); tempX < endX; ++tempX)
        {
            if (tempX > gameMapWidth - 1)
                continue;

            float x = static_cast<float>(tempX) * tileWidth / 2.0f;
            float y = static_cast<float>(tempY) * tileHeight;

            const utils::Vector2 iso(x - y, (x + y) / 2.f);

            size_t hash = tileData[tempX + (gameMapWidth * tempY)];
            if (oldHash != hash)
            {
                textureMapPtr->getSourceRect(hash, &srcRect);
                oldHash = hash;
            }

            destRect.x = (iso.getX() * factor);
            destRect.y = ((iso.getY() - (srcRect.height - tileHeight)) * factor);
            destRect.width = srcRect.width * factor;
            destRect.height = srcRect.height * factor;

            if (!destRect.intersects(viewPort))
                continue;
            destRect.x -= camera->getX();
            destRect.y -= camera->getY();
            textureMapPtr->render(hash, destRect, renderer);

            renderResource(renderer, camera, factor, tempX, tempY, iso);
        }
    }

    auto startTimeBuildings = std::chrono::high_resolution_clock::now();

    graphics::Rect displayRect;
    graphics::Rect twoDPosition;

    const std::vector<std::shared_ptr<world::Building>> &buildings = gameState->getGameMap()->getBuildings();

    std::list<RenderCommand> renderCommands;
    std::list<RenderCommand> renderStreetCommands;

    for (int y = std::max(startY, 0); y < endY; ++y)

    {
        if (y > gameMapHeight - 1)
            break;

        for (int x = std::max(startX, 0); x < endX; ++x)

        {
            if (x > gameMapWidth - 1)
                continue;

            const std::shared_ptr<world::Building> &building = buildings[x + (gameMapWidth * y)];
            if (building == nullptr)
                continue;
            twoDPosition = building->get2DPosition();
            int xOffset = x - twoDPosition.x;
            int yOffset = y - twoDPosition.y;
            if (xOffset != twoDPosition.width - 1 || yOffset != twoDPosition.height - 1)
                continue;

            float tx = float(twoDPosition.x) * tileWidth / 2.0f;
            float ty = float(twoDPosition.y) * tileHeight;
            const utils::Vector2 vec(tx, ty);
            // const auto &pos = iso::twoDToIso(vec);
            const utils::Vector2 pos(tx - ty, (tx + ty) / 2.f);

            srcRect = building->getSourceRect();

            const float tileYOffset = 0.0; // getTileYOffset(building->get2DPosition().x, building->get2DPosition().y);

            displayRect.x = ((pos.getX() - building->getXOffset()) * factor);
            displayRect.y = ((pos.getY() - building->getYOffset() - tileYOffset) * factor);
            displayRect.width = srcRect.width * factor;
            displayRect.height = srcRect.height * factor;

            if (!displayRect.intersects(viewPort))
                continue;

            displayRect.x -= camera->getX();
            displayRect.y -= camera->getY();

            RenderCommand command = {building->getSubTextureHash(), displayRect, building->getSourceRect(), nullptr};
            if (building->getType() == world::BuildingType::Street)
                renderStreetCommands.emplace_back(command);
            else
                renderCommands.emplace_back(command);

            // if (building->getSubTextureHash())
            // {
            //     textureMapPtr->render(building->getSubTextureHash(), displayRect, renderer);
            // }
            // else
            // {
            //     groundTexture->render(renderer, building->getSourceRect(), displayRect);
            // }
            if (debugRender)
            {
                std::string text = utils::string_format("x: %d | y: %d", x, y);
                font->render(renderer, text, utils::color::RED, displayRect.x + (displayRect.width / 2), displayRect.y + (displayRect.height / 2));
            }
        }
    }

    for (auto &animation : currentAnimations)
    {
        utils::Vector2 pos = animation->getSprite()->getCalulatedPos(renderer);
        graphics::Rect displayRect = {pos.getX(), pos.getY(), animation->getSprite()->getWidth() * factor, animation->getSprite()->getHeight() * factor};
        graphics::Rect srcRect;
        RenderCommand command = {0, displayRect, srcRect, animation->getSprite()};
        renderCommands.emplace_back(command);
    }

    renderCommands.sort(compareRenderCommand);
    renderStreetCommands.sort(compareRenderCommand);

    for (auto &command : renderStreetCommands)
    {
        // assert(command.sprite != nullptr && command.hashId == 0);
        if (command.sprite != nullptr)
        {
            command.sprite->render(renderer);
        }
        else if (command.hashId)
        {
            textureMapPtr->render(command.hashId, command.dstRect, renderer);
        }
        else
        {
            groundTexture->render(renderer, command.srcRect, command.dstRect);
        }
    }

    for (auto &command : renderCommands)
    {
        // assert(command.sprite != nullptr && command.hashId == 0);
        if (command.sprite != nullptr)
        {
            command.sprite->render(renderer);
        }
        else if (command.hashId)
        {
            textureMapPtr->render(command.hashId, command.dstRect, renderer);
        }
        else
        {
            groundTexture->render(renderer, command.srcRect, command.dstRect);
        }
    }

    auto elapsedBuildings = std::chrono::high_resolution_clock::now() - startTimeBuildings;

    for (auto &city : gameState->getCities())
    {
        city->renderCity(renderer);
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    int64_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    int64_t milliBuildings = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedBuildings).count();
    if (milliseconds >= 10)
    {
        APP_LOG_TRACE("update map time: " + std::to_string(milliseconds) + "ms");
        APP_LOG_TRACE("update building time: " + std::to_string(milliBuildings) + "ms");
        APP_LOG_TRACE("number of tiles: " + std::to_string(tilesX * tilesY));
        APP_LOG_TRACE("factor: " + std::to_string(factor));
    }

    if (updateMiniMap)
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
