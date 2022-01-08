#ifndef __HOUSECOMPONENT_H__
#define __HOUSECOMPONENT_H__

#include "BuildingComponent.h"
#include "world/buildingtypes.h"
namespace world
{
    namespace buildings
    {
        typedef std::map<world::ProductType, float> DemandMap;
        struct Demand
        {
            size_t maxDemand;
            size_t monthsFullFilled;
            size_t value;
        };

        class HouseComponent : public world::buildings::BuildingComponent
        {

        public:
            HouseComponent();
            HouseComponent(unsigned int residents, DemandMap baseDemand, int houseId);
            ~HouseComponent();
            unsigned int getResidents();

            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            virtual void updateProduction(int month, int year, Building *building);
            void updateTexture(Building *building);
            virtual std::shared_ptr<BuildingComponent> clone();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            int getCurrentDemand(world::ProductType type);
            int fullfillDemand(world::ProductType type, int value);
            virtual std::map<std::string, std::string> displayData();

        private:
            void initDemand();
            unsigned int residents;
            const unsigned int maxResidents = 3000;
            DemandMap baseDemand;
            int houseId;
            std::map<world::ProductType, Demand> demand;
        };

    } // namespace buildings

} // namespace world

#endif // __HOUSECOMPONENT_H__