#include "street.h"
#include "../../translate.h"
#include "../gamemap.h"
namespace world
{
    namespace buildings
    {
        Street::Street()
            : Building("Street", _("Street"), "Street", 1000, BuildingType::Street, 1, 1)

        {
            setSubTexture("street1");
            m_dragBuild = true;
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
            std::shared_ptr<Street> northStreet = nullptr;
            std::shared_ptr<Street> southStreet = nullptr;
            std::shared_ptr<Street> eastStreet = nullptr;
            std::shared_ptr<Street> westStreet = nullptr;

            auto &northBuilding = gameMap->getBuilding(pos.x, pos.y - 1);
            auto &southBuilding = gameMap->getBuilding(pos.x, pos.y + 1);
            auto &eastBuilding = gameMap->getBuilding(pos.x + 1, pos.y);
            auto &westBuilding = gameMap->getBuilding(pos.x - 1, pos.y);

            if (northBuilding != nullptr && northBuilding->getType() == BuildingType::Street)
            {
                northStreet = std::dynamic_pointer_cast<Street>(northBuilding);
                isBorderingNorth = true;
            }

            if (southBuilding != nullptr && southBuilding->getType() == BuildingType::Street)
            {
                southStreet = std::dynamic_pointer_cast<Street>(southBuilding);
                isBorderingSouth = true;
            }
            if (eastBuilding != nullptr && eastBuilding->getType() == BuildingType::Street)
            {
                eastStreet = std::dynamic_pointer_cast<Street>(eastBuilding);
                isBorderingEast = true;
            }
            if (westBuilding != nullptr && westBuilding->getType() == BuildingType::Street)
            {
                westStreet = std::dynamic_pointer_cast<Street>(westBuilding);
                isBorderingWest = true;
            }

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