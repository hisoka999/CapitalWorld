#include "NewGameScene.h"
#include "../translate.h"
#include "scenes/worldscene.h"
#include "services/researchservice.h"
#include "world/mapgenerator.h"
#include "world/CompanyNameGenerator.h"
#include <engine/core/SceneManager.h>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/ComboBox.h>
#include <engine/ui/TextItem.h>
#include <magic_enum.hpp>
#include <string>

#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/color.h>
#include <random>

namespace scenes
{
    NewGameScene::NewGameScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager) : core::Scene(pRenderer), sceneManager(pSceneManager)
    {

        const auto &viewPort = pRenderer->getViewPort();
        container = std::make_shared<UI::Container>();
        buttonList = std::make_shared<UI::Container>();
        auto layout = std::make_shared<UI::layout::GridLayout>(container.get(), 2);
        layout->setPadding(utils::Vector2(10, 10));

        auto startButton = std::make_shared<UI::Button>();
        startButton->setPos(viewPort.width - 150, viewPort.height - 50);
        startButton->setFont("fonts/arial.ttf", 14);
        startButton->setLabel(_("Start Game"));

        startButton->connect(UI::Button::buttonClickCallback(), [&]()
                             { startGame(); });
        buttonList->addObject(startButton);

        auto backButton = std::make_shared<UI::Button>();
        backButton->setPos(viewPort.width - 250, viewPort.height - 50);
        backButton->setFont("fonts/arial.ttf", 14);
        backButton->setLabel(_("Back"));
        backButton->connect(UI::Button::buttonClickCallback(), [&]()
                            { sceneManager->setCurrentScene("main"); });

        buttonList->addObject(backButton);
        winMgr->addContainer(buttonList.get());

        // add comboboxes and textfields
        int yOffset = 30;
        int y = 300;

        auto nameLabel = std::make_shared<UI::Label>(nullptr);
        nameLabel->setFont("fonts/arial.ttf", 14);
        nameLabel->setText(_("Companyname: "));
        nameLabel->setPos(20, y);
        container->addObject(nameLabel);

        auto nameEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        nameEdit->setPos(200, y);
        nameEdit->setFont("fonts/arial.ttf", 14);
        playerName = "Player";
        nameEdit->setText(playerName);
        nameEdit->connect("textChanged", [&](std::string data)
                          { playerName = data; });
        container->addObject(nameEdit);

        auto colorLabel = std::make_shared<UI::Label>(nullptr);
        colorLabel->setFont("fonts/arial.ttf", 14);
        colorLabel->setText(_("Company color: "));
        colorLabel->setPos(20, y);
        container->addObject(colorLabel);

        auto colorCombobox = std::make_shared<UI::ComboBox<world::CompanyColor>>();
        colorCombobox->setPos(200, y);
        colorCombobox->setFont("fonts/arial.ttf", 14);
        color = world::CompanyColor::black;
        colorCombobox->setSelectionByText(world::CompanyColor::black);

        constexpr auto &colors = magic_enum::enum_values<world::CompanyColor>();

        for (auto &value : colors)
        {
            colorCombobox->addElement(value);
        }
        colorCombobox->connect("valueChanged", [&](world::CompanyColor data)
                               { color = data; });

        colorCombobox->setElementFunction([](world::CompanyColor val) -> std::string
                                          { return _(std::string(magic_enum::enum_name(val))); });

        container->addObject(nameEdit);

        y += yOffset;
        auto seedLabel = std::make_shared<UI::Label>(nullptr);
        seedLabel->setFont("fonts/arial.ttf", 14);
        seedLabel->setText(_("Seed: "));
        seedLabel->setPos(20, y);
        container->addObject(seedLabel);

        seedEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        seedEdit->setPos(200, y);
        seedEdit->setFont("fonts/arial.ttf", 14);
        seed = std::chrono::system_clock::now().time_since_epoch().count();

        seedEdit->setText(std::to_string(seed));
        seedEdit->connect("textChanged", [&](std::string data)
                          { seed = std::stoul(data); });

        container->addObject(seedEdit);

        y += yOffset;
        auto numberOfPlayersLabel = std::make_shared<UI::Label>(nullptr);
        numberOfPlayersLabel->setFont("fonts/arial.ttf", 14);
        numberOfPlayersLabel->setText(_("number of companies: "));
        numberOfPlayersLabel->setPos(20, y);
        container->addObject(numberOfPlayersLabel);

        auto numberOfPlayersCombobox = std::make_shared<UI::ComboBox<int>>();
        numberOfPlayersCombobox->setFont("fonts/arial.ttf", 14);
        numberOfPlayersCombobox->addElement(0);
        numberOfPlayersCombobox->addElement(2);
        numberOfPlayersCombobox->addElement(4);
        numberOfPlayersCombobox->addElement(8);
        // numberOfPlayersCombobox->addElement(16);
        // numberOfPlayersCombobox->addElement(32);
        numberOfPlayersCombobox->setPos(200, y);
        numberOfPlayersCombobox->setSelectionByText(8);
        numberOfPlayersCombobox->setWidth(200);
        numberOfPlayersCombobox->setElementFunction([](int val)
                                                    { return std::to_string(val); });

        numberOfPlayersCombobox->connect("valueChanged", [&](int size)
                                         { numberOfCompanys = size; });
        container->addObject(numberOfPlayersCombobox);
        numberOfCompanys = 8;
        // numberOfCities
        numberOfCities = 8;
        auto numberOfCitiesLabel = std::make_shared<UI::Label>(nullptr);
        numberOfCitiesLabel->setFont("fonts/arial.ttf", 14);
        numberOfCitiesLabel->setText(_("number of cities: "));
        numberOfCitiesLabel->setPos(20, y);
        container->addObject(numberOfCitiesLabel);

        auto numberOfCitiesCombobox = std::make_shared<UI::ComboBox<int>>();
        numberOfCitiesCombobox->setFont("fonts/arial.ttf", 14);

        numberOfCitiesCombobox->addElement(4);
        numberOfCitiesCombobox->addElement(8);
        numberOfCitiesCombobox->addElement(16);
        numberOfCitiesCombobox->addElement(32);
        numberOfCitiesCombobox->setPos(200, y);
        numberOfCitiesCombobox->setSelectionByText(numberOfCities);
        numberOfCitiesCombobox->setWidth(200);
        numberOfCitiesCombobox->setElementFunction([](int val)
                                                   { return std::to_string(val); });

        numberOfCitiesCombobox->connect("valueChanged", [&](int size)
                                        { numberOfCities = size; });
        container->addObject(numberOfCitiesCombobox);

        y += yOffset;
        auto mapSizeLabel = std::make_shared<UI::Label>(nullptr);
        mapSizeLabel->setFont("fonts/arial.ttf", 14);
        mapSizeLabel->setText(_("Map size: "));
        mapSizeLabel->setPos(20, y);
        container->addObject(mapSizeLabel);

        auto mapSizeCombobox = std::make_shared<UI::ComboBox<WorldSize>>();
        mapSizeCombobox->setFont("fonts/arial.ttf", 14);
        mapSizeCombobox->connect("valueChanged", [&](WorldSize size)
                                 { worldSize = size; });
        constexpr auto &worldSizes = magic_enum::enum_values<WorldSize>();

        for (auto &value : worldSizes)
        {
            mapSizeCombobox->addElement(value);
        }
        mapSizeCombobox->setSelectionByText(WorldSize::Medium);

        mapSizeCombobox->setPos(200, y);
        mapSizeCombobox->setWidth(200);
        mapSizeCombobox->setElementFunction([](WorldSize val) -> std::string
                                            { return _(std::string(magic_enum::enum_name(val))); });
        container->addObject(mapSizeCombobox);

        y += yOffset;
        auto difficultyLabel = std::make_shared<UI::Label>(nullptr);
        difficultyLabel->setFont("fonts/arial.ttf", 14);
        difficultyLabel->setText(_("Difficultiy: "));
        difficultyLabel->setPos(20, y);
        container->addObject(difficultyLabel);

        auto difficultyCombobox = std::make_shared<UI::ComboBox<Difficulty>>();
        difficultyCombobox->setFont("fonts/arial.ttf", 14);
        difficultyCombobox->addElement(Difficulty::Easy);
        difficultyCombobox->addElement(Difficulty::Normal);
        difficultyCombobox->addElement(Difficulty::Hard);
        difficulty = Difficulty::Normal;
        difficultyCombobox->setSelectionByText(difficulty);

        difficultyCombobox->setPos(200, y);
        difficultyCombobox->setWidth(200);
        difficultyCombobox->setElementFunction([](Difficulty val) -> std::string
                                               { return _(std::string(magic_enum::enum_name(val))); });

        difficultyCombobox->connect("valueChanged", [&](Difficulty diff)
                                    { difficulty = diff; });
        container->addObject(difficultyCombobox);

        auto cityNameLabel = std::make_shared<UI::Label>(nullptr);
        cityNameLabel->setFont("fonts/arial.ttf", 14);
        cityNameLabel->setText(_("City Names: "));
        cityNameLabel->setPos(20, y);
        container->addObject(cityNameLabel);

        auto cityNamesCombobox = std::make_shared<UI::ComboBox<CityNames>>();
        cityNamesCombobox->setFont("fonts/arial.ttf", 14);
        constexpr auto &cityNames = magic_enum::enum_values<CityNames>();
        for (auto &value : cityNames)
        {
            cityNamesCombobox->addElement(value);
        }
        cityName = CityNames::de;
        cityNamesCombobox->setSelectionByText(cityName);

        cityNamesCombobox->setPos(200, y);
        cityNamesCombobox->setWidth(200);
        cityNamesCombobox->setElementFunction([](CityNames val) -> std::string
                                              { return _(std::string(magic_enum::enum_name(val))); });
        cityNamesCombobox->connect("valueChanged", [&](CityNames diff)
                                   { cityName = diff; });
        container->addObject(cityNamesCombobox);
        winMgr->addContainer(container.get());
        graphics::Rect bounds = {10, 450, 1280, 400};

        layout->updateLayout(bounds);

        bgTexture = graphics::TextureManager::Instance().loadTexture("images/title_background.png");
    }

    void NewGameScene::render()
    {
        bgTexture->renderResized(renderer, 0, 0,
                                 renderer->getMainCamera()->getWidth(),
                                 renderer->getMainCamera()->getHeight());

        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer->setDrawColor(12, 21, 24, 155);
        graphics::Rect bounds = {5, 250, renderer->getViewPort().width - 10, renderer->getViewPort().height - 260};
        utils::Vector2 lineStart(bounds.x, 440);
        utils::Vector2 lineEnd(bounds.x + bounds.width, 440);

        renderer->fillRect(bounds);
        renderer->setDrawColor(93, 103, 108, 255);
        renderer->drawLine(lineStart, lineEnd);
        renderer->drawRect(bounds);
        renderer->setRenderTarget(nullptr);
        renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
        // texture.render(renderer, 0, 0);

        winMgr->render(renderer);
    }

    void NewGameScene::update()
    {
    }

    bool NewGameScene::handleEvents(core::Input *pInput)
    {
        input = pInput;
        return winMgr->handleInput(pInput);
    }

    void NewGameScene::load()
    {
        seed = std::chrono::system_clock::now().time_since_epoch().count();

        seedEdit->setText(std::to_string(seed));
    }

    void NewGameScene::startGame()
    {
        std::cout << "start game" << std::endl;
        world::MapGenerator gen;

        int size = 100 * static_cast<int>(worldSize);

        auto gameMap = gen.generateMap(size, size, numberOfCities, cityName, seed);
        auto cities = gen.getGeneratedCities();
        int cash = 1000000 * int(difficulty);
        auto player = std::make_shared<world::Company>(playerName, cash, true, color);
        player->setAvailableResearch(services::ResearchService::Instance().getData());

        std::vector<std::shared_ptr<world::Company>> companies;

        world::CompanyNameGenerator nameGen("data/company_names.json", seed);
        constexpr auto &colors = magic_enum::enum_values<world::CompanyColor>();

        for (int i = 1; i <= numberOfCompanys; ++i)
        {
            auto company = std::make_shared<world::Company>(nameGen.generateName(), 1000000, false, colors[i]);
            company->setAvailableResearch(services::ResearchService::Instance().getData());
            companies.push_back(company);
        }

        auto gameState = std::make_shared<world::GameState>(companies, player, gameMap, cities, difficulty);

        auto worldScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, gameState, m_gameWindow->getSettings(), input);
        worldScene->setGameWindow(m_gameWindow);
        sceneManager->addScene("world", worldScene);
        sceneManager->setCurrentScene("world");
    }
} // namespace scenes