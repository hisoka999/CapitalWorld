#pragma once

#include "gamestate.h"
#include "world/actions/Action.h"
#include <atomic>
#include <memory>
#include <thread>

namespace world
{
    class AIThread
    {
    public:
        AIThread(std::shared_ptr<world::GameState> gameState);
        ~AIThread();

        void stop();
        void start();
        void pause();
        void setSpeed(int value);
        int getSpeed() const;

        bool getPaused() const;

    private:
        void update();
        std::shared_ptr<world::GameState> gameState;
        std::thread thread;
        std::atomic_bool running;
        bool paused;
        int speed;
        std::vector<std::shared_ptr<world::actions::Action>> actions;
    };

}