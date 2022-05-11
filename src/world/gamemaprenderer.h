#ifndef GAMEMAPRENDERER_H
#define GAMEMAPRENDERER_H

#include "world/gamestate.h"
#include <engine/core/renderer.h>
#include <engine/graphics/TextureManager.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/texturemap.h>
#include <memory>

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
    GameMapRenderer(std::shared_ptr<world::GameState> gameState);

    void render(core::Renderer *renderer);
    size_t getTileWidth();
    size_t getTileHeight();
    float getTileYOffset(size_t tileX, size_t tileY);
    void clearCache();
    void refreshMiniMap();
    std::shared_ptr<graphics::Texture> &getMiniMap();
    std::shared_ptr<graphics::TextureMap> &getTextureMap();

private:
    void renderMiniMap(core::Renderer *renderer);
    void generateTileDataFromMap();
    graphics::Rect getAutoTile(size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit);
    size_t getAutoTileId(size_t baseTile, const size_t tileX, const size_t tileY, const TileType groundLimit);
    size_t getSourceTile(const TileType tile, const size_t tileX, const size_t tileY);
    void fillAutoTileMap();
    Autotile generateAutoTile(std::string base);

    void renderTile(core::Renderer *renderer, const core::Camera *camera, const float factor, const int tileX, const int tileY, const utils::Vector2 &pos);
    void renderResource(core::Renderer *renderer, const core::Camera *camera, const float factor, const int tileX, const int tileY, const utils::Vector2 &pos);
    utils::Vector2 convertVec2(float zoomFactor, utils::Vector2 input);
    const graphics::Rect getSourceRect(const TileType tile, const size_t tileX, const size_t tileY);
    std::shared_ptr<world::GameState> gameState;
    std::vector<size_t> tileData;
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
    size_t resourceRockHash;
    size_t resourceOilHash;
    bool fillCache = false;
    std::map<size_t, Autotile> autoTileMap;
};

#endif // GAMEMAPRENDERER_H
