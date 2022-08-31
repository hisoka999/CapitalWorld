#include "optionswindow.h"
#include <engine/core/SceneManager.h>
#include <engine/ui/Button.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/json/parser.h>
#include <engine/utils/os.h>
#include <filesystem>
#include <fstream>
#include "translate.h"
namespace UI
{
    OptionsWindow::OptionsWindow(int x, int y, std::shared_ptr<utils::IniBase> settings) : UI::Window(x, y, 250, 300), saveWindow(false), loadWindow(true), previewSurface(nullptr), settingsWindow(settings)
    {

        setTitle(_("Options"));

        auto layout = std::make_shared<UI::layout::GridLayout>(this, 1);
        layout->setPadding(utils::Vector2(20, 10));
        setLayout(layout);
        auto loadButton = std::make_shared<UI::Button>(this);
        loadButton->setFont("fonts/arial.ttf", 12);
        loadButton->setLabel(_("Load Game"));
        loadButton->connect(UI::Button::buttonClickCallback(), [&]()
                            { loadGame(); });
        addObject(loadButton);

        auto saveButton = std::make_shared<UI::Button>(this);
        saveButton->setFont("fonts/arial.ttf", 12);
        saveButton->setLabel(_("Save Game"));
        saveButton->connect(UI::Button::buttonClickCallback(), [&]()
                            { saveGame(); });
        addObject(saveButton);

        auto optionsButton = std::make_shared<UI::Button>(this);
        optionsButton->setFont("fonts/arial.ttf", 12);
        optionsButton->setLabel(_("Options"));
        addObject(optionsButton);
        optionsButton->connect(UI::Button::buttonClickCallback(), [&]
                               {
                                   settingsWindow.setPos(300, 300);
                                   settingsWindow.setVisible(true); });

        auto mainMenuButton = std::make_shared<UI::Button>(this);
        mainMenuButton->setFont("fonts/arial.ttf", 12);
        mainMenuButton->setLabel(_("Back to main"));
        addObject(mainMenuButton);
        mainMenuButton->connect(UI::Button::buttonClickCallback(), []
                                { core::SceneManager::Instance().setCurrentScene("main"); });

        loadWindow.connect("saveAction", [&](std::string fileName)
                           {
                               std::string saveGameFile = utils::os::get_pref_dir("", "captialworld") + "/saves/" + fileName + ".save";
                               std::ifstream file;
                               std::istringstream is;
                               std::string s;
                               std::string group;

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
                               this->fireFuncionCall("stateChanged", gameState); });

        saveWindow.connect("saveAction", [&](std::string fileName)
                           {
                               setlocale(LC_ALL, "C");
                               std::string saveGameFile = utils::os::get_pref_dir("", "captialworld") + "/saves/" + fileName + ".save";
                               APP_LOG_INFO("savegame: "+ saveGameFile);
                               std::ofstream ostream(saveGameFile, std::ios::trunc | std::ios::out);

                               ostream << gameState->toJsonString();
                               ostream.flush();
                               ostream.close();

                               std::filesystem::path extention(".png");
                               std::filesystem::path previewPath(saveGameFile);
                               previewPath.replace_extension(extention);
                               APP_LOG_INFO("preview path: " + previewPath.string());
                               IMG_SavePNG(previewSurface, previewPath.string().c_str()); });

        needsRefresh();
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

    bool OptionsWindow::handleEvents(core::Input *pInput)
    {
        bool eventsHandled = UI::Window::handleEvents(pInput);
        if (!eventsHandled)
            eventsHandled = settingsWindow.handleEvents(pInput);
        if (!eventsHandled)
            eventsHandled = saveWindow.handleEvents(pInput);
        if (!eventsHandled)
            eventsHandled = loadWindow.handleEvents((pInput));
        return eventsHandled;
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