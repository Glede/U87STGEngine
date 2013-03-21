#include "TestTarget.h"

USING_NS_CC;
USING_NS_U;


bool TestTarget::init()
{
    MovableObject::init(); 

    m_round = CCSprite::spriteWithFile("TestRound.png");
    addChild(m_round);

    m_hit = false;
     
    scheduleUpdate();

    return true;
}

void TestTarget::update(float dt)
{
    if(m_hit)
        m_round->setColor(ccc3(255,0,0));
    else
        m_round->setColor(ccc3(255,255,255));
    m_hit = false;
}

bool TestTarget::onCollision(const HitRecord& hr)
{
    TestTarget* unit = dynamic_cast<TestTarget*>(hr.target->getGameUnit());
    if(unit->getUnitType() == TEST_UNIT)
    {
        m_hit = true;
    }
    GameEventDispatcher::sharedInstance()->dispatchGameEvent(TestEvent::create(hr.hitPoint, unit));
    return false;
}

