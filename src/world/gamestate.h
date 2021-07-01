#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../world/company.h"
#include "world/city.h"
#include "world/gamemap.h"
#include <ctime>
#include <chrono>
#include <memory>
#include <vector>

namespace world
{

    enum class TimeState
    {
        Normal,
        Fast,
        VeryFast,
        Pause,
    };

    class GameState
    {
    public:
        GameState(const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities);

        void setTimeState(TimeState state);
        void increaseTime();
        std::chrono::system_clock::time_point getTime();
        const std::shared_ptr<world::Company> &getPlayer() const;
        const std::shared_ptr<GameMap> &getGameMap() const;
        const std::vector<std::shared_ptr<world::City>> &getCities() const;

    private:
        std::chrono::system_clock::time_point time;
        std::vector<std::shared_ptr<world::Company>> companies;
        std::shared_ptr<Company> player;
        TimeState timeState;
        std::shared_ptr<GameMap> gameMap;
        std::vector<std::shared_ptr<world::City>> cities;
    };
}
#endif // GAMESTATE_H
