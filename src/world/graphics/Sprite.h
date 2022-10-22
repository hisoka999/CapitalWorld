#pragma once

#include <engine/graphics/texture.h>
#include <engine/utils/vector2.h>

namespace world
{
    namespace graphics
    {
        enum class SpriteDirection
        {
            LeftDown = 0,
            Left = 1,
            TopLeft = 2,
            Top = 3,
            TopRight = 4,
            Right = 5,
            RightDown = 6,
            Down = 7

        };
        class Sprite
        {
        private:
            std::shared_ptr<::graphics::Texture> m_texture;
            SpriteDirection m_currentDirection;
            size_t m_numberOfSprites;
            size_t m_currentSprite;
            utils::Vector2 m_currentPosition;
            bool m_moving = false;
            float m_width;
            float m_height;

        public:
            Sprite(const std::shared_ptr<::graphics::Texture> &texture, SpriteDirection startDirection, size_t numberOfSprites, size_t currentSprite);
            ~Sprite() = default;

            void setCurrentPosition(utils::Vector2 &position);
            void setDirection(SpriteDirection direction);
            void update(double deltaTime);
            void render(core::Renderer *renderer);
            bool isMoving();
            float getWidth();
            float getHeight();
            utils::Vector2 getCalulatedPos(core::Renderer *renderer);
        };

    } // namespace graphics

} // namespace world
