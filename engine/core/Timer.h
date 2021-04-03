/*
 * Timer.h
 *
 *  Created on: 11.07.2017
 *      Author: stefan
 */

#ifndef CORE_TIMER_H_
#define CORE_TIMER_H_

#include <SDL2/SDL.h>
#include "dispatcher.h"

namespace core {

class Timer {
public:
	Timer(const unsigned int time, const unsigned int repeat);
	virtual ~Timer();
	template<typename F>
	void setCallback(F&& f) {
		_callback = core::make_dispatcher(std::forward<F>(f));
	}

	void start();
	void stop();
	void pause();
	void update();

private:
	template<typename F, typename ... Args>
	void call(F const& f, Args const&... args) {
        std::vector<std::any> v { args... };
		f(v);
	}


	unsigned int _startTime;
	unsigned int _time;
	unsigned int _repeat;
	unsigned int _execs;

	core::dispatcher_type _callback;
};

} /* namespace core */

#endif /* CORE_TIMER_H_ */
