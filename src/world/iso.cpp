#include "iso.h"

namespace iso
{
    const utils::Vector2 isoTo2D(const utils::Vector2 &pt)
    {
        float x, y = 0.0f;
        x = ((2.0f * pt.getY()) + pt.getX()) / 2.0f;
        y = ((2.0f * pt.getY()) - pt.getX()) / 2.0f;
        return utils::Vector2(x, y);
    }

    const utils::Vector2 twoDToIso(const utils::Vector2 &pt)
    {
        float x, y = 0.0f;
        x = pt.getX() - pt.getY();
        y = (pt.getX() + pt.getY()) / 2.0f;
        return utils::Vector2(x, y);
    }

    const utils::Vector2 twoDToIso(int x, int y)
    {
        return utils::Vector2(x - y, (x + y) / 2.f);
    }
}