#pragma once
#include "BuildingComponent.h"

namespace world
{
    namespace buildings
    {

        class ProductionComponent : public world::buildings::BuildingComponent
        {

        public:
            ProductionComponent();
            ~ProductionComponent();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company) override;
            virtual void updateProduction(int month, int year, Building *building) override;
            virtual std::shared_ptr<BuildingComponent> clone() override;
            virtual std::shared_ptr<utils::JSON::Object> toJson() override;
            virtual std::map<std::string, std::string> displayData() override;
        };

    } // namespace buildings

} // namespace world
