/*
 * Music.h
 *
 *  Created on: 02.06.2019
 *      Author: stefan
 */

#ifndef CORE_MUSIC_H_
#define CORE_MUSIC_H_
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace core {

class Music {
public:
	Music();
	virtual ~Music();
	void loadMusic(std::string filename);
	void play(int repeat);
	void pause();
	void resume();
	void stop();

	bool isMusicPlaying();

private:
	Mix_Music* _data;
};

} /* namespace core */

#endif /* CORE_MUSIC_H_ */
