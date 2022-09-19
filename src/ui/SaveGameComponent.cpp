#include "SaveGameComponent.h"
#include "translate.h"
#include <chrono>

namespace UI
{

    template <typename TP>
    std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
        return system_clock::to_time_t(sctp);
    }
    std::string serializeTimePoint(const std::filesystem::file_time_type &time, const std::string &format)
    {

        std::time_t tt = to_time_t(time);
        std::tm tm = *std::gmtime(&tt); // GMT (UTC)
        // std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
        std::stringstream ss;
        ss << std::put_time(&tm, format.c_str());
        return ss.str();
    }

    void SaveGameComponent::initUI()
    {
        actionButton = std::make_shared<UI::Button>(this);
        deleteButton = std::make_shared<UI::Button>(this);
        previewImageButton = std::make_shared<UI::ImageButton>(this, 100, 100, 0, 0, true);
        fileDateLabel = std::make_shared<UI::Label>(this);
        nameTextItem = std::make_shared<UI::TextItem>(this, 150, 20);
        auto nameLabel = std::make_shared<UI::Label>(this);

        previewImageButton->setPos(5, 5);
        nameLabel->setPos(110, 5);
        nameTextItem->setPos(150, 5);
        fileDateLabel->setPos(110, 30);
        actionButton->setPos(110, 50);
        deleteButton->setPos(220, 50);

        actionButton->connect(UI::Button::buttonClickCallback(), [&]()
                              { this->fireFuncionCall("saveAction", nameTextItem->getText()); });

        addObject(nameLabel);
        addObject(nameTextItem);
        addObject(actionButton);
        addObject(deleteButton);
        addObject(previewImageButton);
        addObject(fileDateLabel);
        deleteButton->setFont("fonts/arial.ttf", 12);
        nameLabel->setFont("fonts/arial.ttf", 12);
        actionButton->setFont("fonts/arial.ttf", 12);
        fileDateLabel->setFont("fonts/arial.ttf", 12);
        deleteButton->setLabel(_("Delete"));
        nameLabel->setText(_("Name"));
    }

    SaveGameComponent::SaveGameComponent(UI::Object *parent, bool loadGame, const std::filesystem::directory_entry &entry)
        : UI::Container(), UI::Object(parent)
    {
        initUI();

        setWidth(300);
        setHeight(110);

        if (loadGame)
        {
            actionButton->setLabel(_("Load Game"));
        }
        else
        {
            actionButton->setLabel(_("Save Game"));
        }

        if (entry.exists())
        {
            std::filesystem::path extention(".png");
            auto previewPath = entry.path();
            previewPath.replace_extension(extention);
            auto fileName = entry.path().stem().string();
            auto tp = entry.last_write_time();
            fileDateLabel->setText(serializeTimePoint(tp, "%d.%m.%Y %H:%M:%S"));
            nameTextItem->setText(fileName);
            if (std::filesystem::exists(previewPath))
                previewImageButton->loadImage(previewPath.string());
        }
    }

    SaveGameComponent::~SaveGameComponent()
    {
    }

    void SaveGameComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    bool SaveGameComponent::handleEvents(core::Input *pInput)
    {
        return UI::Container::handleEvents(pInput);
    }
}