#include "engine/graphics/text.h"
#include "engine/utils/logger.h"
#include "engine/utils/exceptions.h"

namespace graphics
{

Text::Text()
{
    //ctor
    font = nullptr;
    //logger = new utils::Logger();
    texture = nullptr;
}

Text::~Text()
{
    if (font != nullptr)
        TTF_CloseFont (font);
}

void Text::openFont(const std::string fontFile, int fontSize)
{
    //Open the font
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == NULL)
    {
        logger.logSDLError("TTF_OpenFont");
        throw new SDLException("TTF_OpenFont");
    }
}

int Text::size(std::string message, int* w, int* h)
{
    if (font != nullptr)
        return TTF_SizeText(font, message.c_str(), w, h);
    else
        return -1;
}

void Text::render(core::Renderer *ren, const std::string message,
        SDL_Color color, int x, int y)
{
    if (message.size() == 0)
        return;
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    int w, h;
    size(message, &w, &h);

    SDL_Surface *surf = TTF_RenderUTF8_Blended_Wrapped(font, message.c_str(),
            color, w);
    if (surf == nullptr)
    {
        logger.logSDLError("TTF_RenderText_Blended");
        TTF_CloseFont (font);
        throw new SDLException("TTF_RenderText_Blended");

    }
    texture = SDL_CreateTextureFromSurface(ren->getRenderer(), surf);
    if (texture == nullptr)
    {
        logger.logSDLError("SDL_CreateTextureFromSurface");
        throw new SDLException("SDL_CreateTextureFromSurface");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    int iW, iH;
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = iW;
    dst.h = iH;

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = iW;
    src.h = iH;

    SDL_RenderCopy(ren->getRenderer(), texture, &src, &dst);
    SDL_DestroyTexture (texture);
}

} // namespace graphics
