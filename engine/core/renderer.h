#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "engine/core/gamewindow.h"
#include "engine/core/Camera.h"
#include "engine/graphics/rect.h"
#include "engine/utils/logger.h"
#include <SDL2/SDL_render.h>

namespace core
{

class Renderer
{
public:
    /** Default constructor */
    Renderer(utils::Logger& pLogger);
    /** Default destructor */
    virtual ~Renderer();

    void open(GameWindow *pWin, bool pVSync);

    SDL_Renderer *getRenderer();
    void clear();
    void renderPresent();
    Uint32 getTickCount();
    double getTimeDelta();
    void setRenderTarget(SDL_Texture* pTexture);
    graphics::Rect getViewPort();
    void setViewPort(graphics::Rect &rect);
    void setMainCamera(Camera* pCamera);
    Camera* getMainCamera();
    void calcDelta();
    double getZoomFactor();
    void setZoomFactor(double factor);

    //drawing routines
    /**
     * draws a not filled circle on the renderer
     * @param
     * @param color of the ellipse
     * @throws
     */
    void drawCircle(int x, int y, int rad, SDL_Color color);

    void drawRect(graphics::Rect &rect);

    void fillRect(graphics::Rect &rect);

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

protected:
private:
    utils::Logger logger;
    SDL_Renderer *ren;
    Uint32 lastTick;
    Uint32 end;
    double delta;
    graphics::Rect viewPort;
    Camera *camera;
    double zoomFactor;
};

} // namespace core

#endif // CORE_RENDERER_H
