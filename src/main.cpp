#include "config.h"
#include "engine/core/renderer.h"
#include "engine/graphics/TextureManager.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/localisation.h"
#include "engine/utils/logger.h"
#include "engine/utils/os.h"
#include "engine/utils/string.h"
#include "scenes/MainScene.h"
#include "scenes/NewGameScene.h"
#include "scenes/worldscene.h"
#include "services/buildingservice.h"
#include "services/productservice.h"
#include "services/researchservice.h"
#include "services/ressourceservice.h"
#include <chrono>
#include <engine/core/SceneManager.h>
#include <engine/core/gamewindow.h>
#include <engine/core/input.h>
#include <iostream>

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

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    if (argc == 2)
    {
        std::string arg = std::string(argv[1]);
        if (arg == "--version")
        {
            std::cout << GAME_VERSION_MAJOR << "." << GAME_VERSION_MINOR << std::endl;
            return 0;
        }
    }

    try
    {
        //#ifdef NDEBUG
        std::string fileName = "locale/enum.pot";
        generateEnumPot(fileName);
        //#endif
        Localisation::Instance().detectLanguage("capitalworld");
        Localisation::Instance().detectLanguage("enum");

        auto &win = core::GameWindow::Instance(); //(utils::string_format("CapitalWorld %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR), 1280, 720);
        win.open(utils::string_format("CapitalWorld %d.%d", GAME_VERSION_MAJOR, GAME_VERSION_MINOR), 1280, 720, "captialworld");

        auto lang = win.getSettings()->getValue("Base", "Lang");
        if (!lang.empty())
        {
            Language lng = magic_enum::enum_cast<Language>(lang).value();
            Localisation::Instance().loadLanguage(lng, "capitalworld");
            Localisation::Instance().loadLanguage(lng, "enum");
        }
        setlocale(LC_ALL, Localisation::Instance().getLocale().name().c_str());
        utils::Logger logger(utils::LogLevel::trace);
        core::Renderer ren(logger);
        graphics::TextureManager::Instance().setRenderer(&ren);
        graphics::TextureManager::Instance().loadTheme("data/theme.json", true);
        core::Input input;
        auto &sceneManager = core::SceneManager::Instance();

        ren.open(&win, win.getSettings()->getValueB("Base", "VSync"));
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);
        world::Building::initComponentMap();
        services::ResourceService::Instance().loadData("data/ressources.json");
        services::ProductService::Instance().loadData("data/products.json");
        services::BuildingService::Instance().loadData("data/buildings.json");
        services::ResearchService::Instance().loadData("data/research.json");

        auto mainScene = std::make_shared<scenes::MainScene>(&ren, &sceneManager);
        // auto worldScene = std::make_shared<scenes::WorldScene>(&ren, &sceneManager);
        auto newGameScene = std::make_shared<scenes::NewGameScene>(&ren, &sceneManager);
        sceneManager.addScene("main", mainScene);
        // sceneManager.addScene("world", worldScene);
        sceneManager.addScene("newGameScene", newGameScene);
        sceneManager.setCurrentScene("main");

        unsigned int lastTime = ren.getTickCount();
        unsigned int lastUpdateTime = ren.getTickCount();

        unsigned int frames = 0;

        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color = {200, 200, 0, 0};

        bool run = true;
        lastUpdateTime = 0;
        while (run && mainScene->isRunning())
        {
            ren.setDrawColor(0, 0, 0, 255);
            ren.clear();
            bool saveScreenshot = false;
            // try
            //{
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
            //}
            // catch (std::exception &e)
            // {
            //     std::cerr << e.what() << std::endl;
            // }
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
            lastUpdateTime += ren.getTimeDelta();
            if (lastUpdateTime >= 40)
            {

                sceneManager.fixedUpdate(40);
                lastUpdateTime -= 40;
            }
            sceneManager.update();
            text.render(&ren, "FPS: " + std::to_string(fps), color, 850, 5);
            text.render(&ren, "FT: " + std::to_string(ren.getTimeDelta()) + "ms", color, 850, 25);
            ren.renderPresent();
            if (saveScreenshot)
            {

                SDL_Surface *surf = SDL_CreateRGBSurface(0, win.getWidth(), win.getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
                SDL_RenderReadPixels(ren.getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, surf->pixels, surf->pitch);

                if (surf != 0)
                {
                    std::string fileName = utils::os::get_pref_dir("", "captialworld") + "screenshot_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".png";
                    std::cout << "screenshot: " << fileName << std::endl;
                    IMG_SavePNG(surf, fileName.c_str());
                    SDL_FreeSurface(surf);
                }
                else
                {
                    throw SDLException("SDL_GetWindowSurface");
                }
            }

            // win.delay(delay);

            ren.calcDelta();
        }
    }
    catch (SDLException &e)
    {
        std::cerr << "SDL Exception: " << e.what() << std::endl;
        return 1;
    }
    // catch (std::exception &e)
    // {
    //     std::cerr << "unkown standard exception: " << e.what() << std::endl;
    //     return 1;
    // }
    return 0;
}
