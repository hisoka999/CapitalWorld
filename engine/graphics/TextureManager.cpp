/*
 * TextureManager.cpp
 *
 *  Created on: 14.01.2019
 *      Author: stefan
 */


#include "engine/graphics/TextureManager.h"

namespace graphics {

TextureManager* TextureManager::instance = nullptr;
std::once_flag TextureManager::onceFlag;


}

