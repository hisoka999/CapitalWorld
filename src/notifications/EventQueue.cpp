#include "EventQueue.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <engine/utils/logger.h>

namespace notifications
{
    EventQueue::EventQueue(/* args */)
    {
        iconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        textFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 12);
    }

    EventQueue::~EventQueue()
    {
    }

    void EventQueue::add(Event &event)
    {
        EventQueueEntry entry = {event, 0};
        m_entries.push_back(entry);
    }

    void EventQueue::updateEvents(uint32_t delta)
    {
        for (auto it = m_entries.begin(); it != m_entries.end(); it++)
        {
            it->elepsedTime += delta;

            if (it->elepsedTime > 5000)
            {
                it = m_entries.erase(it);
            }
        }
    }

    void EventQueue::render(core::Renderer *renderer)
    {
        float yOffset = 80;
        float iconBlockSize = 40;
        float textBlockWidth = 200;
        for (auto &event : m_entries)
        {
            renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
            int alpha = (5000.0 - event.elepsedTime) / 5000.0 * 255.0;

            renderer->setDrawColor(0, 200, 200, alpha);

            graphics::Rect iconRect = {renderer->getMainCamera()->getWidth() - textBlockWidth - iconBlockSize - 220, yOffset, iconBlockSize, iconBlockSize};

            graphics::Rect textBlockRect = {renderer->getMainCamera()->getWidth() - textBlockWidth - iconBlockSize - 220, yOffset, textBlockWidth + iconBlockSize, iconBlockSize};
            renderer->drawRect(textBlockRect);
            textBlockRect.x += iconBlockSize;
            textBlockRect.width -= iconBlockSize;
            renderer->fillRect(iconRect);

            renderer->setDrawColor(0x22, 0x22, 0x22, alpha);
            renderer->fillRect(textBlockRect);
            SDL_Color textColor = {255, 255, 255, 0};

            iconFont->render(renderer, eventType2Icon(event.event.type), textColor, iconRect.x + 10, iconRect.y + 10);

            textFont->setStyle(graphics::FontStyle::BOLD);
            textFont->render(renderer, event.event.title, textColor, textBlockRect.x + 5, textBlockRect.y + 5);
            textFont->setStyle(graphics::FontStyle::NORMAL);
            textFont->renderWrapped(renderer, event.event.description, textColor, textBlockRect.x + 5, textBlockRect.y + 20, textBlockWidth);

            yOffset += iconBlockSize + 5;

            renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
        }
    }
}