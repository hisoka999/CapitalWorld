#include "updatethread.h"
#include <ctime>
#include <iostream>

UpdateThread::UpdateThread(std::shared_ptr<world::GameState> gameState)
    :gameState(gameState),running(false)
{

    running = true;
    thread = std::thread(&UpdateThread::update, this);

}
UpdateThread::~UpdateThread()
{
    thread.detach();
}

void UpdateThread::stop()
{
    running = false;
}

void UpdateThread::update()
{
    while(running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        gameState->increaseTime();
        std::time_t tmpTime = std::chrono::system_clock::to_time_t(gameState->getTime());
        std::tm* localTime = localtime(&tmpTime);
        if(localTime->tm_mday == 1)
        {
            //update game state
            auto company = gameState->getPlayer();
            company->updateBalance(localTime->tm_mon,localTime->tm_year);
            std::cout<<std::asctime(localTime)<<": update game state"<<std::endl;

        }
    }
}
