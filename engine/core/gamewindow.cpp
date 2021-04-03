#include "engine/core/gamewindow.h"
#include "engine/utils/exceptions.h"
#include <SDL2/SDL_mixer.h>
#include <engine/utils/os.h>
#include <fstream>
#include <iostream>

namespace core
{

    GameWindow::GameWindow(const std::string pTitle, int pWidth, int pHeight)
        : win(nullptr), width(pWidth), height(pHeight), title(pTitle)
    {
        //ctor
        settings = std::make_shared<utils::IniBase>();
        open();
    }

    int GameWindow::open()
    {

        if (!utils::os::is_dir(utils::os::get_pref_dir("", "starconquest")))
        {
            utils::os::create_dir(utils::os::get_pref_dir("", "starconquest"));
            std::cout << "config dir: " << utils::os::get_pref_dir("", "starconquest") << std::endl;
        }

        std::string path = utils::os::combine(utils::os::get_pref_dir("", "starconquest"), "settings.conf");
        if (!utils::os::is_file(path))
        {
            std::ofstream filestream;
            filestream.open(path, std::ios::out);
            filestream << "[Base]" << std::endl;
            filestream << "Width=800" << std::endl;
            filestream << "Height=600" << std::endl;
            filestream << "Fullscreen=FALSE" << std::endl;

            std::cout << path << " created!" << std::endl;
            filestream.close();
        }
        settings->Setfilename(path);
        settings->read();
        width = settings->getValueI("Base", "Width");
        height = settings->getValueI("Base", "Height");

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        {
            throw SDLException("GameWindow::SDL_Init");
        }

#ifdef __WIN32
        //SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER,"d3dcompiler_46.dll");
        //sSDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG,"1");
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

#endif

        win = SDL_CreateWindow(title.c_str(), 100, 100, width, height,
                               SDL_WINDOW_SHOWN);
        if (win == nullptr)
        {
            throw SDLException("GameWindow::SDL_CreateWindow");
        }
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
        {
            throw SDLException("GameWindow::IMG_Init");
        }

        if (TTF_Init() != 0)
        {
            throw SDLException("GameWindow::TTF_Init");
        }

        //Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            throw SDLException(
                "SDL_mixer could not initialize! SDL_mixer Error:" + std::string(Mix_GetError()));
        }

        if (settings->getValueB("Base", "Fullscreen"))
        {
            SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
        }

        return 0;
    }

    GameWindow::~GameWindow()
    {
        if (win != nullptr)
        {
            SDL_DestroyWindow(win);
            TTF_Quit();
            IMG_Quit();
            Mix_Quit();
            SDL_Quit();
        }
    }
    const SDL_Window *GameWindow::getSDLWindow() const
    {
        return win;
    }

    void GameWindow::delay(unsigned int millsec)
    {
        SDL_Delay(millsec);
    }

    std::shared_ptr<utils::IniBase> GameWindow::getSettings() const
    {
        return settings;
    }

} // namespace core
