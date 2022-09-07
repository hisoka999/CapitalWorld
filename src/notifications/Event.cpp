#include "Event.h"

namespace notifications
{
    std::string eventType2Icon(EventType type)
    {
        switch (type)
        {
        case EventType::Research:
            return "\uf0c3";
        case EventType::Loan:
            return "\uf53a";
        case EventType::Balance:
            return "\ue097";
        default:
            break;
        }
        return "X";
    }
}