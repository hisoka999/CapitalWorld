#ifndef __SALESTAB_H__
#define __SALESTAB_H__

#include <engine/ui/Tab.h>
#include "world/building.h"
#include <engine/ui/scrollarea.h>

namespace UI
{
    class SalesTab : public UI::Tab
    {

    public:
        SalesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building);
        ~SalesTab();
        void setBuilding(const std::shared_ptr<world::Building> &building);

    protected:
        void refresh();

    private:
        std::shared_ptr<world::Building> building;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        void refreshUI();
    };

} // namespace UI

#endif // __SALESTAB_H__