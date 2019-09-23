/*
 * Timer.cpp
 *
 *  Created on: 11.07.2017
 *      Author: stefan
 */

#include "engine/core/Timer.h"

namespace core {

Timer::Timer(const unsigned int time, const unsigned int repeat) :
        _time(time), _repeat(repeat), _execs(0) {
    _startTime = 0;

}

Timer::~Timer() {
    // TODO Auto-generated destructor stub
}

void Timer::start() {
    _startTime = SDL_GetTicks();
    _execs = 0;
}
void Timer::stop() {
    _startTime = 0;
}
void Timer::pause() {

}

void Timer::update() {
    if (_startTime == 0)
        return;

    if (_startTime + _time > SDL_GetTicks()) {
        call(_callback);

        _startTime = SDL_GetTicks();
        if (_repeat > 0) {
            _execs++;
            if (_execs > _repeat) {
                stop();
            }
        }
    }
}


} /* namespace core */
