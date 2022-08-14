#include "storagetab.h"
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/utils/os.h>
#include "../services/productservice.h"
#include "../messages.h"
#include "world/buildings/StorageComponent.h"
#include "engine/ui/StringHint.h"
#include "translate.h"

namespace UI
{
    StorageTab::StorageTab(UI::Object *parent, std::shared_ptr<world::Building> building)
        : UI::Tab(parent, _("Storage")), building(building)
    {
        cellTexture = graphics::TextureManager::Instance().loadTexture("images/Cell01.png");
        initUI();

        auto &sys = core::MessageSystem<MessageTypes>::get();
        msgRef = sys.registerForType(MessageTypes::NewMonth, [&]()
                                     { needsRefresh(); });
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

    void StorageTab::initUI()
    {
        this->clear();
        int offset = 20;
        int size = 55;
        for (size_t y = 0; y < 4; ++y)
        {
            for (size_t x = 0; x < 4; ++x)
            {
                auto cell = std::make_shared<UI::ImageButton>(this, 53, 53, 0, 0, false);
                cell->setImage(cellTexture);
                cell->setPos(offset + (x * size), offset + (y * size));
                addObject(cell);
                // cell->connect(UI::Button::buttonClickCallback(), [&]()
                //               { itemComponent->setProduct(nullptr); });
            }
        }

        int x = 0;
        int y = 0;
        auto storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");

        for (auto productName : storage->getStoredProducts())
        {
            auto amount = std::make_shared<UI::Label>(this);
            amount->setFont("fonts/arial.ttf", 12);
            int productAmount = storage->getEntry(productName);
            amount->setTextF("%d", productAmount);
            amount->setPos(offset + (x * size), offset + 40 + (y * size));
            auto icon = std::make_shared<UI::ImageButton>(this, 50, 50, 0, 0, true);
            auto product = services::ProductService::Instance().getProductByName(productName);
            icon->loadImage(utils::os::combine("images", "products", product->getImage()));
            icon->setPos(offset + (x * size), offset + (y * size));
            icon->setHint(std::make_shared<UI::StringHint>(product->getLocalisedName()));
            icon->connect(UI::Button::buttonClickCallback(), [=]()
                          { itemComponent->setProduct(product);
                          itemComponent->setAmount(productAmount) ;
                          lastProduct = product; });
            x++;
            if (x == 4)
            {
                y++;
                x = 0;
            }

            addObject(icon);
            addObject(amount);
        }

        itemComponent = std::make_shared<UI::StorageItemComponent>(this);
        itemComponent->setPos(150, offset);
        itemComponent->setProduct(lastProduct);
        if (lastProduct != nullptr)
        {
            int productAmount = storage->getEntry(lastProduct->getName());
            itemComponent->setAmount(productAmount);
        }
        addObject(itemComponent);
    }

    void StorageTab::refresh()
    {
        initUI();
        endRefresh();
    }
}