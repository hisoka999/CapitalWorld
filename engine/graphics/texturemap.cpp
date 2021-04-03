#include "texturemap.h"
#include <engine/graphics/TextureManager.h>

namespace graphics {

TextureMap::TextureMap()
{
}
std::string TextureMap::getFileName()
{
    return textureName;
}
void TextureMap::render(const std::string& subTexture, graphics::Rect destRect, core::Renderer* renderer)
{
    texture->render(renderer, subTextures[subTexture], destRect);
}
void TextureMap::loadFromFile(const std::string& fileName)
{
    textureName = fileName;
    iniFile.Setfilename(fileName);
    iniFile.read();
    std::string textureName = iniFile.getValue("TEXTURE_MAP", "texture");

    texture = graphics::TextureManager::Instance().loadTexture(textureName);

    size_t subTextureNum = static_cast<size_t>(iniFile.getValueI("TEXTURES", "tex\\size"));

    for (size_t i = 1; i <= subTextureNum; ++i) {
        std::string baseName = "tex\\" + std::to_string(i) + "\\";
        graphics::Rect textureData;
        std::string subTextureName = iniFile.getValue("TEXTURES", baseName + "name");
        textureData.x = iniFile.getValueI("TEXTURES", baseName + "x");
        textureData.y = iniFile.getValueI("TEXTURES", baseName + "y");
        textureData.width = iniFile.getValueI("TEXTURES", baseName + "width");
        textureData.height = iniFile.getValueI("TEXTURES", baseName + "height");
        subTextures[subTextureName] = textureData;
    }
}
graphics::Rect TextureMap::getSourceRect(const std::string& subTexture)
{
    return subTextures[subTexture];
}

}
