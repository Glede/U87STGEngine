#pragma once

#include "TestScene.h"

class AccelerationTest1: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(AccelerationTest1);         
};             


class AccelerationTest2: public TestLayer, public u87::TouchWheelInputHandler  
{                           
public:                     
    bool init();            
    CREATE_FUNC(AccelerationTest2);         

    virtual void wheelTouchEnded(u87::TouchWheel* sender, const cocos2d::CCPoint& point); 

protected:
    u87::MovableObject* m_controlObject; 
};             


class AccelerationTest3: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(AccelerationTest3);         
};             


class AccelerationTest4: public TestLayer, public u87::TouchWheelInputHandler     
{                           
public:                     
    bool init();            
    CREATE_FUNC(AccelerationTest4);             
    
    virtual void wheelTouchUpdated(u87::TouchWheel* sender, const cocos2d::CCPoint& point, float dt); 

protected:
    u87::MovableObject* m_controlObject;
};             

class SpinTest1: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(SpinTest1);         
};

class SpinTest2: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(SpinTest2);         
};

class RotationTest: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(RotationTest);         
};

class TurnTest: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(TurnTest);         
};

class RoturnTest: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(RoturnTest);         
};

class TimeControlTest: public TestLayer   
{                           
public:                     
    bool init();            
    CREATE_FUNC(TimeControlTest);     
    
protected:
    u87::MovableObject* m_controlObject;
};






class DynamicsTestScene: public TestScene
{
public:
    bool init()
    {
        if(!TestScene::init())
            return false;

        // init all testLayers
        m_testLayers->addObject(AccelerationTest1::create());
        m_testLayers->addObject(AccelerationTest2::create());
        m_testLayers->addObject(SpinTest1::create());
        m_testLayers->addObject(RotationTest::create());
        m_testLayers->addObject(TurnTest::create());
        m_testLayers->addObject(RoturnTest::create());
        m_testLayers->addObject(AccelerationTest3::create());
        m_testLayers->addObject(AccelerationTest4::create());
        m_testLayers->addObject(SpinTest2::create());
        m_testLayers->addObject(TimeControlTest::create());

        return true;
    }

    CREATE_FUNC(DynamicsTestScene);
};