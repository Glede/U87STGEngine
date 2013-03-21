#include "MenuScene.h"
#include "DynamicsTestScene.h"
#include "GameFunctionTestScene.h"
#include "CollisionTestScene.h"

USING_NS_CC;
USING_NS_U;

const int TAG_Physics_Test       = 1;
const int TAG_Collision_Test     = 2;
const int TAG_Game_Function_Test = 3;


bool MenuScene::init()
{
    bool bRet = false;

    do
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        CCMenu* menu = CCMenu::menuWithItems(NULL);
        menu->setPosition(POINT_ZERO);
        addChild(menu);

        CCMenuItemLabel* phytest = CCMenuItemLabel::itemWithLabel(
            CCLabelTTF::labelWithString("Dynamics Test", "Consolas", 40),
            this, menu_selector(MenuScene::runTestScene));
        phytest->setPosition(ccp(winSize.width/2, winSize.height - 200));
        menu->addChild(phytest, 0, TAG_Physics_Test);
                
        CCMenuItemLabel* gamefunctest = CCMenuItemLabel::itemWithLabel(
            CCLabelTTF::labelWithString("Game Function Test", "Consolas", 40),
            this, menu_selector(MenuScene::runTestScene));
        gamefunctest->setPosition(ccp(winSize.width/2, winSize.height - 250));
        menu->addChild(gamefunctest, 0, TAG_Game_Function_Test);
               
        
        CCMenuItemLabel* collisionTest = CCMenuItemLabel::itemWithLabel(
            CCLabelTTF::labelWithString("Collision Test", "Consolas", 40),
            this, menu_selector(MenuScene::runTestScene));
        collisionTest->setPosition(ccp(winSize.width/2, winSize.height - 300));
        menu->addChild(collisionTest, 0, TAG_Collision_Test);
               
        bRet = true;
    } while(0);

    return bRet;
}

void MenuScene::runTestScene(CCObject* sender)
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCNode* _sender = (CCNode*)sender;
 
    switch(_sender->getTag())
    {
    case TAG_Physics_Test:
        pDirector->pushScene(DynamicsTestScene::create());
        return;
    case TAG_Game_Function_Test:
        pDirector->pushScene(GameFunctionTestScene::create());
        return;
    case TAG_Collision_Test:
        pDirector->pushScene(CollisionTestScene::create());
        return;
    default:
        break;
    }
}