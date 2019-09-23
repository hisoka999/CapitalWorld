/*
 * Scene.h
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#ifndef CORE_SCENE_H_
#define CORE_SCENE_H_

#include "engine/core/renderer.h"
#include "engine/core/input.h"
#include "engine/ui/windowmanager.h"
#include "engine/graphics/texture.h"
#include <memory>

namespace core
{

class Scene
{
public:
    Scene(core::Renderer *pRenderer, graphics::Texture* pUITexture);
    virtual ~Scene();

    virtual void render() = 0;
    virtual void handleEvents(core::Input *pInput) = 0;
    virtual void update() {
    }
    ;
protected:
    core::Renderer* renderer;
    std::shared_ptr<UI::WindowManager> winMgr;

};

} /* namespace core */

#endif /* CORE_SCENE_H_ */
