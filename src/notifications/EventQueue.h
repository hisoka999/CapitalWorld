#pragma once

#include "Event.h"
#include <engine/core/renderer.h>
#include <engine/graphics/text.h>
#include <list>

namespace notifications
{

    struct EventQueueEntry
    {
        Event event;
        uint32_t elepsedTime = 0;
    };

    class EventQueue
    {
    public:
        EventQueue(/* args */);
        ~EventQueue();
        void add(Event &event);

        void updateEvents(uint32_t delta);

        void render(core::Renderer *renderer);

    private:
        std::list<EventQueueEntry> m_entries;

        std::shared_ptr<graphics::Text> iconFont;
        std::shared_ptr<graphics::Text> textFont;
    };

} // namespace notifcations
