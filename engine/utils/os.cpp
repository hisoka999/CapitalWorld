/*
 * os.cpp
 *
 *  Created on: 11.07.2016
 *      Author: stefan
 */

#include "engine/utils/os.h"
#ifdef __linux
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif
#include <SDL2/SDL.h>

namespace utils
{

namespace os
{

const std::string path_separator()
{
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

const std::string get_home_dir()
{
#ifdef __linux
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL)
    {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return homedir;
#else
    return "undefined";
#endif
}

const std::string get_prog_dir()
{
    char *base_path = SDL_GetBasePath();
    if (base_path)
    {
        return base_path;
    }
    else
    {
        return SDL_strdup("./");
    }
}
const std::string get_current_work_dir()
{
#ifdef __linux
    return get_current_dir_name();
#else
    return SDL_strdup("./");
#endif
}

const std::string get_pref_dir(const std::string& org, const std::string& app)
{
    return SDL_GetPrefPath(org.c_str(), app.c_str());
}

}
}
