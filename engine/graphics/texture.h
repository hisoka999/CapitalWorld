#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "engine/core/renderer.h"
#include "engine/graphics/rect.h"

namespace graphics {

class Texture
{
    public:
        /** Default constructor */
        Texture();
        Texture(core::Renderer *pRenderer,const int pWidth,const int pHeight);
        /** Default destructor */
        virtual ~Texture();
        void loadTexture(core::Renderer *ren, std::string filename);
        void render(core::Renderer *ren,int x,int y);
        void render(core::Renderer *ren,int x,int y,int pWidth,int pHeight,int pSrcX,int pSrcY);
        void render(core::Renderer *ren,Rect pSrc,Rect pDest);
        void renderResized(core::Renderer *ren,int x, int y,int pWidth,int pHeight);
        void renderRotated(core::Renderer *ren,const double angle,const int x,const int y,const int pWidth,const int pHeight);
        void setColorKey(uint8_t r,uint8_t g,uint8_t b);
        void setBlendMode(SDL_BlendMode blendMode);
        void setAlphaColor(core::Renderer* ren,uint8_t r,uint8_t g,uint8_t b);
        int getWidth();
        int getHeight();
        SDL_Texture* getSDLTexture(){
            return tex;
        }
    protected:
    private:
        SDL_Texture *tex;
        SDL_Surface* surface;
        int width;
        int height;
};

} // namespace graphics

#endif // GRAPHICS_TEXTURE_H
