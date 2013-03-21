#include "GameFunctionTestScene.h"

USING_NS_CC;
USING_NS_U;
 

bool TargetUnitTest::init()
{
    bool bRet = false;

    do
    {
        CC_BREAK_IF(!TestLayer::init());

        m_title = "Target Unit Hit Test";

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

        bRet = true;
    } while(0);

    return bRet;
}