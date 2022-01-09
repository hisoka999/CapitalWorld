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
                // update game state
                auto start = std::chrono::high_resolution_clock::now();

                gameState->update();

                std::cout << time.format() << ": update game state" << std::endl;
                auto &msgSystem = core::MessageSystem<MessageTypes>::get();
                std::shared_ptr<core::Message<MessageTypes, int>> msg = std::make_shared<core::Message<MessageTypes, int>>(MessageTypes::NewMonth, 0);
                msgSystem.sendMessage(msg);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                std::cout << "update thread time: " << microseconds << "µs" << std::endl;
            }

            // // first day of the year
            // uint16_t year = time.getYear();
            // int letter = int(1 + year + std::floor(float(year / 4)) + std::floor(float((year - 1600) / 400)) - std::floor(float((year - 1600) / 100))) % 7;
            // std::string weekday = "";

            // int dayOfYear = 0;
            // for (int month = 0; month <= time.getMonth(); month++)
            // {
            //     if (month < time.getMonth())
            //         dayOfYear += utils::time::lastDayOfMonth(year, time.getMonth());
            //     else
            //         dayOfYear += time.getDay();
            // }
            // int weekOfYear = dayOfYear / 7;
            // int dayOfWeek = (letter + dayOfYear) - (weekOfYear * 7) - 1;

            // std::array<std::string, 7> weekDayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"

            // };

            // switch (letter)
            // {
            // case 6:
            //     weekday = "Sunday";
            //     break;
            // case 5:
            //     weekday = "Saturday";
            //     break;
            // case 4:
            //     weekday = "Friday";
            //     break;
            // case 3:
            //     weekday = "Thursday";
            //     break;
            // case 2:
            //     weekday = "Wednesday";
            //     break;
            // case 1:
            //     weekday = "Tuesday";
            //     break;
            // case 0:
            //     weekday = "Monday";
            //     break;
            // }
            // std::cout << "current date: " << time.format() << std::endl;
            // std::cout << "gregorian starting day: " << weekDayNames[letter] << std::endl;
            // std::cout << "day of the year: " << dayOfYear << " week of year: " << weekOfYear << std::endl;
            // std::cout << "current day of the week:" << dayOfWeek << " as Name: " << weekDayNames[dayOfWeek] << std::endl;
        }
    }
}
