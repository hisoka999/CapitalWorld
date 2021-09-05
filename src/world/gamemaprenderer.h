#ifndef GAMEMAPRENDERER_H
#define GAMEMAPRENDERER_H

#include <memory>
#include <engine/core/renderer.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/TextureManager.h>
#include <engine/graphics/texturemap.h>
#include "world/gamemap.h"

struct Autotile
{
    size_t baseHash;
    size_t top_right;
    size_t top_left;
    size_t bottom_right;
    size_t bottom_left;
    size_t bottom_right_corner;
    size_t top_left_corner;
    size_t left;
    size_t right;
    size_t top;
    size_t bottom;
};

class GameMapRenderer
{
public:
    GameMapRenderer(std::shared_ptr<GameMap> gameMap);

    void render(core::Renderer *renderer);
    size_t getTileWidth();
    size_t getTileHeight();
    float getTileYOffset(uint16_t tile, size_t tileX, size_t tileY);
    void clearCache();
    void refreshMiniMap();
    std::shared_ptr<graphics::Texture> &getMiniMap();

private:
    void renderMiniMap(core::Renderer *renderer);
    const graphics::Rect &getAutoTile(const TileType tile, size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit);
    void fillAutoTileMap();
    Autotile generateAutoTile(std::string base);

    void renderTile(core::Renderer *renderer, const core::Camera *camera, const float factor, const uint16_t tile, const int tileX, const int tileY, const utils::Vector2 &pos);
    utils::Vector2 convertVec2(float zoomFactor, utils::Vector2 input);
    const graphics::Rect &getSourceRect(const TileType tile, const size_t tileX, const size_t tileY);
    std::shared_ptr<GameMap> gameMap;
    std::shared_ptr<graphics::Texture> groundTexture;
    std::shared_ptr<graphics::Text> debugText;
    std::shared_ptr<graphics::TextureMap> textureMap;

    std::shared_ptr<graphics::Texture> cacheTexture;
    std::shared_ptr<graphics::Texture> miniMap;
    bool updateMiniMap = true;
    size_t tileWidth;
    size_t tileHeight;
    std::hash<std::string_view> hasher;
    size_t sandHash;
    size_t grassHash;
    size_t waterHash;
    size_t grass1Hash;
    size_t grasRockHash;
    size_t treesHash;
    bool fillCache = false;
    std::map<size_t, Autotile> autoTileMap;
};

#endif // GAMEMAPRENDERER_H
