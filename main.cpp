#include <iostream>
#include "engine/core/gamewindow.h"
#include "engine/core/renderer.h"
#include "engine/core/input.h"
#include "engine/core/SceneManager.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/graphics/TextureManager.h"
#include "scenes/MainScene.h"
#include "scenes/worldscene.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/os.h"
#include "engine/utils/logger.h"
#include "engine/utils/string.h"
#include "config.h"
#include "services/productservice.h"

int main()
{
    try
    {

        core::GameWindow win(utils::string_format("CapitalWorld %d.%d",GAME_VERSION_MAJOR,GAME_VERSION_MINOR), 1280, 720);
        utils::Logger logger(utils::LogLevel::trace);
        core::Renderer ren(logger);
        graphics::TextureManager::Instance().setRenderer(&ren);
        core::Input input;
        core::SceneManager sceneManager;
        win.open();
        ren.open(&win, true);
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);

        services::ProductService::Instance().loadResources("");
        services::ProductService::Instance().loadProducts("");
        scenes::MainScene mainScene(&ren,&sceneManager);
        scenes::WorldScene worldScene(&ren,&sceneManager);
        sceneManager.addScene("main",&mainScene);
        sceneManager.addScene("world",&worldScene);
        sceneManager.setCurrentScene("main");



        unsigned int lastTime = ren.getTickCount();
        unsigned int frames = 0;
        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color =
        { 200, 200, 0, 0 };

        unsigned int delay = 0;

        bool run = true;
        while (run && mainScene.isRunning())
        {
            ren.clear();
            try
            {
                while (input.poll())
                {
                    if (input.isKeyDown(SDLK_ESCAPE) || input.isQuit())
                        run = false;
                    sceneManager.handleEvents(&input);
                }
            } catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            sceneManager.render();
            frames++;
            if ((ren.getTickCount() - lastTime) >= 1000)
            {
                lastTime = ren.getTickCount();
                fps = frames;
                frames = 0;
                if (fps > 300)
                {
                    delay++;
                    std::cout << "delay => " << delay << std::endl;
                }
                else
                {
                    if (delay > 0)
                        delay--;
                }
            }

            text.render(&ren, "FPS: " + std::to_string(fps), color, 850, 5);
            ren.renderPresent();

            win.delay(delay);
            ren.calcDelta();
        }
    } catch (SDLException &e)
    {
        std::cerr << "SDL Exception: " << e.what() << std::endl;
    } catch (std::exception &e)
    {
        std::cerr << "unkown standard exception: " << e.what() << std::endl;
    }
    return 0;
}
