/*
 * os.h
 *
 *  Created on: 09.06.2016
 *      Author: stefan
 */

#ifndef UTILS_OS_H_
#define UTILS_OS_H_

#include <initializer_list>
#include <string>

namespace utils {

namespace os {

    const std::string path_separator();

    template <class T>
    std::string combine(T t)
    {
        return t;
    }

    template <class T, typename... Args>
    std::string combine(T t, Args... args)
    {
        return t + path_separator() + combine(args...);
    }
    const std::string get_home_dir();

    const std::string get_prog_dir();

    const std::string get_pref_dir(const std::string& org, const std::string& app);

    const std::string get_current_work_dir();

    bool is_dir(const std::string path);

    bool is_file(const std::string path);

    void create_dir(const std::string path);
}
}

#endif /* UTILS_OS_H_ */
