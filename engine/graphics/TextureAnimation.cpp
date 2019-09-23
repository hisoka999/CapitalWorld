/*
 * TextureAnimation.cpp
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#include <engine/graphics/TextureAnimation.h>

namespace graphics {

TextureAnimation::TextureAnimation(utils::Vector2 startPosition) :
        Animation<graphics::Texture>(startPosition) {
    // TODO Auto-generated constructor stub

}

TextureAnimation::~TextureAnimation() {
    // TODO Auto-generated destructor stub
}

} /* namespace graphics */

void graphics::TextureAnimation::renderFrame(
        AnimationFrame<graphics::Texture>& frame, core::Renderer* renderer) {

    frame.data->render(renderer, currentPosition.getX(),
            currentPosition.getY());

}
