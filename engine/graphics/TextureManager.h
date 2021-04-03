/*
 * TextureManager.h
 *
 *  Created on: 16.05.2018
 *      Author: stefan
 */

#ifndef GRAPHICS_TEXTUREMANAGER_H_
#define GRAPHICS_TEXTUREMANAGER_H_

#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/texturemap.h>
#include <map>
#include <memory>
#include <mutex>

namespace graphics
{

    class TextureManager
    {
    public:
        static TextureManager &Instance()
        {
            static TextureManager instance;
            return instance;
        }

        const std::shared_ptr<graphics::Texture> loadTexture(const std::string &filename)
        {
            if (renderer == nullptr)
            {
                throw std::runtime_error("TextureManager has no renderer");
            }
            if (textures.count(filename) > 0)
                return textures[filename];

            auto texture = std::make_shared<graphics::Texture>();
            texture->loadTexture(renderer, filename);
            textures[filename] = texture;
            return textures[filename];
        }
        const std::shared_ptr<graphics::Text> loadFont(std::string filename,
                                                       int fontSize)
        {
            if (renderer == nullptr)
            {
                throw std::runtime_error("TextureManager has no renderer");
            }
            std::string cache = filename + "#" + std::to_string(fontSize);
            if (fonts.count(cache) == 0)
            {
                auto font = std::make_shared<graphics::Text>();
                font->openFont(filename, fontSize);
                fonts[cache] = font;
            }

            return fonts[cache];
        }

        std::shared_ptr<graphics::TextureMap> loadTextureMap(std::string filename)
        {
            if (renderer == nullptr)
            {
                throw std::runtime_error("TextureManager has no renderer");
            }
            if (textureMaps.count(filename) > 0)
                return textureMaps[filename];

            auto textureMap = std::make_shared<graphics::TextureMap>();
            textureMap->loadFromFile(filename);
            textureMaps[filename] = textureMap;
            return textureMaps[filename];
        }

        void setRenderer(core::Renderer *renderer)
        {
            this->renderer = renderer;
        }

        void updateRessources();

    private:
        TextureManager() = default;
        ~TextureManager();
        TextureManager(const TextureManager &) = delete;
        TextureManager &operator=(const TextureManager &) = delete;

        std::map<std::string, std::shared_ptr<graphics::Texture>> textures;
        std::map<std::string, std::shared_ptr<graphics::Text>> fonts;
        std::map<std::string, std::shared_ptr<graphics::TextureMap>> textureMaps;

        core::Renderer *renderer = nullptr;
    };

} /* namespace graphics */

#endif /* GRAPHICS_TEXTUREMANAGER_H_ */
