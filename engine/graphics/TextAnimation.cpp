/*
 * TextAnimation.cpp
 *
 *  Created on: 09.09.2019
 *      Author: stefan
 */

#include <engine/graphics/TextAnimation.h>

namespace graphics {

TextAnimation::TextAnimation(utils::Vector2 startPosition, SDL_Color color, std::string text)
    : Animation<graphics::Text>(startPosition)
    , color(color)
    , text(text)
{
    // TODO Auto-generated constructor stub
}
void TextAnimation::renderFrame(AnimationFrame<Text>& frame,
    core::Renderer* renderer)
{
    frame.data->render(renderer, text, color, currentPosition.getX(), currentPosition.getY());
}
TextAnimation::~TextAnimation()
{
    // TODO Auto-generated destructor stub
}

} /* namespace graphics */
