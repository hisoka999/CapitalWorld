#pragma once

#include <string>

namespace notifications
{

    enum class EventType
    {
        Research,
        Balance,
        Loan
    };

    struct Event
    {
        EventType type;
        std::string title;
        std::string description;
    };

    std::string eventType2Icon(EventType type);

} // namespace notifications
