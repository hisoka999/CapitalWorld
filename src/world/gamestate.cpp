#include "gamestate.h"
#include <engine/utils/json/object.h>
#include <magic_enum.hpp>
#include <world/buildings/SalesComponent.h>
#include <future>
#include <iostream>
namespace world
{

    GameState::GameState(const std::vector<std::shared_ptr<world::Company>> &companies, const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty)
        : timeState(TimeState::Normal), player(player), gameMap(gameMap), cities(cities), difficulty(difficulty), time(1900, 0, 1), companies(companies)
    {

        this->companies.push_back(player);
    }

    GameState::GameState(const std::vector<std::shared_ptr<world::Company>> &companies, const std::shared_ptr<Company> &player, const std::shared_ptr<GameMap> &gameMap, const std::vector<std::shared_ptr<world::City>> &cities, const Difficulty difficulty, utils::time::Date &time)
        : timeState(TimeState::Normal), player(player), gameMap(gameMap), cities(cities), difficulty(difficulty), time(time), companies(companies)
    {
        this->companies.push_back(player);
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

    const std::vector<std::shared_ptr<world::City>> &GameState::getCities()
    {
        return cities;
    }

    const std::vector<std::shared_ptr<world::Company>> &GameState::getCompanies()
    {
        return companies;
    }

    Difficulty GameState::getDifficulty() const
    {
        return difficulty;
    }

    void GameState::update()
    {
        // auto start = std::chrono::high_resolution_clock::now();

        // auto elapsed = std::chrono::high_resolution_clock::now() - start;
        // long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        // std::cout << "update companies time: " << milliseconds << "ms" << std::endl;
        // start = std::chrono::high_resolution_clock::now();
        // update cities
        for (auto &city : cities)
        {

            for (auto &building : city->getBuildings())
            {
                building->updateProduction(time.getMonth(), time.getYear());
            }
        }

        for (auto &company : companies)
        {
            company->updateBalance(time.getMonth(), time.getYear());
        }

        // elapsed = std::chrono::high_resolution_clock::now() - start;
        // milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        // std::cout << "update cities time: " << milliseconds << "ms" << std::endl;
    }

    std::shared_ptr<GameState> GameState::fromJson(std::shared_ptr<utils::JSON::Object> &object)
    {
        std::shared_ptr<Company> player = Company::fromJson(object->getObjectValue("player"));

        std::shared_ptr<GameMap> gameMap = GameMap::fromJson(object->getObjectValue("gameMap"));
        std::vector<std::shared_ptr<world::City>> cities;

        utils::JSON::JsonArray jsonCities = object->getArray("cities");

        for (auto c : jsonCities)
        {
            auto cityObject = std::get<std::shared_ptr<utils::JSON::Object>>(c);

            auto city = City::fromJson(cityObject);
            cities.push_back(city);

            for (auto &street : city->getStreets())
            {
                gameMap->addBuilding(street);
            }
            for (auto &street : city->getBuildings())
            {
                gameMap->addBuilding(street);
            }
        }

        utils::JSON::JsonArray jsonCompanies = object->getArray("companies");
        std::vector<std::shared_ptr<Company>> companies;

        for (auto &jsonCompany : jsonCompanies)
        {
            auto company = Company::fromJson(std::get<std::shared_ptr<utils::JSON::Object>>(jsonCompany));

            for (auto &street : company->getBuildings())
            {
                if (street->hasComponent("SalesComponent"))
                {
                    auto component = street->getComponent<world::buildings::SalesComponent>("SalesComponent");
                    component->setGameMap(gameMap.get());
                }
                gameMap->addBuilding(street);
            }
            companies.push_back(company);
        }

        for (auto &street : player->getBuildings())
        {
            if (street->hasComponent("SalesComponent"))
            {
                auto component = street->getComponent<world::buildings::SalesComponent>("SalesComponent");
                component->setGameMap(gameMap.get());
            }
            gameMap->addBuilding(street);
        }

        Difficulty difficulty = magic_enum::enum_cast<Difficulty>(object->getStringValue("difficulty")).value();
        int year = object->getIntValue("time_year");
        int month = object->getIntValue("time_month");
        int day = object->getIntValue("time_day");
        utils::time::Date time(year, month, day);
        return std::make_shared<GameState>(companies, player, gameMap, cities, difficulty, time);
    }

    const std::shared_ptr<Company> GameState::findBuildingOwner(const std::shared_ptr<world::Building> &building)
    {
        for (auto company : getCompanies())
        {
            if (company->hasBuilding(building))
            {
                return company;
            }
        }
        return nullptr;
    }

    std::string GameState::toJsonString()
    {
        std::shared_ptr<utils::JSON::Object> jsonGameState = std::make_shared<utils::JSON::Object>();

        jsonGameState->setAttribute("player", player->toJson());
        jsonGameState->setAttribute("time_year", time.getYear());
        jsonGameState->setAttribute("time_month", time.getMonth());
        jsonGameState->setAttribute("time_day", time.getDay());
        jsonGameState->setAttribute("gameMap", gameMap->toJson());
        jsonGameState->setAttribute("difficulty", std::string(magic_enum::enum_name(difficulty)));

        utils::JSON::JsonArray jsonCities;
        for (auto city : cities)
        {
            utils::JSON::JsonValue jsonCity = city->toJson();
            jsonCities.push_back(jsonCity);
        }
        jsonGameState->setArrayAttribute("cities", jsonCities);

        utils::JSON::JsonArray jsonCompanies;

        for (auto &company : companies)
        {
            if (!company->isPLayer())
            {
                utils::JSON::JsonValue jsonCompany = company->toJson();
                jsonCompanies.push_back(jsonCompany);
            }
        }
        jsonGameState->setArrayAttribute("companies", jsonCompanies);

        return jsonGameState->toJsonString();
    }

}
