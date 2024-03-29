#include "BuildingComponent.h"
#include <engine/core/SceneManager.h>
#include "scenes/worldscene.h"

namespace world
{
    namespace buildings
    {
        BuildingComponent::BuildingComponent(const std::string &name) : name(name)
        {
        }

        void BuildingComponent::setMetaData([[maybe_unused]] std::shared_ptr<utils::JSON::Object> jsonData)
        {
        }

        BuildingComponent::~BuildingComponent()
        {
        }

        std::shared_ptr<utils::JSON::Object> BuildingComponent::toJson()
        {
            auto object = std::make_shared<utils::JSON::Object>();
            object->setAttribute("name", name);

            return object;
        }

        void BuildingComponent::fromJson([[maybe_unused]] std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
        }

        std::string &BuildingComponent::getName()
        {
            return name;
        }

        void BuildingComponent::updateProduction([[maybe_unused]] int month, [[maybe_unused]] int year, [[maybe_unused]] Building *building)
        {
        }

        void BuildingComponent::updateDaily([[maybe_unused]] uint16_t day, [[maybe_unused]] uint16_t month, [[maybe_unused]] uint16_t year, [[maybe_unused]] Building *building, [[maybe_unused]] Company *company)
        {
        }

        std::shared_ptr<BuildingComponent> BuildingComponent::clone()
        {
            return std::make_shared<BuildingComponent>(this->getName());
        }

        void BuildingComponent::delayedUpdate([[maybe_unused]] Company *company)
        {
        }

        std::map<std::string, std::string> BuildingComponent::displayData()
        {
            std::map<std::string, std::string> result;
            return result;
        }

        world::GameState *BuildingComponent::getGameState()
        {
            auto worldScene = dynamic_cast<scenes::WorldScene *>(core::SceneManager::Instance().getCurrentScene());

            return worldScene->getGameState().get();
        }

        bool BuildingComponent::isGameRunning()
        {
            auto worldScene = dynamic_cast<scenes::WorldScene *>(core::SceneManager::Instance().getCurrentScene());
            return worldScene != nullptr && worldScene->getGameState() != nullptr;
        }

    }
}