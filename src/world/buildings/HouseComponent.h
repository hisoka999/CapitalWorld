#ifndef __HOUSECOMPONENT_H__
#define __HOUSECOMPONENT_H__

#include "BuildingComponent.h"
namespace world
{
    namespace buildings
    {
        class HouseComponent : public world::buildings::BuildingComponent
        {

        public:
            HouseComponent();
            HouseComponent(unsigned int residents);
            ~HouseComponent();
            unsigned int getResidents();

            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            virtual void updateProduction(int month, int year, Building *building);
            virtual std::shared_ptr<BuildingComponent> clone();
            virtual std::shared_ptr<utils::JSON::Object> toJson();

        private:
            unsigned int residents;
        };

    } // namespace buildings

} // namespace world

#endif // __HOUSECOMPONENT_H__