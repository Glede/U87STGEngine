#include "CollisionTestScene.h"

USING_NS_CC;
USING_NS_U;

const int TAG_MOVE_WHEEL = 15;
const int TAG_ROT_WHEEL = 25; 

bool CollisionTest::init()
{
    bool bRet = false;

    do
    {
        setTouchEnabled(false);
        CC_BREAK_IF(TestLayer::init() == false);
        setTouchEnabled(true);

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // init world
        if(World::sharedInstance() != NULL)
        {
            if(World::sharedInstance()->getParent() != NULL)
                World::sharedInstance()->removeFromParentAndCleanup(true);
            
            delete World::sharedInstance();
        }

        addChild(World::create()); 

        //add to listen event
        listenGameEvent(TEST_EVENT, EVT_NONE);

        TestTarget** t = m_targets;
        for(int i = 0; i < 6; i++)
        {
            t[i] = TestTarget::create();
            t[i]->setResistanceFactor(0.90f);
            t[i]->setUnitType(TEST_UNIT);
            t[i]->addToWorld();

            m_hitpoint[i] = CCSprite::spriteWithFile("TouchPoint.png");
            m_hitpoint[i]->setScale(0.5f);
            World::sharedInstance()->addChild(m_hitpoint[i]);
        }
        t[0]->setPosition(ccp(winSize.width/2 + 100, winSize.height/3*1));
        t[1]->setPosition(ccp(winSize.width/2, winSize.height/3*1));
        t[2]->setPosition(ccp(winSize.width/2 - 100, winSize.height/3*1));
        t[3]->setPosition(ccp(winSize.width/2 + 100, winSize.height/3*2));
        t[4]->setPosition(ccp(winSize.width/2, winSize.height/3*2));
        t[5]->setPosition(ccp(winSize.width/2 - 100, winSize.height/3*2));
        t[0]->addChild(t[0]->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
        t[1]->addChild(t[1]->addBody(RectBodyNode::create(25, 50), TEST_UNIT, TEST_UNIT, CLT_NONE));
        t[2]->addChild(t[2]->addBody(StripBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
        t[3]->addChild(t[3]->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
        t[4]->addChild(t[4]->addBody(RectBodyNode::create(25, 50), TEST_UNIT, TEST_UNIT, CLT_NONE));
        t[5]->addChild(t[5]->addBody(StripBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
         
        m_selected = NULL;

        // input
        {
            TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
                CCSprite::spriteWithFile("TouchPoint.png"));
            panel->setPosition(ccp(200, 200));
            panel->setInnerRadius(25);
            panel->setTag(TAG_MOVE_WHEEL);
            panel->setHandler(this);
            addChild(panel);
        }

        {
            TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
                CCSprite::spriteWithFile("TouchPoint.png"));
            panel->setPosition(ccp(winSize.width - 200, 200));
            panel->setInnerRadius(25);
            panel->setTag(TAG_ROT_WHEEL);
            panel->setHandler(this);
            addChild(panel);
        }

        bRet = true;
    } while(0);

    return bRet;
}
 
void CollisionTest::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent)
{
    CCTouch* touch = (CCTouch*)pTouches->anyObject();
    CCPoint pos = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    pos = World::sharedInstance()->convertToWorldSpace(pos);

    if(m_selected)
    {  
        CCFadeTo* fade = CCFadeTo::actionWithDuration(0.1f, 255);  
        m_selected->getVisual()->runAction(fade);
    }

    for(int i = 0; i < 6; i++)
    { 
        if(isPointInRound(pos, m_targets[i]->getPosition(), 50))
        {
            m_selected = m_targets[i];
            CCFadeTo* fade = CCFadeTo::actionWithDuration(0.1f, 155);  
            m_selected->getVisual()->runAction(fade);
            return;
        }
    }
    m_selected = NULL;
}



void CollisionTest::wheelTouchUpdated(u87::TouchWheel* sender, const u87::Point& point, float dt)
{
    if(m_selected == NULL)
        return;

    switch(sender->getTag())
    {
    case TAG_MOVE_WHEEL:
        {
            m_selected->accelerateTo(point * 150 , 200 * dt);
        }
        break;
    case TAG_ROT_WHEEL:
        {
            float rv = angleForVector2(point);
            rv += 90;
            rv = normalizeDegree(rv);
            m_selected->rotate(rv * 0.25f * dt);
        }
        break;
    }
}

void CollisionTest::onGameEvent(EventType type, GameEvent* gameEvent)
{
    switch(type)
    {
    case TEST_EVENT:
        {
            TestEvent* e = (TestEvent*)gameEvent;
            for(int i = 0; i < 6; i++)
            {
                if(e->target == m_targets[i])
                {
                    m_hitpoint[i]->setPosition(e->hitPoint);
                    break;
                }
            }
        }
    }
}