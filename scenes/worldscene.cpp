#include "worldscene.h"
#include <iostream>
#include <cmath>
#include <engine/utils/os.h>
#include <engine/utils/string.h>

namespace scenes {


WorldScene::WorldScene(core::Renderer *pRenderer,
                         core::SceneManager* pSceneManager) :
                         core::Scene(pRenderer, &uiTexture), sceneManager(
                                         pSceneManager)
                       ,buildWindow(0,pRenderer->getViewPort().height/2)
                       ,buildingWindow(100,100)
{
    cursorTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","tiles","iso_cursor.png"));
    hudTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","hud_base.png"));
    hudFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts","arial.ttf"),16);
    gameMap = std::make_shared<GameMap>(100,100);
    mapRenderer = std::make_shared<GameMapRenderer>(gameMap);
    auto playerCompany = std::make_shared<world::Company>("Player Company",1000000,true);
    gameState = std::make_shared<world::GameState>(playerCompany);
    renderer->setZoomFactor(6);
    uiTexture.loadTexture(renderer,utils::os::combine("images","ArkanaLook.png"));

    thread = std::make_unique<UpdateThread>(gameState);
    winMgr->addWindow(&buildingWindow);

}
WorldScene::~WorldScene()
{
    thread->stop();
}
std::shared_ptr<world::Building> WorldScene::createBuilding(world::BuildAction action)
{
    std::shared_ptr<world::Building> building = nullptr;
    graphics::Rect rect;
    rect.width = (float)mapRenderer->getTileWidth();
    rect.height = (float)mapRenderer->getTileHeight();
    switch(action)
    {
        case world::BuildAction::Farm:
        building = std::make_shared<world::Building>("Farm","A farm",10000);
        rect.x= 754;
        rect.y = 236;
        building->setSourceRect(rect);
        break;

        case world::BuildAction::Factory:
        building = std::make_shared<world::Building>("Factory","A factory",20000,3,3);
        rect.x = 14;
        rect.y = 1878;
        rect.width = 184;
        rect.height = 162;
        building->setOffset(0,-116);
        building->setSourceRect(rect);

        break;

    default:
       break;
    }
    return building;
}

void WorldScene::renderHUD()
{
    int y = renderer->getMainCamera()->getHeight() - hudTexture->getHeight();
    hudTexture->render(renderer,0,y);

    //render hud text
    SDL_Color color = {0xef,0xef,0xef,0xff};
    y+=5;
    hudFont->render(renderer,"Cash: ",color,200,y+(hudTexture->getHeight()/2));
    auto playerCompany = gameState->getPlayer();
    hudFont->render(renderer,utils::string_format("%'.2f €",playerCompany->getCash()),color,250,y+(hudTexture->getHeight()/2));

    std::time_t tmpTime = std::chrono::system_clock::to_time_t(
            gameState->getTime());


    char date_time_format[] = "%d.%m.%Y";

    char time_str[100];

    std::strftime(time_str, 100, date_time_format,
            std::localtime(&tmpTime));

    hudFont->render(renderer,"Date: "+std::string(time_str),color,200,y+(hudTexture->getHeight()/2)+20);
    hudFont->render(renderer,"Profit:",color,400,y+(hudTexture->getHeight()/2));
    hudFont->render(renderer,utils::string_format("%'.2f €",playerCompany->getProfit()),color,450,y+(hudTexture->getHeight()/2));
    //render icons
    buildWindow.render(renderer,nullptr);
}


void WorldScene::render(){
    int width = gameMap->getHeight()*mapRenderer->getTileWidth();
    int height = gameMap->getWidth()* mapRenderer->getTileHeight()/2;
    graphics::Texture mapTexture(renderer,width,height);
    renderer->setRenderTarget(mapTexture.getSDLTexture());
    mapRenderer->render(renderer);
    renderer->setRenderTarget(nullptr);
    graphics::Texture resizedTexture(renderer,mapTexture.getWidth()* 6.0f/renderer->getZoomFactor(),mapTexture.getHeight()* 6.0f/renderer->getZoomFactor());
    renderer->setRenderTarget(resizedTexture.getSDLTexture());
    mapTexture.renderResized(renderer,0,0,mapTexture.getWidth()* 6.0f/renderer->getZoomFactor(),mapTexture.getHeight()* 6.f/renderer->getZoomFactor());
    renderer->setRenderTarget(nullptr);
    resizedTexture.render(renderer,0,0,renderer->getMainCamera()->getWidth(),renderer->getMainCamera()->getHeight(),0,0);


    float cursorX = (cursorPosition.getX()*mapRenderer->getTileWidth()/2.f)-renderer->getMainCamera()->getX();
    float cursorY = (cursorPosition.getY()*mapRenderer->getTileHeight())-renderer->getMainCamera()->getY();
    utils::Vector2 pos = gameMap->twoDToIso(utils::Vector2(cursorX,cursorY));
    cursorTexture->render(renderer,pos.getX(),pos.getY());//,mapRenderer->getTileWidth(),mapRenderer->getTileHeight());
    renderHUD();

    winMgr->render(renderer);
}
void WorldScene::handleEvents(core::Input *pInput){
    bool mouseIntersectsWindow = buildWindow.displayRect().intersects(pInput->getMousePostion());

    if(!mouseIntersectsWindow && !winMgr->isWindowOpen())
    {
        if(pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            std::cout<< "build here"<<std::endl;
            auto action = buildWindow.getCurrentAction();
            if(action == world::BuildAction::Destroy)
            {
                //check if building exists and then destroy it

            }else if (action != world::BuildAction::None)
            {
                auto building = createBuilding(action);
                building->setPosition(cursorPosition.getX(),cursorPosition.getY());

                if(building != nullptr && building->canBuild(gameState->getPlayer()->getCash())
                        && gameMap->canBuild(building->get2DPosition()))
                {


                    gameMap->addBuilding(building);
                    gameState->getPlayer()->addBuilding(building);
                    gameState->getPlayer()->incCash(building->getBuildPrice()*-1);
                }
            }else{
                auto building = gameMap->getBuilding2D(cursorPosition);
                buildingWindow.open(building,gameMap->getTile(cursorPosition));
            }

        }else if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            buildWindow.setCurrentAction(world::BuildAction::None);
        }
        if(pInput->isScrollWheel())
        {
            auto wheelPosition = pInput->getMouseWheelPosition();
            float factor = renderer->getZoomFactor()+wheelPosition.getY();
            if(factor < 3)
            {
                factor = 3;
            }else if (factor > 18) {
                factor = 18;

            }
            renderer->setZoomFactor(factor);
            std::cout<<"factor: "<<factor<<std::endl;
        }
        if(pInput->isMouseMoving()){

            utils::Vector2 pt = gameMap->isoTo2D(pInput->getMousePostion()- utils::Vector2(renderer->getMainCamera()->getX(),renderer->getMainCamera()->getY())) - utils::Vector2(mapRenderer->getTileWidth()/2,mapRenderer->getTileHeight()/2);
            float x,y = 0.0;
            x = std::round(pt.getX() / static_cast<float>(mapRenderer->getTileHeight()));
            y = std::round(pt.getY() / static_cast<float>(mapRenderer->getTileHeight()));

            if(y == 0.0f)
            std::cout <<"x: "<<x<<" y: "<<y<<std::endl;

            cursorPosition = utils::Vector2(x,y);
        }
    }

    float yOffset = 0;
    float xOffset = 0;
    if(pInput->isKeyDown(SDLK_w))
    {
        yOffset = -1.0f;
    }else if (pInput->isKeyDown(SDLK_s))
    {
        yOffset = 1.0f;
    }
    if(pInput->isKeyDown(SDLK_a))
    {
        xOffset = -1.0f;
    }else if (pInput->isKeyDown(SDLK_d))
    {
        xOffset = 1.0f;
    }
    renderer->getMainCamera()->move(xOffset*5.0f,yOffset*5.0f);

    buildWindow.handleEvents(pInput);
    winMgr->handleInput(pInput);
}
}
