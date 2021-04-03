/*
 * Music.cpp
 *
 *  Created on: 02.06.2019
 *      Author: stefan
 */

#include <engine/core/Music.h>
#include <stdexcept>

namespace core {

Music::Music()
    : _data(nullptr)
{
}

Music::~Music()
{
    Mix_FreeMusic(_data);
    _data = nullptr;
}

} /* namespace core */

void core::Music::loadMusic(std::string filename)
{
    _data = Mix_LoadMUS(filename.c_str());
    if (_data == nullptr) {
        throw std::runtime_error(
            std::string("failed to load music: ")
            + std::string(Mix_GetError()));
    }
}

void core::Music::play(int repeat)
{
    Mix_PlayMusic(_data, repeat);
}

void core::Music::pause()
{
    Mix_PauseMusic();
}

void core::Music::resume()
{
    Mix_ResumeMusic();
}

void core::Music::stop()
{
    Mix_HaltMusic();
}

bool core::Music::isMusicPlaying()
{
    return Mix_PlayingMusic() == 1;
}
