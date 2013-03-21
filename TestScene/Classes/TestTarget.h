#pragma once


#include "TestScene.h"
#include "DynamicsTestScene.h"

const u87::CTag TEST_UNIT = 15;

class TestTarget: public u87::MovableObject, public u87::GameUnit
{
public:
    bool init();

    CREATE_FUNC(TestTarget);

    virtual void update(float dt);

    virtual bool onCollision(const u87::HitRecord& hr);

    inline cocos2d::CCSprite* getVisual() const { return m_round; }

protected:
    cocos2d::CCSprite* m_round;
    bool m_hit;
};


const u87::EventType TEST_EVENT = 3;


class TestEvent: public u87::GameEvent
{
public:
    EVENTTYPE_FUNC(TEST_EVENT);

    bool init(const u87::Point& point, cocos2d::CCNode* _targ) { target = _targ; hitPoint = point; return true; }
    CREATE_FUNC2(TestEvent, const u87::Point&, hitPoint, cocos2d::CCNode*, _targ);
    
    cocos2d::CCNode* target;
    u87::Point hitPoint;
};

