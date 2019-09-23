#ifndef UI_LISTBOX_H
#define UI_LISTBOX_H

#include <engine/ui/Object.h>
#include <vector>
#include <string>
#include "engine/utils/vector2.h"
#include "engine/core/renderer.h"
#include "engine/core/input.h"
#include "engine/graphics/text.h"
#include "engine/graphics/rect.h"

namespace UI
{

typedef std::vector<int> ListBoxSelections;

struct ListBoxItem
{
    std::string text;
    bool selected;
};

class ListBox : public UI::Object
{
public:
    /** Default constructor */
    ListBox(UI::Object parent = 0);
    /** Default destructor */
    virtual ~ListBox();
    void handleEvents(core::Input *pInput);
    void render(core::Renderer *pRender,graphics::Texture *pTexture);
    ListBoxSelections getSelections();
    void addItem(std::string item);
    void removeItem(int pos);
protected:
private:
    std::vector<ListBoxItem> items;
    graphics::Text *font;

};

} // namespace UI

#endif // UI_LISTBOX_H
