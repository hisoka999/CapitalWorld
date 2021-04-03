/*
 * Label.h
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#ifndef UI_LABEL_H_
#define UI_LABEL_H_

#include <engine/ui/Object.h>
#include <engine/utils/string.h>
namespace UI
{

    class Label : public Object
    {
    public:
        Label(const std::string &text, Object *parent = nullptr);
        Label(Object *parent);
        virtual ~Label();
        virtual void render(core::Renderer *pRender);

        std::string getText();
        void setText(const std::string &text);
        template <typename... Args>
        void setTextF(const std::string &format, Args const &...args)
        {
            setText(utils::string_format(format, args...));
        }

        void setColor(SDL_Color color);

    private:
        std::string text;
        SDL_Color color;
    };

} /* namespace UI */

#endif /* UI_LABEL_H_ */
