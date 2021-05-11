#include "street.h"
#include "../gamemap.h"
#include "../../translate.h"
namespace world
{
    namespace buildings
    {
        Street::Street()
            : Building("Street", _("Street"), "Street", 1000, BuildingType::Street, 1, 1)

        {
        }

        void Street::update(GameMap *gameMap)
        {
            updateWithParent(gameMap, nullptr, 1);
        }

        void Street::updateWithParent(GameMap *gameMap, Street *sourceStreet, const unsigned level)
        {
            bool isBorderingNorth = false;
            bool isBorderingSouth = false;
            bool isBorderingEast = false;
            bool isBorderingWest = false;
            auto pos = get2DPosition();
            auto posNorth = pos;
            posNorth.y -= 1;
            auto posEast = pos;
            posEast.x += 1;
            auto posWest = pos;
            posWest.x -= 1;
            auto posSouth = pos;
            posSouth.y += 1;
            Street *northStreet = nullptr;
            Street *southStreet = nullptr;
            Street *eastStreet = nullptr;
            Street *westStreet = nullptr;

            auto northBuilding = gameMap->getBuilding(pos.x, pos.y - 1);
            auto southBuilding = gameMap->getBuilding(pos.x, pos.y + 1);
            auto eastBuilding = gameMap->getBuilding(pos.x + 1, pos.y);
            auto westBuilding = gameMap->getBuilding(pos.x - 1, pos.y);

            if (northBuilding != nullptr && northBuilding->getType() == BuildingType::Street)
            {
                northStreet = static_cast<Street *>(northBuilding);
                isBorderingNorth = true;
            }

            if (southBuilding != nullptr && southBuilding->getType() == BuildingType::Street)
            {
                southStreet = static_cast<Street *>(southBuilding);
                isBorderingSouth = true;
            }
            if (eastBuilding != nullptr && eastBuilding->getType() == BuildingType::Street)
            {
                eastStreet = static_cast<Street *>(eastBuilding);
                isBorderingEast = true;
            }
            if (westBuilding != nullptr && westBuilding->getType() == BuildingType::Street)
            {
                westStreet = static_cast<Street *>(westBuilding);
                isBorderingWest = true;
            }
            // for (auto &tmp : gameMap->getBuildings())
            // {

            //     if (street->getType() != BuildingType::Street)
            //         continue;

            //     if (street->get2DPosition().intersectsNoLine(posNorth))
            //     {
            //         isBorderingNorth = true;
            //         northStreet = static_cast<Street>(street);
            //     }
            //     if (street->get2DPosition().intersectsNoLine(posSouth))
            //     {
            //         isBorderingSouth = true;
            //         southStreet = static_cast<Street>(street);
            //     }
            //     if (street->get2DPosition().intersectsNoLine(posEast))
            //     {
            //         isBorderingEast = true;
            //         eastStreet = static_cast<Street>(street);
            //     }
            //     if (street->get2DPosition().intersectsNoLine(posWest))
            //     {
            //         westStreet = static_cast<Street>(street);
            //         isBorderingWest = true;
            //     }
            // }
            if (isBorderingEast && isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                setSubTexture("street_cross_center");
            }
            else if (isBorderingEast && isBorderingSouth && !isBorderingNorth && isBorderingWest)
            {
                setSubTexture("street_cross_up_right");
            }
            else if (isBorderingEast && isBorderingSouth && isBorderingNorth && !isBorderingWest)
            {
                setSubTexture("street_cross_up_left");
            }
            else if (isBorderingEast && !isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                setSubTexture("street_cross_down_left");
            }
            else if (!isBorderingEast && isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                setSubTexture("street_cross_down_right");
            }
            else if (isBorderingSouth && isBorderingEast && !isBorderingNorth && !isBorderingWest)
            {
                setSubTexture("street_corner_up");
            }
            else if (isBorderingNorth && !isBorderingEast && isBorderingWest && !isBorderingSouth)
            {
                setSubTexture("street_corner_left");
            }
            else if (!isBorderingNorth && !isBorderingEast && isBorderingWest && isBorderingSouth)
            {
                setSubTexture("street_corner_right");
            }
            else if (isBorderingNorth && isBorderingEast && !isBorderingWest && !isBorderingSouth)
            {
                setSubTexture("street_corner_down");
            }
            else if (isBorderingEast || isBorderingWest)
            {
                setSubTexture("street2");
            }
            else
            {
                setSubTexture("street1");
            }
            if (level == 5)
                return;

            if (northStreet != nullptr)
            {
                if (sourceStreet == nullptr || northStreet->get2DPosition() != sourceStreet->get2DPosition())
                {
                    northStreet->updateWithParent(gameMap, this, level + 1);
                }
            }

            if (southStreet != nullptr)
            {
                if (sourceStreet == nullptr || southStreet->get2DPosition() != sourceStreet->get2DPosition())
                {
                    southStreet->updateWithParent(gameMap, this, level + 1);
                }
            }

            if (eastStreet != nullptr)
            {
                if (sourceStreet == nullptr || eastStreet->get2DPosition() != sourceStreet->get2DPosition())
                {
                    eastStreet->updateWithParent(gameMap, this, level + 1);
                }
            }
            if (westStreet != nullptr)
            {
                if (sourceStreet == nullptr || westStreet->get2DPosition() != sourceStreet->get2DPosition())
                {
                    westStreet->updateWithParent(gameMap, this, level + 1);
                }
            }
        }

    }
}