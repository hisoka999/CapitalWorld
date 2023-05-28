#ifndef __WORKERCOMPONENT_H__
#define __WORKERCOMPONENT_H__

#include "BuildingComponent.h"
#include "world/buildingtypes.h"
namespace world
{
    namespace buildings
    {

        class WorkerComponent : public world::buildings::BuildingComponent
        {

        public:
            WorkerComponent();
            ~WorkerComponent();

            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            virtual void updateProduction(int month, int year, Building *building);
            virtual std::shared_ptr<BuildingComponent> clone();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual std::map<std::string, std::string> displayData();
            virtual void setMetaData(std::shared_ptr<utils::JSON::Object> jsonData);
            int getCurrentWorkers();
            int getMaxWorkers();
            int getSalary();
            void setCurrentWorkers(int workers);

        private:
            int currentWorkers = 0;
            int maxWorkers = 0;
            int salary = 0;
        };

    } // namespace buildings

} // namespace world
#endif // __WORKERCOMPONENT_H__