/*
 * os.cpp
 *
 *  Created on: 11.07.2016
 *      Author: stefan
 */

#include "engine/utils/os.h"
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <SDL2/SDL.h>

namespace utils {

namespace os {

    const std::string path_separator()
    {
#ifdef __WIN32__
        return "\\";
#else
        return "/";
#endif
    }

    const std::string get_home_dir()
    {
#ifdef __linux
        const char* homedir;

        if ((homedir = getenv("HOME")) == nullptr) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return homedir;
#else
        return "undefined";
#endif
    }

    const std::string get_prog_dir()
    {
        char* base_path = SDL_GetBasePath();
        if (base_path) {
            return base_path;
        } else {
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

    bool is_dir(const std::string path)
    {
#ifdef __linux
        struct stat sb;
        return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
#elif __WIN32__
        DWORD dwAttrib = GetFileAttributes(path.c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#endif
    }

    bool is_file(const std::string path)
    {
#ifdef __linux
        struct stat sb;
        return stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode);
#elif __WIN32__
        DWORD dwAttrib = GetFileAttributes(path.c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES );
#endif
    }

    void create_dir(const std::string path)
    {
#ifdef __linux
        mkdir(path.c_str(), 0755);
#elif __WIN32__
        CreateDirectory(path.c_str(),nullptr);
#endif
    }

}
}
