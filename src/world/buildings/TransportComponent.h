#ifndef __TRANSPORTCOMPONENT_H__
#define __TRANSPORTCOMPONENT_H__

#include "BuildingComponent.h"

#include <list>
#include <engine/utils/vector2.h>

class Product;

namespace world
{
    class Building;

    namespace buildings
    {
        const size_t MAX_ROUTES = 10;

        struct TransportRoute
        {
            std::shared_ptr<world::Building> startBuilding;
            std::shared_ptr<world::Building> endBuilding;
            std::string startBuildingName;
            std::string endBuildingName;
            std::shared_ptr<Product> product;
            unsigned quantity = 0;
            unsigned maxQuantity = 10000;
            bool active = false;
            bool transportActive = false;
            bool transportFinished = false;
            std::list<utils::Vector2> path;
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
            void clearRoutes();
            void addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity);
            void removeRoute(const size_t position);
            void updateProduction(int month, int year, Building *building) override;
            void updateDaily(uint16_t day, uint16_t month, uint16_t year, Building *building, Company *company) override;
            virtual std::shared_ptr<BuildingComponent> clone();
            virtual void delayedUpdate(Company *company);
        };

    } // namespace buildings

    struct AnimatedMovementData
    {
        std::shared_ptr<buildings::TransportRoute> route;
        std::string colorName;
    };

} // namespace world
#endif // __TRANSPORTCOMPONENT_H__