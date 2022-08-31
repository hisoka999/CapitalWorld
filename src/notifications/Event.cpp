#include "Event.h"

namespace notifications
{
    std::string eventType2Icon(EventType type)
    {
        switch (type)
        {
        case EventType::Research:
            return "\uf0c3";

        default:
            break;
        }
        return "X";
    }
}