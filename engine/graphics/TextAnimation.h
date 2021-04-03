/*
 * TextAnimation.h
 *
 *  Created on: 09.09.2019
 *      Author: stefan
 */

#ifndef GRAPHICS_TEXTANIMATION_H_
#define GRAPHICS_TEXTANIMATION_H_

#include <engine/graphics/Animation.h>
#include <engine/graphics/text.h>

namespace graphics {

class TextAnimation : public Animation<Text> {
public:
    TextAnimation(utils::Vector2 startPosition, SDL_Color color, std::string text);
    virtual ~TextAnimation();

protected:
    virtual void renderFrame(AnimationFrame<Text>& frame,
        core::Renderer* renderer);

private:
    SDL_Color color;
    std::string text;
};

} /* namespace graphics */

#endif /* GRAPHICS_TEXTANIMATION_H_ */
