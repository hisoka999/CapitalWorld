#include "GridLayout.h"
#include <cmath>

namespace UI
{
    namespace layout
    {
        GridLayout::GridLayout(UI::Container *container, unsigned int columns) : Layout(container), columns(2), padding(5, 5)
        {
        }

        void GridLayout::updateLayout(const graphics::Rect &bounds)
        {
            int xOffset = bounds.x;

            int height = 0;
            for (size_t i = 0; i < container->size(); ++i)
            {
                auto item = container->get(i);

                height = std::max(height, item->getHeight());
            }
            for (unsigned int column = 1; column <= columns; column++)
            {
                int width = 0;
                int yOffset = bounds.y;
                for (size_t i = 0; i < container->size(); ++i)
                {
                    if (i % columns == column - 1)
                    {
                        auto item = container->get(i);
                        width = std::max(width, item->getWidth());
                        item->setX(xOffset);
                        item->setY(yOffset);
                        yOffset = item->getY() + height + padding.getY();
                    }
                }
                xOffset += width + padding.getX();
            }
        }

        void GridLayout::setPadding(utils::Vector2 value)
        {
            padding = value;
        }

    } // namespace layout
} // namespace UI
