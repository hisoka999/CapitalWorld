#ifndef GRAPHICS_TEXT_H
#define GRAPHICS_TEXT_H

#include <SDL2/SDL_ttf.h>
#include <string>
#include <engine/core/renderer.h>
#include <engine/utils/logger.h>

namespace graphics
{

class Text
{
public:
    /** Default constructor */
    Text();
    /** Default destructor */
    virtual ~Text();

    void openFont(const std::string fontFile, int fontSize);
    void render(core::Renderer *ren, const std::string message, SDL_Color color,
            int x, int y);
    int size(std::string message, int* w, int* h);
protected:
private:
    TTF_Font *font;
    utils::Logger logger;
    SDL_Texture *texture;
};

} // namespace graphics

#endif // GRAPHICS_TEXT_H
