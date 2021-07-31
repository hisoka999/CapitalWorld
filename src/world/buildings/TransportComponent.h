#ifndef __TRANSPORTCOMPONENT_H__
#define __TRANSPORTCOMPONENT_H__

#include "BuildingComponent.h"

class Product;

namespace world
{
    class Building;

    namespace buildings
    {

        struct TransportRoute
        {
            std::shared_ptr<world::Building> startBuilding;
            std::shared_ptr<world::Building> endBuilding;
            std::string startBuildingName;
            std::string endBuildingName;
            std::shared_ptr<Product> product;
            unsigned quantity;
            bool active;
        };

        class TransportComponent : public world::buildings::BuildingComponent
        {
        private:
            std::vector<std::shared_ptr<TransportRoute>> routes;

        public:
            TransportComponent();
            ~TransportComponent();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);

            const std::vector<std::shared_ptr<TransportRoute>> &getAllRoutes();
            std::vector<std::shared_ptr<TransportRoute>> getActiveRoutes();
            void addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity);
            void removeRoute(const size_t position);
            void updateProduction(int month, int year, Building *building);
            virtual std::shared_ptr<BuildingComponent> clone();
            virtual void delayedUpdate(Company *company);
        };

    } // namespace buildings

} // namespace world
#endif // __TRANSPORTCOMPONENT_H__