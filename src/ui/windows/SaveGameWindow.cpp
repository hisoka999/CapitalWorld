#include "SaveGameWindow.h"
#include "ui/SaveGameComponent.h"
#include <engine/utils/os.h>
#include <filesystem>
#include <ctime>

namespace UI
{
    template <typename TP>
    std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
        return system_clock::to_time_t(sctp);
    }
    SaveGameWindow::SaveGameWindow(bool load) : UI::Window(200, 200, 430, 450), load(load), previewSurface(nullptr)
    {
        if (load)
        {
            setTitle("Load Games");
        }
        else
        {
            setTitle("Save Games");

            saveButton = std::make_shared<UI::Button>(this);
            saveButton->setPos(5, 370);
            saveButton->setFont("fonts/arial.ttf", 12);
            saveButton->setLabel("Save");
            saveButton->connect(UI::Button::buttonClickCallback(), [&]()
                                {
                                    std::string saveGameFolder = utils::os::get_pref_dir("", "capitalworld") + "saves/";
                                    std::filesystem::path p(saveGameFolder);
                                    p /= "New Savegame.save";

                                    std::filesystem::directory_entry dir_entry(p);
                                    auto saveGame = std::make_shared<UI::SaveGameComponent>(saveGameArea.get(), load, dir_entry);
                                    saveGame->setX(0);
                                    int yPos = saveGameArea->size() * saveGame->getHeight();
                                    saveGame->setY(yPos);
                                    saveGame->connect("saveAction", [&](std::string fileName)
                                                      { this->fireFuncionCall("saveAction", fileName); });
                                    saveGameArea->addObject(saveGame); });
            addObject(saveButton);
        }
        setFont("fonts/arial.ttf", 12);
        saveGameArea = std::make_shared<UI::ScrollArea>(400, 350, this);
        addObject(saveGameArea);
    }

    void SaveGameWindow::updateSaveGames()
    {
        std::string saveGameFolder = utils::os::get_pref_dir("capitalworld", "") + "/saves/";
        std::filesystem::path p(saveGameFolder);

        if (!std::filesystem::exists(p))
        {
            std::filesystem::create_directory(p);
        }

        std::vector<std::filesystem::directory_entry> sortbyTime;

        //--- sort the files in the map by size
        for (auto &entry : std::filesystem::directory_iterator(saveGameFolder))
            if (entry.is_regular_file())
            {
                sortbyTime.push_back(entry);
            }
        std::sort(sortbyTime.begin(), sortbyTime.end(), [](std::filesystem::directory_entry &entry, std::filesystem::directory_entry &other)
                  { return entry.last_write_time() > other.last_write_time(); });

        int yPos = 0;
        for (auto const &dir_entry : sortbyTime)
        {
            auto extension = dir_entry.path().extension();
            if (extension.string() != ".save")
            {
                continue;
            }
            auto saveGame = std::make_shared<UI::SaveGameComponent>(saveGameArea.get(), load, dir_entry);
            saveGame->setX(0);
            saveGame->setY(yPos);
            saveGame->connect("saveAction", [&](std::string fileName)
                              { this->fireFuncionCall("saveAction", fileName); });
            yPos += saveGame->getHeight();
            saveGameArea->addObject(saveGame);
        }
        saveGameArea->reset(false);
    }

    SaveGameWindow::~SaveGameWindow()
    {
    }

    void SaveGameWindow::setPreview(SDL_Surface *previewSurface)
    {
        this->previewSurface = previewSurface;
    }
}