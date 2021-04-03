#pragma once

#include "engine/ui/Container.h"

namespace UI
{
    namespace layout
    {
        class Layout
        {
        public:
            Layout(UI::Container *container);
            virtual void updateLayout(const graphics::Rect &bounds) = 0;

        protected:
            UI::Container *container;
        };
    }; // namespace layout
};     // namespace UI