#include "gamestate.h"
#include <engine/utils/json/object.h>

namespace world
{

    GameState::GameState(const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty)
        : timeState(TimeState::Normal), player(player), gameMap(gameMap), cities(cities), difficulty(difficulty), time(1900, 0, 1)
    {

        companies.push_back(player);
    }
    void GameState::setTimeState(TimeState state)
    {
        timeState = state;
    }
    void GameState::increaseTime()
    {
        time.addDay(1);
    }

    utils::time::Date &GameState::getTime()
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

    std::string GameState::toJsonString()
    {
        std::shared_ptr<utils::JSON::Object> jsonGameState = std::make_shared<utils::JSON::Object>();

        jsonGameState->setAttribute("player", player->toJson());

        return jsonGameState->toJsonString();
    }

}
