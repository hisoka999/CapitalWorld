#include "GameStateMutex.h"
#include "updatethread.h"
#include <ctime>
#include <iostream>
#include <engine/utils/os.h>
#include "../messages.h"

UpdateThread::UpdateThread(std::shared_ptr<world::GameState> gameState)
    : gameState(gameState), running(false), speed(300)
{

    running = true;
    thread = std::thread(&UpdateThread::update, this);

    utils::os::SetThreadName(&thread, "UpdateThread");
}
UpdateThread::~UpdateThread()
{
    stop();
    thread.join();
}

bool UpdateThread::getPaused() const
{
    return paused;
}

int UpdateThread::getSpeed() const
{
    return speed;
}

void UpdateThread::setSpeed(int value)
{
    speed = value;
}

void UpdateThread::stop()
{
    running = false;
}

void UpdateThread::start()
{
    running = true;
    paused = false;
}
void UpdateThread::pause()
{
    paused = true;
}

void UpdateThread::update()
{
    while (this->running)
    {

        while (!paused && running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
            gameState->increaseTime();
            auto time = gameState->getTime();
            gameState->updateDaily();
            if (time.getDay() == 1)
            {
                gGameStateMutex.lock();
                // update game state
                auto start = std::chrono::high_resolution_clock::now();
                if (!running)
                    return;
                gameState->update();

                gGameStateMutex.unlock();

                auto &msgSystem = core::MessageSystem<MessageTypes>::get();
                msgSystem.sendMessage<int>(MessageTypes::NewMonth, 0);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
                if (milliseconds > 100)
                    std::cout << "update thread time: " << milliseconds << "ms" << std::endl;
            }
        }
    }
}
