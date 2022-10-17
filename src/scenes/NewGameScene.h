#pragma once
#include <engine/core/Scene.h>
#include "constants.h"
#include "world/company.h"

namespace core
{
    class SceneManager;
};
namespace UI
{
    class ScrollArea;
    class TextItem;
};

namespace scenes
{

    class NewGameScene : public core::Scene
    {
    public:
        NewGameScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager);

        void render();
        void update();
        bool handleEvents(core::Input *pInput);
        virtual void load();

    private:
        void startGame();

        core::SceneManager *sceneManager;
        std::shared_ptr<graphics::Texture> bgTexture;
        std::shared_ptr<UI::Container> container;
        std::shared_ptr<UI::Container> buttonList;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        std::shared_ptr<UI::TextItem> seedEdit;
        unsigned long seed;
        WorldSize worldSize;
        Difficulty difficulty;
        CityNames cityName;
        std::string playerName;
        int numberOfCompanys;
        int numberOfCities;
        world::CompanyColor color;
    };
}; // namespace scenes
