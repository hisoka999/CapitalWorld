#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <memory>
#include <thread>
#include <atomic>

#include "../world/gamestate.h"

class UpdateThread
{
public:
    UpdateThread(std::shared_ptr<world::GameState> gameState);
    ~UpdateThread();

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
};

#endif // UPDATETHREAD_H
