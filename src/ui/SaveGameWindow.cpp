#include "SaveGameWindow.h"
#include "SaveGameComponent.h"
#include <engine/utils/os.h>
#include <filesystem>

namespace UI
{
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
                                    std::string saveGameFolder = utils::os::get_pref_dir("captialworld", "") + "saves/";
                                    std::filesystem::path p(saveGameFolder);
                                    p /= "New Savegame.save";
                                    std::filesystem::directory_entry dir_entry(p);
                                    auto saveGame = std::make_shared<UI::SaveGameComponent>(saveGameArea.get(), load, dir_entry);
                                    saveGame->setX(0);
                                    int yPos = saveGameArea->size() * saveGame->getHeight();
                                    saveGame->setY(yPos);
                                    saveGame->connect("saveAction", [&](std::string fileName)
                                                      { this->fireFuncionCall("saveAction", fileName); });
                                    saveGameArea->addObject(saveGame);
                                });
            addObject(saveButton);
        }
        setFont("fonts/arial.ttf", 12);
        saveGameArea = std::make_shared<UI::ScrollArea>(400, 350, this);
        addObject(saveGameArea);
    }

    void SaveGameWindow::updateSaveGames()
    {
        std::string saveGameFolder = utils::os::get_pref_dir("captialworld", "") + "/saves/";
        std::filesystem::path p(saveGameFolder);
        if (!std::filesystem::exists(p))
        {
            std::filesystem::create_directory(p);
        }
        int yPos = 0;
        for (auto const &dir_entry : std::filesystem::directory_iterator{saveGameFolder})
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