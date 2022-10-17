#pragma once

#include <list>
#include <vector>
#include <memory>
#include <world/graphics/Sprite.h>
#include <engine/utils/vector2.h>
#include <functional>

namespace world
{

    class AnimatedMovement
    {
    private:
        std::vector<utils::Vector2> m_path;
        std::shared_ptr<world::graphics::Sprite> m_sprite;
        utils::Vector2 m_currentPosition;
        size_t m_nextTargetIndex;
        float m_progress;
        bool m_finished = false;
        std::function<void()> m_callback = nullptr;

    public:
        AnimatedMovement(const std::list<utils::Vector2> &path,
                         std::shared_ptr<world::graphics::Sprite> sprite);
        ~AnimatedMovement();
        AnimatedMovement &operator=(const AnimatedMovement &other) = default;
        void setFinishCallback(std::function<void()> callback);
        void render(core::Renderer *pRenderer);
        void update(double deltaTime, double speed);
        bool isFinished();
        const std::shared_ptr<world::graphics::Sprite> &getSprite();
    };

} // namespace world
