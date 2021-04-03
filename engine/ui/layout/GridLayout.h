#pragma once

#include "Layout.h"

namespace UI
{
    namespace layout
    {
        class GridLayout : public Layout
        {
        public:
            GridLayout(UI::Container *container, unsigned int columns = 2);
            virtual void updateLayout(const graphics::Rect &bounds);

            void setPadding(utils::Vector2 value);

        private:
            unsigned int columns;
            utils::Vector2 padding;
        };
    }; // namespace layout
};     // namespace UI