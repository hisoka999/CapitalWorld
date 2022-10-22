#include "Sprite.h"
#include <engine/graphics/rect.h>
#include "world/iso.h"
namespace world
{
    namespace graphics
    {
        Sprite::Sprite(const std::shared_ptr<::graphics::Texture> &texture, SpriteDirection startDirection, size_t numberOfSprites, size_t currentSprite)
            : m_texture(texture), m_currentDirection(startDirection), m_numberOfSprites(numberOfSprites), m_currentSprite(currentSprite)
        {

            m_width = m_texture->getWidth() / (int(SpriteDirection::Down) + 1);
            m_height = m_texture->getHeight() / m_numberOfSprites;
        }

        void Sprite::setCurrentPosition(utils::Vector2 &position)
        {
            m_currentPosition = position;
        }

        void Sprite::setDirection(SpriteDirection direction)
        {
            m_currentDirection = direction;
        }

        void Sprite::update([[maybe_unused]] double deltaTime)
        {
        }

        void Sprite::render(core::Renderer *renderer)
        {
            float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

            const utils::Vector2 pt = getCalulatedPos(renderer);

            ::graphics::Rect src = {m_width * int(m_currentDirection), m_height * m_currentSprite, m_width, m_height};
            ::graphics::Rect dst = {pt.getX(), pt.getY(), m_width * factor, m_height * factor};

            m_texture->render(renderer, src, dst);
        }

        bool Sprite::isMoving()
        {
            return m_moving;
        }

        float Sprite::getWidth()
        {
            return m_width;
        }

        float Sprite::getHeight()
        {
            return m_height;
        }

        utils::Vector2 Sprite::getCalulatedPos(core::Renderer *renderer)
        {
            float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

            float y = static_cast<float>(m_currentPosition.getY()) * 32.f;
            float x = static_cast<float>(m_currentPosition.getX()) * 64.f / 2.0f;
            utils::Vector2 vec(x, y);
            auto isoPos = iso::twoDToIso(vec);
            const utils::Vector2 pt((isoPos.getX() * factor) - renderer->getMainCamera()->getX(), (isoPos.getY() * factor) - renderer->getMainCamera()->getY());
            return pt;
        }
    }
}