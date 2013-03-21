#include "TestScene.h"
#include <sstream>

USING_NS_U;
USING_NS_CC;
using namespace std;


///////////////////////////////////////////////////////////////////////////////
// Test Scene
///////////////////////////////////////////////////////////////////////////////

TestScene::~TestScene()
{
    m_testLayers->release();
}

bool TestScene::init()
{
    bool bRet = false;
    
    do
    {
        m_testLayers = CCArray::array();
        m_testLayers->retain();  
        m_current = 0;

        // init
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSize buttonSize = CCSizeMake(60 , 60);

        CCLayer* control = CCLayer::node();
        addChild(control, 1000);

#define itemFromNormalImage itemWithNormalImage
        CCMenuItemImage* next = CCMenuItemImage::itemFromNormalImage("NextButton.png", "NextButton.png", this, 
            menu_selector(TestScene::next));
        next->setPosition(ccp(winSize.width - buttonSize.width * 1, winSize.height - buttonSize.height));
        CCMenuItemImage* reset = CCMenuItemImage::itemFromNormalImage("ResetButton.png", "ResetButton.png", this, 
            menu_selector(TestScene::reset));
        reset->setPosition(ccp(winSize.width - buttonSize.width * 2, winSize.height - buttonSize.height));
        CCMenuItemImage* prev = CCMenuItemImage::itemFromNormalImage("PreviousButton.png", "PreviousButton.png", this, 
            menu_selector(TestScene::prev));
        prev->setPosition(ccp(winSize.width - buttonSize.width * 3, winSize.height - buttonSize.height));
        CCMenuItemImage* back = CCMenuItemImage::itemFromNormalImage("BackButton.png", "BackButton.png", this, 
            menu_selector(TestScene::back));
        back->setPosition(ccp(winSize.width - buttonSize.width * 4, winSize.height - buttonSize.height));
#undef itemFromNormalImage

        CCMenu* menu = CCMenu::menuWithItems(reset, next, prev, back, NULL);
        menu->setPosition(CCPointZero);
        control->addChild(menu);

        m_title = CCLabelTTF::labelWithString("", "Consolas", 30);
        m_title->setAnchorPoint(ccp(0.0f, 0.5f));
        m_title->setPosition(ccp(buttonSize.width, winSize.height - buttonSize.height));
        control->addChild(m_title);

        m_index = CCLabelTTF::labelWithString("", "Consolas", 30);
        m_index->setAnchorPoint(ccp(1.0f, 0.5f));
        m_index->setPosition(ccp(winSize.width - buttonSize.width * 1, winSize.height - buttonSize.height * 2));
        control->addChild(m_index);

        bRet = true;
    }while(0);
    return true;
}

void TestScene::onEnter()
{
    CCScene::onEnter();
    runTestLayer();
}

void TestScene::reset(CCObject* sender)
{
    if(m_testLayers-> count() == 0)
        return;

    TestLayer* m_currentLayer = (TestLayer*)m_testLayers->objectAtIndex(m_current);
    m_currentLayer->reset();
}

void TestScene::next(CCObject* sender)
{
    if(m_testLayers->count() == 0)
        return;

    TestLayer* m_currentLayer = getCurrentTestLayer();
    removeChild(m_currentLayer, true);

    m_current++;
    m_current %= m_testLayers->count();
    runTestLayer();
}

void TestScene::prev(CCObject* sender)
{
    if(m_testLayers->count() == 0)
        return;

    TestLayer* m_currentLayer = getCurrentTestLayer();
    removeChild(m_currentLayer, true);

    m_current--;
    if(m_current < 0)
        m_current += m_testLayers->count();
    m_current %= m_testLayers->count();
    runTestLayer();
}

void TestScene::back(CCObject* sender)
{
    CCDirector::sharedDirector()->popScene();
}

void TestScene::runTestLayer()
{
    TestLayer* m_currentLayer = getCurrentTestLayer();
    m_currentLayer = (TestLayer*)m_testLayers->objectAtIndex(m_current);
    m_currentLayer->reset();
    addChild(m_currentLayer);
    m_title->setString(m_currentLayer->getTitle().c_str());

    stringstream index;   
    index << m_current + 1 << "/" << m_testLayers->count(); 
    m_index->setString(index.str().c_str());
}

TestLayer* TestScene::getCurrentTestLayer()
{
    return (TestLayer*)m_testLayers->objectAtIndex(m_current);
}


///////////////////////////////////////////////////////////////////////////////
// Test Layer
///////////////////////////////////////////////////////////////////////////////

bool TestLayer::init()
{
    if(!CCLayer::init())
        return false;

    m_title = "Any Test";

    return true;
} 

void TestLayer::reset()
{
    stopAllActions();
    unscheduleAllSelectors();
    removeAllChildrenWithCleanup(true);

    init();
} 