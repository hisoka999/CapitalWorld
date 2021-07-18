#ifndef __BUILDINGCOMPONENT_H__
#define __BUILDINGCOMPONENT_H__

#include <engine/utils/json/object.h>

namespace world
{
    class Company;

    namespace buildings
    {
        class BuildingComponent
        {
        private:
            std::string name;

        public:
            BuildingComponent(const std::string &name);
            ~BuildingComponent();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            std::string &getName();
            virtual void updateProduction(int month, int year);
            virtual std::shared_ptr<BuildingComponent> clone();
        };

    } // namespace buildings

} // namespace world

#endif // __BUILDINGCOMPONENT_H__