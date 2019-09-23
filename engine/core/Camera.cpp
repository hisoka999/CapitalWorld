/*
 * Camera.cpp
 *
 *  Created on: 20.12.2015
 *      Author: stefan
 */

#include <engine/core/Camera.h>

namespace core
{

Camera::Camera(graphics::Rect &viewPort)
{
        this->viewPort = viewPort;
}

Camera::~Camera()
{
}
void Camera::reset() {
        viewPort.x = 0;
        viewPort.y = 0;
}
void Camera::move(float pX, float pY)
{
        viewPort.x += pX;
        viewPort.y += pY;
}

float Camera::getX()
{
        return viewPort.x;
}
float Camera::getY()
{
        return viewPort.y;
}
float Camera::getWidth()
{
        return viewPort.width;
}
float Camera::getHeight()
{
        return viewPort.height;
}
graphics::Rect Camera::getViewPortRect()
{
        return viewPort;

}
} /* namespace core */
