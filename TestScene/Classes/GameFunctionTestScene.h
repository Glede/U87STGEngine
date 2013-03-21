#pragma once

#include "TestScene.h"
#include "DynamicsTestScene.h"
#include "TestTarget.h"






class TargetUnitTest: public AccelerationTest4
{
public:
    bool init();
    
    CREATE_FUNC(TargetUnitTest);
     
};


class GameEventTest: public TargetUnitTest, public u87::GameEventHandler
{
public:
    bool init();

    CREATE_FUNC(GameEventTest);

    virtual void onGameEvent(u87::EventType type, u87::GameEvent* gameEvent);

protected:
    cocos2d::CCNode* m_hitPoint;
};


class CameraTest: public TestLayer, public u87::TouchWheelInputHandler
{
public:
    bool init();
    //virtual void update(float dt);
    CREATE_FUNC(CameraTest);

    virtual void update(float dt);

    virtual void shockPressed(cocos2d::CCObject* sender);

    virtual void wheelTouchUpdated(u87::TouchWheel* sender, const u87::Point& point, float dt);

protected: 
    cocos2d::CCNode* m_centerObject;
    u87::CameraNode* m_cam;
};


class GameFunctionTestScene: public TestScene
{
public:
    bool init()
    {
        if(!TestScene::init())
            return false;
        
        m_testLayers->addObject(TargetUnitTest::create());
        m_testLayers->addObject(GameEventTest::create());
        m_testLayers->addObject(CameraTest::create());

        u87::BodyNode::setDrawBody(true);

        return true;
    }

    CREATE_FUNC(GameFunctionTestScene);
};