#include "updatethread.h"
#include <ctime>
#include <iostream>
#include <engine/utils/os.h>

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
            std::time_t tmpTime = std::chrono::system_clock::to_time_t(gameState->getTime());
            std::tm *localTime = localtime(&tmpTime);
            if (localTime->tm_mday == 1)
            {
                //update game state
                auto company = gameState->getPlayer();
                company->updateBalance(localTime->tm_mon, localTime->tm_year);
                std::cout << std::asctime(localTime) << ": update game state" << std::endl;
            }
        }
    }
}
