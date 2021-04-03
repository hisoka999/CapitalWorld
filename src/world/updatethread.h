#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <memory>
#include <thread>
#include "../world/gamestate.h"

class UpdateThread
{
public:
    UpdateThread(std::shared_ptr<world::GameState> gameState);
    ~UpdateThread();
    void stop();

private:
    void update();
    std::shared_ptr<world::GameState> gameState;
    std::thread thread;
    bool running;
};

#endif // UPDATETHREAD_H
