/*
 * Animation.h
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#ifndef GRAPHICS_ANIMATION_H_
#define GRAPHICS_ANIMATION_H_

#include <SDL2/SDL.h>
#include <engine/core/renderer.h>
#include <engine/utils/vector2.h>
#include <vector>

namespace graphics {

template <typename T>
struct AnimationFrame {
    utils::Vector2 position;
    unsigned int time;
    T* data;

    AnimationFrame(utils::Vector2& pos, unsigned int time, T* data)
    {
        position = pos;
        this->time = time;
        this->data = data;
    }
};

template <typename T>
class Animation {
public:
    Animation(utils::Vector2 startPosition);
    virtual ~Animation();

    bool isPlaying();
    void play();
    void stop();
    void pause();

    void update();
    void render(core::Renderer* renderer);
    void addFrame(AnimationFrame<T> frame);

protected:
    virtual void renderFrame(AnimationFrame<T>& frame,
        core::Renderer* renderer)
        = 0;
    utils::Vector2 currentPosition;

private:
    std::vector<AnimationFrame<T>> frames;
    bool playing;
    int currentFrame;
    int startTime = 0;

    utils::Vector2 startPosition;
};

} /* namespace graphics */

template <typename T>
inline graphics::Animation<T>::Animation(utils::Vector2 startPosition)
    : playing(false)
    , currentFrame(-1)
    , startPosition(startPosition)
{
}

template <typename T>
inline graphics::Animation<T>::~Animation()
{
}

template <typename T>
inline bool graphics::Animation<T>::isPlaying()
{
    return playing;
}

template <typename T>
inline void graphics::Animation<T>::play()
{
    currentFrame = 0;
    playing = true;
    startTime = SDL_GetTicks();
    currentPosition = startPosition;
}

template <typename T>
inline void graphics::Animation<T>::stop()
{
    currentFrame = 0;
    playing = false;
}

template <typename T>
inline void graphics::Animation<T>::pause()
{
    playing = false;
}

template <typename T>
inline void graphics::Animation<T>::update()
{
    auto& nextFrame = frames[currentFrame + 1];
    int time = SDL_GetTicks() - startTime;
    currentPosition = utils::lerp(frames[currentFrame].position,
        nextFrame.position,
        ((float)nextFrame.time - (float)time) / (float)nextFrame.time);
    if (time > frames[currentFrame].time) {
        currentFrame++;
        startTime = SDL_GetTicks();
        if (currentFrame >= frames.size())
            stop();
    }
}

template <typename T>
inline void graphics::Animation<T>::render(core::Renderer* renderer)
{
    renderFrame(frames[currentFrame], renderer);
}

template <typename T>
inline void graphics::Animation<T>::addFrame(AnimationFrame<T> frame)
{
    frames.push_back(frame);
}

#endif /* GRAPHICS_ANIMATION_H_ */
