#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include <engine/graphics/texture.h>
#include <memory>
#include <engine/utils/IniBase.h>
#include <engine/core/renderer.h>

namespace graphics {

class TextureMap
{
public:
    TextureMap();
    void loadFromFile(const std::string& fileName);
    void render(const std::string& subTexture,graphics::Rect destRect,core::Renderer* renderer);
    std::string getFileName();
    graphics::Rect getSourceRect(const std::string& subTexture);
private:
    std::string textureName;
    std::shared_ptr<graphics::Texture> texture;
    utils::IniBase iniFile;
    std::map<std::string,graphics::Rect> subTextures;
};


}


#endif // TEXTUREMAP_H
