#pragma once

#include "../building.h"

namespace world
{
    namespace buildings
    {

        class Street : public Building
        {
        public:
            Street();
            virtual ~Street() = default;
            virtual void update(GameMap *gameMap);
            void updateWithParent(GameMap *gameMap, Street *sourceStreet, const unsigned level);
        };

    };
};
