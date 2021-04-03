#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include <iostream>

namespace graphics
{

    Texture::Texture()
        : width(0), height(0)
    {
        tex = nullptr;
        surface = nullptr;
    }

    Texture::Texture(core::Renderer *pRenderer, const int pWidth, const int pHeight)
    {
        tex = SDL_CreateTexture(pRenderer->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, pWidth, pHeight);
        width = pWidth;
        height = pHeight;
        surface = nullptr;
        if (tex == nullptr)
        {
            throw SDLException("SDL_CreateTexture");
        }
    }

    void Texture::loadTexture(core::Renderer *ren, std::string filename)
    {
        //tex = IMG_LoadTexture(ren->getRenderer(), filename.c_str());
        surface = IMG_Load(filename.c_str());

        if (surface == nullptr)
        {

            std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError()
                      << std::endl;
            throw SDLException("IMG_LoadTexture");
        }
        tex = SDL_CreateTextureFromSurface(ren->getRenderer(), surface);
        //Query the texture to get its width and height to use
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
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
        SDL_Point *center = nullptr;

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

        int result = SDL_RenderCopyEx(ren->getRenderer(), tex, &src, &dst, angle, center,
                                      SDL_FLIP_NONE);
        if (result != 0)
            throw SDLException("renderRotated::SDL_RenderCopyEx");
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
        SDL_DestroyTexture(tex);
        if (surface != nullptr)
            SDL_FreeSurface(surface);
        surface = nullptr;
    }

    void Texture::setColorKey(uint8_t r, uint8_t g, uint8_t b)
    {
        SDL_SetTextureColorMod(tex, r, g, b);
    }
    void Texture::setBlendMode(SDL_BlendMode blendMode)
    {
        SDL_SetTextureBlendMode(tex, blendMode);
    }
    void Texture::setAlphaColor(core::Renderer *ren, uint8_t r, uint8_t g, uint8_t b)
    {
        if (tex != nullptr)
        {
            SDL_DestroyTexture(tex);
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));

        tex = SDL_CreateTextureFromSurface(ren->getRenderer(), surface);
    }

} // namespace graphics
