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
            if (time.getDay() == 1)
            {
                //update game state
                auto start = std::chrono::high_resolution_clock::now();

                gameState->update(time.getMonth(), time.getYear());

                std::cout << time.format() << ": update game state" << std::endl;
                auto &msgSystem = core::MessageSystem<MessageTypes>::get();
                std::shared_ptr<core::Message<MessageTypes, int>> msg = std::make_shared<core::Message<MessageTypes, int>>(MessageTypes::NewMonth, 0);
                msgSystem.sendMessage(msg);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                std::cout << "update thread time: " << microseconds << "µs" << std::endl;
            }
        }
    }
}
