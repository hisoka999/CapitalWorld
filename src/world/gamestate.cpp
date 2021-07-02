#include "gamestate.h"

namespace world
{
    GameState::GameState(const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty)
        : timeState(TimeState::Normal), player(player), gameMap(gameMap), cities(cities), difficulty(difficulty)
    {
        std::tm ttm = std::tm();
        //Unix Time starts at the year 1900
        ttm.tm_year = 10;
        ttm.tm_mon = 0;
        ttm.tm_mday = 1;

        std::time_t ttime = std::mktime(&ttm);
        time = std::chrono::system_clock::from_time_t(ttime);
        companies.push_back(player);
    }
    void GameState::setTimeState(TimeState state)
    {
        timeState = state;
    }
    void GameState::increaseTime()
    {
        time = time + std::chrono::hours(24);
    }

    std::chrono::system_clock::time_point GameState::getTime()
    {
        return time;
    }

    const std::shared_ptr<world::Company> &GameState::getPlayer() const
    {
        return player;
    }

    const std::shared_ptr<GameMap> &GameState::getGameMap() const
    {
        return gameMap;
    }

    const std::vector<std::shared_ptr<world::City>> &GameState::getCities() const
    {
        return cities;
    }

    const Difficulty GameState::getDifficulty() const
    {
        return difficulty;
    }

}
