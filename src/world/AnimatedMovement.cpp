#include "AnimatedMovement.h"
#include <cassert>

namespace world
{
    AnimatedMovement::AnimatedMovement(const std::list<utils::Vector2> &path,
                                       std::shared_ptr<world::graphics::Sprite> sprite)
        : m_sprite(sprite)

    {
        m_path = std::vector<utils::Vector2>(std::begin(path), std::end(path));
        assert(m_path.size() == path.size());
        m_progress = 0;
        m_nextTargetIndex = 1;
        if (m_path.size() > 0)
        {
            m_currentPosition = m_path.front();
        }
    }

    AnimatedMovement::~AnimatedMovement()
    {
    }

    void AnimatedMovement::setFinishCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

    void AnimatedMovement::render(core::Renderer *pRenderer)
    {
        m_sprite->render(pRenderer);
    }

    void AnimatedMovement::update(double deltaTime, double speed)
    {
        if (isFinished())
            return;
        // assert(m_nextTargetIndex != 0 and "target needs to be positive");
        m_sprite->update(deltaTime);
        m_progress += 0.05 * deltaTime / speed;
        auto &current = m_path[m_nextTargetIndex - 1];
        auto &next = m_path[m_nextTargetIndex];
        auto pos = utils::lerp(current, next, m_progress);
        // calculate direction
        world::graphics::SpriteDirection direction = world::graphics::SpriteDirection::TopLeft;
        if (next.getY() > current.getY())
        {
            direction = world::graphics::SpriteDirection::LeftDown;
        }
        else if (next.getY() < current.getY())
        {
            direction = world::graphics::SpriteDirection::TopRight;
        }
        else if (next.getX() > current.getX())
        {
            direction = world::graphics::SpriteDirection::RightDown;
        }
        else
        {
            direction = world::graphics::SpriteDirection::TopLeft;
        }
        m_sprite->setDirection(direction);
        m_sprite->setCurrentPosition(pos);
        if (m_progress >= 1)
        {
            m_progress = 0;
            m_nextTargetIndex++;
        }
        m_finished = m_nextTargetIndex > m_path.size();
        if (m_finished && m_callback != nullptr)
        {
            m_callback();
        }
    }

    bool AnimatedMovement::isFinished()
    {
        return m_finished;
    }

    const std::shared_ptr<world::graphics::Sprite> &AnimatedMovement::getSprite()
    {
        return m_sprite;
    }
}