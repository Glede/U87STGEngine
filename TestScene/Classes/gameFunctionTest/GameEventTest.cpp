#include "GameFunctionTestScene.h"

USING_NS_CC;
USING_NS_U;

bool GameEventTest::init()
{
    bool bRet = false;

    do
    {
        CC_BREAK_IF(!TestLayer::init());

        m_title = "Game Event Test";

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // init world
        if(World::sharedInstance() != NULL)
        {
            if(World::sharedInstance()->getParent() != NULL)
                World::sharedInstance()->removeFromParentAndCleanup(true);
            
            delete World::sharedInstance();
        }

        addChild(World::create());
    
        TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
            CCSprite::spriteWithFile("TouchPoint.png"));
        panel->setPosition(ccp(200, 200));
        panel->setInnerRadius(25);
        addChild(panel);

        {
            CCSprite* hitPoint = CCSprite::spriteWithFile("TouchPoint.png");
            addChild(hitPoint);
            m_hitPoint = hitPoint;
        }

        panel->setHandler(this);   

        {
            TestTarget* tt = TestTarget::create();
            tt->addChild(tt->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
            tt->setPosition(ccp(winSize.width/3, winSize.height/2));
            tt->setUnitType(TEST_UNIT);
            tt->addToWorld();
        }

        {
            TestTarget* tt = TestTarget::create();
            tt->addChild(tt->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
            tt->addToWorld();
            m_controlObject = (MovableObject*)tt;
            m_controlObject->setPosition(ccp(winSize.width/3*2, winSize.height/2));
        }

        listenGameEvent(TEST_EVENT, EVT_NONE);

        bRet = true;
    } while(0);

    return bRet;
}

void GameEventTest::onGameEvent(EventType type, GameEvent* gameEvent)
{
    switch(type)
    {
    case TEST_EVENT:
        {
            TestEvent* evnt = (TestEvent*)gameEvent;
            m_hitPoint->setPosition(evnt->hitPoint);
        }
    }
}