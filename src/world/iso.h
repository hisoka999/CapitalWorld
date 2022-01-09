#ifndef __ISO_H__
#define __ISO_H__
#include <engine/utils/vector2.h>

namespace iso
{
    const utils::Vector2 isoTo2D(const utils::Vector2 &pt);
    const utils::Vector2 twoDToIso(const utils::Vector2 &pt);
    const utils::Vector2 twoDToIso(int x, int y);
} // namespace iso

#endif // __ISO_H__