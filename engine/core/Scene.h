#ifndef CORE_SCENE_H_
#define CORE_SCENE_H_

#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/graphics/texture.h"
#include "engine/ui/windowmanager.h"
#include <engine/core/Music.h>
#include <memory>

namespace core
{

    class Scene
    {
    public:
        Scene(core::Renderer *pRenderer);
        virtual ~Scene();

        virtual void render() = 0;
        virtual void handleEvents(core::Input *pInput) = 0;
        virtual void update(){};
        virtual void unload();

    protected:
        core::Renderer *renderer;
        std::shared_ptr<UI::WindowManager> winMgr;
        std::shared_ptr<core::Music> music;

    private:
    };

} /* namespace core */

#endif /* CORE_SCENE_H_ */
