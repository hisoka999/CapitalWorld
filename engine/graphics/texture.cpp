#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include <iostream>

namespace graphics
{

Texture::Texture() :
        width(0), height(0)
{
    tex = nullptr;
}

Texture::Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight)
{
    tex = SDL_CreateTexture(pRenderer->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, pWidth, pHeight);
    width = pWidth;
    height = pHeight;
    if (tex == NULL)
    {
        throw SDLException("SDL_CreateTexture");
    }

}

void Texture::loadTexture(core::Renderer *ren, std::string filename)
{
    tex = IMG_LoadTexture(ren->getRenderer(), filename.c_str());

    if (tex == nullptr)
    {

        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError()
                << std::endl;
        throw SDLException("IMG_LoadTexture");
    }
    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);

}
void Texture::render(core::Renderer *ren, int x, int y)
{
    render(ren, x, y, width, height, 0, 0);
}

void Texture::renderResized(core::Renderer *ren, int x, int y, int pWidth,
        int pHeight)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = pWidth;
    dst.h = pHeight;

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;
    SDL_RenderCopy(ren->getRenderer(), tex, &src, &dst);
}

void Texture::render(core::Renderer *ren, Rect pSrc, Rect pDest)
{
    SDL_Rect dst = pDest.sdlRect();
    SDL_Rect src = pSrc.sdlRect();
    SDL_RenderCopy(ren->getRenderer(), tex, &src, &dst);
}

void Texture::render(core::Renderer *ren, int x, int y, int pWidth, int pHeight,
        int pSrcX, int pSrcY)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = pWidth;
    dst.h = pHeight;

    SDL_Rect src;
    src.x = pSrcX;
    src.y = pSrcY;
    src.w = pWidth;
    src.h = pHeight;

    SDL_RenderCopy(ren->getRenderer(), tex, &src, &dst);
}

void Texture::renderRotated(core::Renderer *ren, const double angle,
        const int x, const int y, const int pWidth, const int pHeight)
{
    SDL_Point* center = NULL;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = pWidth;
    dst.h = pHeight;

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = width;
    src.h = height;

    SDL_RenderCopyEx(ren->getRenderer(), tex, &src, &dst, angle, center,
            SDL_FLIP_NONE);
}

int Texture::getWidth()
{
    return width;
}
int Texture::getHeight()
{
    return height;
}
Texture::~Texture()
{
    SDL_DestroyTexture (tex);
}

void Texture::setColorKey(int r, int g, int b)
{
    SDL_SetTextureColorMod(tex, r, g, b);
}

} // namespace graphics
