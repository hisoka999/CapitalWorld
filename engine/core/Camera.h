/*
 * Camera.h
 *
 *  Created on: 20.12.2015
 *      Author: stefan
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <engine/graphics/rect.h>

namespace core
{

class Camera
{
public:
    Camera(graphics::Rect &viewPort);
    virtual ~Camera();

    void move(float pX, float pY);
    void reset();

    float getX();
    float getY();
    float getWidth();
    float getHeight();

    graphics::Rect getViewPortRect();

private:
    graphics::Rect viewPort;
};

} /* namespace core */

#endif /* CAMERA_H_ */
