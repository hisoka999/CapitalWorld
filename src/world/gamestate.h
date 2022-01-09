#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../world/company.h"
#include "constants.h"
#include "world/city.h"
#include "world/gamemap.h"
#include <chrono>
#include <ctime>
#include <engine/utils/json/object.h>
#include <engine/utils/time/date.h>
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
        GameState(const std::vector<std::shared_ptr<world::Company>> &companies, const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty);
        GameState(const std::vector<std::shared_ptr<world::Company>> &companies, const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty, utils::time::Date &time);

        void setTimeState(TimeState state);
        void increaseTime();
        utils::time::Date &getTime();
        const std::shared_ptr<world::Company> &getPlayer() const;
        const std::shared_ptr<GameMap> &getGameMap() const;
        const std::vector<std::shared_ptr<world::City>> &getCities();
        const std::vector<std::shared_ptr<world::Company>> &getCompanies();
        Difficulty getDifficulty() const;
        void update();

        std::string toJsonString();
        static std::shared_ptr<GameState> fromJson(std::shared_ptr<utils::JSON::Object> &object);

    private:
        TimeState timeState;
        std::shared_ptr<Company> player;
        std::shared_ptr<GameMap> gameMap;
        std::vector<std::shared_ptr<world::City>> cities;
        Difficulty difficulty;
        utils::time::Date time;
        std::vector<std::shared_ptr<world::Company>> companies;
    };
}
#endif // GAMESTATE_H
