#include "engine/core/renderer.h"
#include <exception>
#include "engine/utils/exceptions.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

namespace core
{

Renderer::Renderer(utils::Logger& pLogger) :
        camera(NULL), logger(pLogger)
{
    delta = 0;
    this->end = 0;
    lastTick = 0;
    ren = nullptr;
    viewPort.x = 0;
    viewPort.y = 0;
    viewPort.width = 0;
    viewPort.height = 0;
    zoomFactor = 1;
}

void Renderer::open(GameWindow *pWin, bool pVSync)
{

    int numdrivers = SDL_GetNumRenderDrivers();
    logger.trace("Renderer::open",
            "Render driver count: " + std::to_string(numdrivers));

    int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
    if (pVSync) {
        renderFlags |= SDL_RENDERER_PRESENTVSYNC;

    }
    int drvId = -1;
    for (int i = 0; i < numdrivers; i++)
    {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo(i, &drinfo);
        logger.trace("Renderer::open",
                "Driver name (" + std::to_string(i) + "): " + drinfo.name);
        if (drinfo.flags & SDL_RENDERER_SOFTWARE)
            logger.trace("Renderer::open",
                    " the renderer is a software fallback");
        if (drinfo.flags & SDL_RENDERER_ACCELERATED)
            logger.trace("Renderer::open",
                    " the renderer uses hardware acceleration");
        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC)
            logger.trace("Renderer::open",
                    " present is synchronized	with the refresh rate");
        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
            logger.trace("Renderer::open",
                    " the renderer supports rendering to	texture");

        if (drinfo.flags & renderFlags) {
            drvId = i;
            break;
        }
    }
    logger.trace("Renderer::open", "use driver id = " + std::to_string(drvId));



    ren = SDL_CreateRenderer(pWin->getSDLWindow(), drvId,
            renderFlags);

    if (ren == nullptr)
    {
        logger.logSDLError("Renderer::open");
        throw SDLException("Renderer::open");
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
        logger.logSDLError("Renderer::SDL_HINT_RENDER_VSYNC");
    }
    // set viewport
    graphics::Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = pWin->getWidth();
    rect.height = pWin->getHeight();
    setViewPort(rect);

}
Renderer::~Renderer()
{
    if (ren != nullptr)
        SDL_DestroyRenderer (ren);
}

SDL_Renderer *Renderer::getRenderer()
{
    return ren;
}
void Renderer::clear()
{
    SDL_RenderClear (ren);
}

void Renderer::renderPresent()
{
    SDL_RenderPresent (ren);
}

void Renderer::calcDelta()
{
    // calc time delta
    Uint32 end = getTickCount();
    delta = end - lastTick;
//	if (delta == 0)
//		logger.error("calcDelta", "time delta is null");
//	else
        lastTick = end;

}

Uint32 Renderer::getTickCount()
{
    return SDL_GetTicks();
}
void Renderer::setRenderTarget(SDL_Texture* pTexture)
{
    if (SDL_SetRenderTarget(ren, pTexture) != 0)
    {
        logger.logSDLError("Renderer::setRenderTarget");
        throw SDLException("SDL_SetRenderTarget");
    }
}

double Renderer::getTimeDelta()
{
    return delta;
}

graphics::Rect Renderer::getViewPort()
{
    if (viewPort.width == 0)
    {
        SDL_Rect *rect;
        SDL_RenderGetViewport(ren, rect);
        viewPort.width = rect->w;
        viewPort.height = rect->h;
        viewPort.x = rect->x;
        viewPort.y = rect->y;
    }
    return viewPort;
}
void Renderer::setViewPort(graphics::Rect &rect)
{
    viewPort = rect;
    SDL_Rect r = viewPort.sdlRect();
    if (SDL_RenderSetViewport(ren, &r) != 0)
    {
        logger.logSDLError("Renderer::setViewPort");
        throw SDLException("SDL_RenderSetViewport");
    }
}

void Renderer::setMainCamera(Camera* pCamera)
{
    camera = pCamera;
}
Camera* Renderer::getMainCamera()
{
    return camera;
}

void Renderer::drawCircle(int x, int y, int rad, SDL_Color color)
{
    int result = circleRGBA(ren, x, y, rad, color.r, color.g, color.b, color.a);
    if (result == -1)
    {
        logger.logSDLError("Renderer::drawCircle");
        throw SDLException("drawCircle");
    }
}
void Renderer::setDrawColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(ren, r, g, b, a);
}
void Renderer::drawRect(graphics::Rect &rect)
{
    SDL_Rect r = rect.sdlRect();
    int result = SDL_RenderDrawRect(ren, &r);
    if (result != 0)
    {
        logger.logSDLError("Renderer::drawRect");
        throw SDLException("drawRect");
    }

}

void Renderer::fillRect(graphics::Rect &rect)
{
    SDL_Rect r = rect.sdlRect();
    int result = SDL_RenderFillRect(ren, &r);
    if (result != 0)
    {
        logger.logSDLError("Renderer::fillRect");
        throw SDLException("fillRect");
    }
}
double Renderer::getZoomFactor() {
    return zoomFactor;
}
void Renderer::setZoomFactor(double factor) {
    zoomFactor = factor;
}
}
// namespace core
