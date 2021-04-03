/*
 * TextureManager.cpp
 *
 *  Created on: 14.01.2019
 *      Author: stefan
 */

#include "engine/graphics/TextureManager.h"

namespace graphics
{

    TextureManager::~TextureManager()
    {
        textures.clear();
        //fonts.clear();
        textureMaps.clear();
    }
    void TextureManager::updateRessources()
    {

        for (auto it = textures.begin(); it != textures.end(); it++)
        {
            if ((*it).second.use_count() == 0)
            {
                textures.erase(it);
            }
        }

        for (auto it = fonts.begin(); it != fonts.end(); it++)
        {
            if ((*it).second.use_count() == 0)
            {
                fonts.erase(it);
            }
        }

        for (auto it = textureMaps.begin(); it != textureMaps.end(); it++)
        {
            if ((*it).second.use_count() == 0)
            {
                textureMaps.erase(it);
            }
        }
    }

}
