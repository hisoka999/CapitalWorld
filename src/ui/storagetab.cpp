#include "storagetab.h"
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/utils/os.h>
#include "../services/productservice.h"
#include "../messages.h"

namespace UI
{
    StorageTab::StorageTab(UI::Object *parent, std::shared_ptr<world::Building> building)
        : UI::Tab(parent, "Storage"), building(building)
    {
        cellTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        initUI();

        auto &sys = core::MessageSystem<MessageTypes>::get();
        msgRef = sys.registerForType(MessageTypes::NewMonth, [&]() {
            refreshUI = true;
        });
    }

    StorageTab::~StorageTab()
    {
        if (msgRef != -1)
        {
            auto &sys = core::MessageSystem<MessageTypes>::get();
            sys.deregister(msgRef);
        }
    }

    void StorageTab::setBuilding(std::shared_ptr<world::Building> building)
    {
        this->building = building;
    }

    void StorageTab::render(core::Renderer *render)
    {
        if (refreshUI)
        {
            initUI();
            refreshUI = false;
        }
        UI::Tab::render(render);
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
        for (auto productName : building->getStorage().getStoredProducts())
        {
            auto amount = std::make_shared<UI::Label>(this);
            amount->setFont("fonts/arial.ttf", 12);
            amount->setTextF("%d", building->getStorage().getEntry(productName));
            amount->setPos(x * 55, (y * 55) + 40);
            auto icon = std::make_shared<UI::ImageButton>(this, 50, 50, 0, 0, true);
            auto product = services::ProductService::Instance().getProductByName(productName);
            icon->loadImage(utils::os::combine("images", "products", product->getImage()));
            icon->setPos(x * 55, y * 55);

            x++;
            if (x == 4)
            {
                y++;
                x = 0;
            }

            addObject(icon);
            addObject(amount);
        }
    }
}