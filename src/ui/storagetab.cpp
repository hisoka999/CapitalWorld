#include "storagetab.h"
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
namespace UI
{
    StorageTab::StorageTab(UI::Object *parent, std::shared_ptr<world::Building> building)
        : UI::Tab(parent, "Storage"), building(building)
    {
        cellTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        initUI();
    }

    void StorageTab::setBuilding(std::shared_ptr<world::Building> building)
    {
        this->building = building;
    }

    void StorageTab::initUI()
    {
        this->clear();
        for (size_t y = 0; y < 4; ++y)
        {
            for (size_t x = 0; x < 4; ++x)
            {
                auto cell = std::make_shared<UI::ImageButton>(this, 53, 53, 0, 0, false);
                cell->setImage(cellTexture);
                cell->setPos(x * 55, y * 55);
                addObject(cell);
            }
        }

        int x = 0;
        int y = 0;
        for (auto product : building->getStorage().getStoredProducts())
        {
            auto amount = std::make_shared<UI::Label>(this);
            amount->setFont("fonts/arial.ttf", 12);
            amount->setTextF("%d", building->getStorage().getEntry(product));
            amount->setPos(x * 55, (y * 55) + 40);
            x++;
            if (x == 4)
            {
                y++;
                x = 0;
            }
            addObject(amount);
        }
    }
}