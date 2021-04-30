#ifndef STORAGETAB_H
#define STORAGETAB_H

#include <engine/ui/Tab.h>
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/ComboBox.h>
#include "../services/productservice.h"
#include "../world/building.h"

namespace UI
{

    class StorageTab : public UI::Tab
    {
    public:
        StorageTab(UI::Object *parent, std::shared_ptr<world::Building> building);
        virtual ~StorageTab();

        void setBuilding(std::shared_ptr<world::Building> building);
        virtual void render(core::Renderer *render);

    private:
        void initUI();
        std::shared_ptr<world::Building> building;
        std::shared_ptr<graphics::Texture> cellTexture;
        int msgRef = -1;
        bool refreshUI = false;
    };
}

#endif // STORAGETAB_H
