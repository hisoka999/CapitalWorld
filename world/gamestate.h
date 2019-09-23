#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "world/company.h"
#include <ctime>
#include <chrono>
#include <memory>
#include <vector>

namespace world{

enum class TimeState{
    Normal,
    Fast,
    VeryFast,
    Pause,
};

class GameState
{
public:
    GameState(std::shared_ptr<Company> player);

    void setTimeState(TimeState state);
    void increaseTime();
    std::chrono::system_clock::time_point getTime();
    std::shared_ptr<world::Company> getPlayer();

private:
    std::chrono::system_clock::time_point time;
    std::vector<std::shared_ptr<world::Company>> companies;
    std::shared_ptr<Company> player;
    TimeState timeState;
};
}
#endif // GAMESTATE_H
