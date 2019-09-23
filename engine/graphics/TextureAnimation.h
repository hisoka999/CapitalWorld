/*
 * TextureAnimation.h
 *
 *  Created on: 15.06.2019
 *      Author: stefan
 */

#ifndef GRAPHICS_TEXTUREANIMATION_H_
#define GRAPHICS_TEXTUREANIMATION_H_

#include <engine/graphics/Animation.h>
#include <engine/graphics/texture.h>

namespace graphics {

class TextureAnimation: public Animation<graphics::Texture> {
public:
    TextureAnimation(utils::Vector2 startPosition);
    virtual ~TextureAnimation();

protected:
    virtual void renderFrame(AnimationFrame<graphics::Texture>& frame,
            core::Renderer* renderer);
};

} /* namespace graphics */

#endif /* GRAPHICS_TEXTUREANIMATION_H_ */
