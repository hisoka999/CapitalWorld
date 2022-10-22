#ifndef __BUILDINGCOMPONENT_H__
#define __BUILDINGCOMPONENT_H__

#include <engine/utils/json/object.h>

namespace world
{
    class Company;
    class Building;
    class GameState;

    namespace buildings
    {

        class BuildingComponent
        {
        private:
            std::string name;

        public:
            BuildingComponent(const std::string &name);
            virtual void setMetaData(std::shared_ptr<utils::JSON::Object> jsonData);
            ~BuildingComponent();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            std::string &getName();
            virtual void updateProduction(int month, int year, Building *building);
            virtual void updateDaily(uint16_t day, uint16_t month, uint16_t year, Building *building, Company *company);
            virtual std::shared_ptr<BuildingComponent> clone();

            virtual void delayedUpdate(Company *company);
            virtual std::map<std::string, std::string> displayData();

        protected:
            world::GameState *getGameState();
            bool isGameRunning();
        };

    } // namespace buildings

} // namespace world

#endif // __BUILDINGCOMPONENT_H__