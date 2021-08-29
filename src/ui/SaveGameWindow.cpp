#include "SaveGameWindow.h"
#include <filesystem>
#include "SaveGameComponent.h"
#include <engine/utils/os.h>

namespace UI
{
    SaveGameWindow::SaveGameWindow(bool load) : UI::Window(200, 200, 430, 450), load(load)
    {
        setTitle("Save Games");
        setFont("fonts/arial.ttf", 12);
        saveGameArea = std::make_shared<UI::ScrollArea>(400, 400, this);
        addObject(saveGameArea);
    }

    void SaveGameWindow::updateSaveGames()
    {
        std::string saveGameFolder = utils::os::get_pref_dir("captialworld", "") + "/saves/";
        int yPos = 0;
        for (auto const &dir_entry : std::filesystem::directory_iterator{saveGameFolder})
        {
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
}