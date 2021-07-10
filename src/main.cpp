#include "config.h"
#include <engine/core/SceneManager.h>
#include <engine/core/gamewindow.h>
#include <engine/core/input.h>
#include "engine/core/renderer.h"
#include "engine/graphics/TextureManager.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/logger.h"
#include "engine/utils/os.h"
#include "engine/utils/string.h"
#include "scenes/MainScene.h"
#include "scenes/worldscene.h"
#include "scenes/NewGameScene.h"
#include "services/productservice.h"
#include "services/ressourceservice.h"
#include "services/buildingservice.h"
#include <iostream>
#include "engine/utils/localisation.h"

#include <magic_enum.hpp>

template <size_t SIZE>
void writeEnumArray(std::array<std::string_view, SIZE> names, std::ofstream &stream)
{
    for (auto name : names)
    {
        stream << "msgid \"" << name << "\"\n";
        stream << "msgstr \"\"\n\n";
    }
}

void generateEnumPot(std::string fileName)
{
    std::ofstream os(fileName, std::ios::trunc | std::ios::out);
    std::cout << "test file: " << fileName << std::endl;

    writeEnumArray(magic_enum::enum_names<world::BuildingType>(), os);
    writeEnumArray(magic_enum::enum_names<world::CityType>(), os);
    writeEnumArray(magic_enum::enum_names<world::BalanceAccount>(), os);
    writeEnumArray(magic_enum::enum_names<WorldSize>(), os);
    writeEnumArray(magic_enum::enum_names<Difficulty>(), os);

    os.flush();
    os.close();
}

int main(int argc, char *argv[])
{
    try
    {
        //#ifdef NDEBUG
        std::string fileName = "locale/enum.pot";
        generateEnumPot(fileName);
        //#endif
        Localisation::Instance().detectLanguage("capitalworld");
        Localisation::Instance().detectLanguage("enum");

        setlocale(LC_ALL, "de_DE");
        auto &win = core::GameWindow::Instance(); //(utils::string_format("CapitalWorld %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR), 1280, 720);
        win.open(utils::string_format("CapitalWorld %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR), 1280, 720, "captialworld");
        utils::Logger logger(utils::LogLevel::trace);
        core::Renderer ren(logger);
        graphics::TextureManager::Instance().setRenderer(&ren);
        core::Input input;
        auto &sceneManager = core::SceneManager::Instance();

        ren.open(&win, false);
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);
        services::RessourceService::Instance().loadData("data/ressources.json");
        services::ProductService::Instance().loadData("data/products.json");
        services::BuildingService::Instance().loadData("data/buildings.json");

        auto mainScene = std::make_shared<scenes::MainScene>(&ren, &sceneManager);
        //auto worldScene = std::make_shared<scenes::WorldScene>(&ren, &sceneManager);
        auto newGameScene = std::make_shared<scenes::NewGameScene>(&ren, &sceneManager);
        sceneManager.addScene("main", mainScene);
        //sceneManager.addScene("world", worldScene);
        sceneManager.addScene("newGameScene", newGameScene);
        sceneManager.setCurrentScene("main");

        unsigned int lastTime = ren.getTickCount();
        unsigned int lastUpdateTime = ren.getTickCount();

        unsigned int frames = 0;

        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color = {200, 200, 0, 0};

        unsigned int delay = 0;

        bool run = true;
        while (run && mainScene->isRunning())
        {
            ren.setDrawColor(0, 0, 0, 255);
            ren.clear();
            bool saveScreenshot = false;
            try
            {
                while (input.poll())
                {
                    if (input.isQuit())
                        run = false;
                    // else if (input.isKeyDown(SDLK_ESCAPE))
                    //     sceneManager.setCurrentScene("main");
                    else if (input.isKeyDown(SDLK_p))
                    {
                        saveScreenshot = true;
                    }

                    sceneManager.handleEvents(&input);
                }
            }
            catch (std::exception &e)
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
                // if (fps > 300)
                // {
                //     delay++;
                //     std::cout << "delay => " << delay << std::endl;
                // }
                // else
                // {
                //     if (delay > 0)
                //         delay--;
                // }
            }
            if (ren.getTickCount() - lastUpdateTime >= 40)
            {
                sceneManager.fixedUpdate(40);
                lastUpdateTime = ren.getTickCount();
            }
            sceneManager.update();
            text.render(&ren, "FPS: " + std::to_string(fps), color, 850, 5);
            ren.renderPresent();

            if (saveScreenshot)
            {

                SDL_Surface *surf = SDL_CreateRGBSurface(0, win.getWidth(), win.getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                SDL_RenderReadPixels(ren.getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, surf->pixels, surf->pitch);

                if (surf != 0)
                {
                    std::string fileName = utils::os::get_pref_dir("", "captialworld") + "screenshot_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".bmp";
                    std::cout << "screenshot: " << fileName << std::endl;
                    SDL_SaveBMP(surf, fileName.c_str());
                    SDL_FreeSurface(surf);
                }
                else
                {
                    throw SDLException("SDL_GetWindowSurface");
                }
            }

            //win.delay(delay);
            ren.calcDelta();
        }
    }
    catch (SDLException &e)
    {
        std::cerr << "SDL Exception: " << e.what() << std::endl;
    }
    // catch (std::exception &e)
    // {
    //     std::cerr << "unkown standard exception: " << e.what() << std::endl;
    // }
    return 0;
}
