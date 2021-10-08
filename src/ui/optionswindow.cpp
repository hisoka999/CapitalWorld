#include "optionswindow.h"
#include <engine/core/SceneManager.h>
#include <engine/ui/Button.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/json/parser.h>
#include <engine/utils/os.h>
#include <filesystem>
#include <fstream>

namespace UI
{
    OptionsWindow::OptionsWindow(int x, int y) : UI::Window(x, y, 200, 300), saveWindow(false), loadWindow(true), previewSurface(nullptr)
    {

        setTitle("Options");
        mainArea = std::make_shared<UI::ScrollArea>(150, 250, this);
        mainArea->setPos(5, 5);
        addObject(mainArea);

        auto layout = std::make_shared<UI::layout::GridLayout>(mainArea.get(), 1);
        layout->setPadding(utils::Vector2(20, 10));

        auto loadButton = std::make_shared<UI::Button>(mainArea.get());
        loadButton->setFont("fonts/arial.ttf", 12);
        loadButton->setLabel("Load Game");
        loadButton->connect(UI::Button::buttonClickCallback(), [&]()
                            { loadGame(); });
        mainArea->addObject(loadButton);

        auto saveButton = std::make_shared<UI::Button>(mainArea.get());
        saveButton->setFont("fonts/arial.ttf", 12);
        saveButton->setLabel("Save Game");
        saveButton->connect(UI::Button::buttonClickCallback(), [&]()
                            { saveGame(); });
        mainArea->addObject(saveButton);

        auto optionsButton = std::make_shared<UI::Button>(mainArea.get());
        optionsButton->setFont("fonts/arial.ttf", 12);
        optionsButton->setLabel("Options");
        mainArea->addObject(optionsButton);
        optionsButton->connect(UI::Button::buttonClickCallback(), [&]
                               {
                                   settingsWindow.setPos(300, 300);
                                   settingsWindow.setVisible(true);
                               });

        auto mainMenuButton = std::make_shared<UI::Button>(mainArea.get());
        mainMenuButton->setFont("fonts/arial.ttf", 12);
        mainMenuButton->setLabel("Back to main");
        mainArea->addObject(mainMenuButton);
        mainMenuButton->connect(UI::Button::buttonClickCallback(), []
                                { core::SceneManager::Instance().setCurrentScene("main"); });

        graphics::Rect bounds = {10, 10, float(mainArea->getWidth()), float(mainArea->getHeight())};

        layout->updateLayout(bounds);
        mainArea->reset();

        loadWindow.connect("saveAction", [&](std::string fileName)
                           {
                               std::string saveGameFile = utils::os::get_pref_dir("", "captialworld") + "/saves/" + fileName + ".save";
                               std::ifstream file;
                               std::istringstream is;
                               std::string s;
                               std::string group;
                               //  std::cout << filename << std::endl;

                               file.open(saveGameFile.c_str(), std::ios::in);
                               if (!file.is_open())
                               {
                                   throw IOException(saveGameFile, "file does not exists");
                               }
                               std::string buffer((std::istreambuf_iterator<char>(file)),
                                                  std::istreambuf_iterator<char>());
                               utils::JSON::Parser parser;
                               auto jsonObject = parser.parseObject(buffer);

                               file.close();

                               gameState = world::GameState::fromJson(jsonObject);
                               this->fireFuncionCall("stateChanged", gameState);
                           });

        saveWindow.connect("saveAction", [&](std::string fileName)
                           {
                               setlocale(LC_ALL, "C");
                               std::string saveGameFile = utils::os::get_pref_dir("", "captialworld") + "/saves/" + fileName + ".save";
                               std::cout << "savegame: " << saveGameFile << std::endl;
                               std::ofstream ostream(saveGameFile, std::ios::trunc | std::ios::out);

                               ostream << gameState->toJsonString();
                               ostream.flush();
                               ostream.close();

                               std::filesystem::path extention(".png");
                               std::filesystem::path previewPath(saveGameFile);
                               previewPath.replace_extension(extention);
                               std::cout << "preview path: " << previewPath << std::endl;
                               IMG_SavePNG(previewSurface, previewPath.c_str());
                           });
    }

    OptionsWindow::~OptionsWindow()
    {
    }

    void OptionsWindow::render(core::Renderer *pRender)
    {
        UI::Window::render(pRender);
        settingsWindow.render(pRender);
        saveWindow.render(pRender);
        loadWindow.render(pRender);
    }

    void OptionsWindow::handleEvents(core::Input *pInput)
    {
        UI::Window::handleEvents(pInput);
        settingsWindow.handleEvents(pInput);
        saveWindow.handleEvents(pInput);
        loadWindow.handleEvents((pInput));
    }

    void OptionsWindow::setGameState(const std::shared_ptr<world::GameState> &gameState)
    {
        this->gameState = gameState;
    }

    const std::shared_ptr<world::GameState> &OptionsWindow::getGameState()
    {
        return gameState;
    }

    void OptionsWindow::setPreview(SDL_Surface *previewSurface)
    {
        this->previewSurface = previewSurface;
    }

    void OptionsWindow::saveGame()
    {

        saveWindow.updateSaveGames();
        saveWindow.setVisible(true);
    }

    void OptionsWindow::loadGame()
    {

        loadWindow.updateSaveGames();
        loadWindow.setVisible(true);
    }

}