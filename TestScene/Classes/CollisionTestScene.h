#pragma once

#include "TestScene.h"
#include "TestTarget.h"



class CollisionTest: public TestLayer, public u87::TouchWheelInputHandler, public u87::GameEventHandler
{
public:
    bool init();
    CREATE_FUNC(CollisionTest);
     
    void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    virtual void wheelTouchUpdated(u87::TouchWheel* sender, const u87::Point& point, float dt);

    virtual void onGameEvent(u87::EventType type, u87::GameEvent* gameEvent);

protected:
    TestTarget* m_selected;
    TestTarget* m_targets[6];
    cocos2d::CCSprite* m_hitpoint[6];
};


class TerrainTest: public TestLayer
{
public:
	bool init();
	CREATE_FUNC(TerrainTest);
	virtual void draw();
protected:
	u87::Terrain terrain;
};


class CollisionTestScene: public TestScene
{
public:
    bool init()
    {
        if(TestScene::init() == false)
            return false;
        
        m_testLayers->addObject(CollisionTest::create());
        m_testLayers->addObject(TerrainTest::create());

        u87::BodyNode::setDrawBody(true);

        return true;
    }

    CREATE_FUNC(CollisionTestScene);
};
