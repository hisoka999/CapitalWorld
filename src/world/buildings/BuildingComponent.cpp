#include "BuildingComponent.h"

namespace world
{
    namespace buildings
    {
        BuildingComponent::BuildingComponent(const std::string &name) : name(name)
        {
        }

        void BuildingComponent::setMetaData(std::shared_ptr<utils::JSON::Object> jsonData)
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

        void BuildingComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company)
        {
        }

        std::string &BuildingComponent::getName()
        {
            return name;
        }

        void BuildingComponent::updateProduction(int month, int year, Building *building)
        {
        }

        std::shared_ptr<BuildingComponent> BuildingComponent::clone()
        {
            return std::make_shared<BuildingComponent>(this->getName());
        }

        void BuildingComponent::delayedUpdate(Company *company)
        {
        }

        std::map<std::string, std::string> BuildingComponent::displayData()
        {
            std::map<std::string, std::string> result;
            return result;
        }
    }
}