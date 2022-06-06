#include "AIThread.h"
#include "../messages.h"
#include <ctime>
#include <engine/utils/os.h>
#include <iostream>
#include <world/actions/BaseAction.h>
#include <world/actions/ProductionAction.h>

namespace world
{
    AIThread::AIThread(std::shared_ptr<world::GameState> gameState)
        : gameState(gameState), running(false), speed(300)
    {

        running = true;
        thread = std::thread(&AIThread::update, this);

        utils::os::SetThreadName(&thread, "AIThread");
    }
    AIThread::~AIThread()
    {
        stop();
        thread.join();
    }

    bool AIThread::getPaused() const
    {
        return paused;
    }

    int AIThread::getSpeed() const
    {
        return speed;
    }

    void AIThread::setSpeed(int value)
    {
        speed = value;
    }

    void AIThread::stop()
    {
        running = false;
    }

    void AIThread::start()
    {
        running = true;
        paused = false;
    }
    void AIThread::pause()
    {
        paused = true;
    }

    void AIThread::update()
    {

        while (this->running)
        {

            while (!paused && running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));
                auto start = std::chrono::high_resolution_clock::now();

                for (auto &company : gameState->getCompanies())
                {
                    if (company->isPLayer())
                        continue;

                    auto action = company->currentAction();
                    if (action == nullptr)
                    {
                        action = std::make_shared<world::actions::BaseAction>(company);
                    }
                    if (!running)
                        return;
                    action->execute(gameState);
                    company->setCurrentAction(action->nextAction());
                }
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                if (microseconds > 1000)
                    std::cout << "ai thread time: " << microseconds << "µs" << std::endl;
            }
        }
    }

}