#ifndef __WORKERTAB_H__
#define __WORKERTAB_H__
#include <engine/ui/Tab.h>
#include "world/building.h"
namespace UI
{
    class WorkerTab : public UI::Tab
    {
    private:
        std::shared_ptr<world::Building> building;
        void initUI();

    public:
        WorkerTab(UI::Object *parent, const std::shared_ptr<world::Building> &building);
        ~WorkerTab();
        void setBuilding(const std::shared_ptr<world::Building> &building);
    };

} // namespace UI

#endif // __WORKERTAB_H__