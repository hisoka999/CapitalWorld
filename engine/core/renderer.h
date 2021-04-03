#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "engine/core/Camera.h"
#include "engine/core/gamewindow.h"
#include "engine/graphics/rect.h"
#include "engine/utils/logger.h"
#include <SDL2/SDL_render.h>

namespace core
{

    class Renderer
    {
    public:
        /** Default constructor */
        Renderer(const utils::Logger &pLogger);
        /** Default destructor */
        virtual ~Renderer();

        void open(const GameWindow *pWin, bool pVSync);

        SDL_Renderer *getRenderer();
        void clear();
        void renderPresent();
        Uint32 getTickCount();
        float getTimeDelta();
        void setRenderTarget(SDL_Texture *pTexture);
        graphics::Rect getViewPort();
        void setViewPort(graphics::Rect &rect);
        void setMainCamera(Camera *pCamera);
        Camera *getMainCamera();
        void calcDelta();
        float getZoomFactor();
        void setZoomFactor(float factor);

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

        void drawLine(utils::Vector2 &start, utils::Vector2 &end);

    protected:
    private:
        utils::Logger logger;
        SDL_Renderer *ren;
        Uint32 lastTick;
        Uint32 end;
        float delta;
        graphics::Rect viewPort;
        Camera *camera;
        float zoomFactor;
    };

} // namespace core

#endif // CORE_RENDERER_H
