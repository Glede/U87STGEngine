#include "DynamicsTestScene.h"

USING_NS_CC;
USING_NS_U;

bool AccelerationTest1::init()
{
    if(!TestLayer::init())
        return false;
     
    m_title = "Acceleration Uniformity";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint standard = ccp(winSize.width/2, winSize.height/3);
    int objectNum = 5;
    for(int i = 0; i < objectNum; i++)
    {
        MovableObject* object = MovableObject::create();
        object->setPosition(ccp(standard.x + (500.0f/objectNum)*(i - objectNum/2.0f),standard.y));
        object->addChild(CCSprite::spriteWithFile("TestObject.png"));
        object->setRotation(-90);
        addChild(object);

        float accelTime = 0.2f * i;
        float delayTime = 0.2f * (objectNum - i);

        CCDelayTime* actDelay1 = CCDelayTime::actionWithDuration(delayTime);
        AccelerateBy* actSpeedUp1 = AccelerateBy::create(accelTime, ccp(0, 100));
        CCDelayTime* actMoving1 = CCDelayTime::actionWithDuration(3.0f);
        AccelerateBy* actSpeedDown1 = AccelerateBy::create(delayTime, ccp(0, -100));

        CCDelayTime* actDelay2 = CCDelayTime::actionWithDuration(delayTime);
        AccelerateBy* actSpeedUp2 = AccelerateBy::create(accelTime, ccp(0, -100));
        CCDelayTime* actMoving2 = CCDelayTime::actionWithDuration(3.0f);
        AccelerateBy* actSpeedDown2 = AccelerateBy::create(delayTime, ccp(0, 100));

        CCAction* actGo = Sequence::create(actDelay1, actSpeedUp1, actMoving1, actSpeedDown1, NULL);
        CCAction* actBack = Sequence::create(actDelay2, actSpeedUp2, actMoving2, actSpeedDown2, NULL);

        object->runAction(Sequence::create(actGo, actBack, NULL));
    }

    //MovableObject* object = MovableObject::create();
    //object->setPosition(standard);
    //object->addChild(CCSprite::spriteWithFile("julian_ball.png"));
    //addChild(object);

    //float accelTime = 1.0f; 
    // 
    //AccelerateBy* actAccel = AccelerateBy::create(accelTime, ccp(0, 30));
    //object->runAction(actAccel);

    return true;
}

bool AccelerationTest2::init()
{
    if(!TestLayer::init())
        return false;
     
    m_title = "Acceleration Combination";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
        CCSprite::spriteWithFile("TouchPoint.png"));
    panel->setPosition(ccp(200, 200));
    panel->setInnerRadius(25);
    addChild(panel);

    panel->setHandler(this);

    m_controlObject = MovableObject::create();
    m_controlObject->addChild(CCSprite::spriteWithFile("TestObject.png"));
    m_controlObject->setLocation(ccpFromSize(winSize)/2);
    addChild(m_controlObject);

    return true;
}

void AccelerationTest2::wheelTouchEnded(u87::TouchWheel* sender, const Point& point)
{ 
    AccelerateBy* accel = AccelerateBy::create(5.0f, 100 * point);
    m_controlObject->runAction(accel);
};

bool AccelerationTest3::init()
{
    if(!TestLayer::init())
        return false;
     
    m_title = "Related Acceleration";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    MovableObject* object = MovableObject::create();
    object->addChild(CCSprite::spriteWithFile("TestObject.png"));
    object->setLocation(ccpFromSize(winSize)/2);
    addChild(object);

    RoturnBy* actRoturn = RoturnBy::create(10.0f, 720);
    AccelerateBy* actAccel = AccelerateBy::create(5.0f, ccp(450, 0), true);
    AccelerateBy* actDeccel = AccelerateBy::create(5.0f, ccp(-450, 0), true);
    Sequence* actAccelUpDown = Sequence::create(actAccel, actDeccel, NULL);

    object->runAction(Repeat::create(Spawn::create(actRoturn, actAccelUpDown, NULL)));

    return true;
}


bool AccelerationTest4::init()
{
    if(!TestLayer::init())
        return false;
     
    m_title = "Accelerate To Target Speed";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  

    TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
        CCSprite::spriteWithFile("TouchPoint.png"));
    panel->setPosition(ccp(200, 200));
    panel->setInnerRadius(25);
    addChild(panel);

    panel->setHandler(this);   


    m_controlObject = MovableObject::create();
    m_controlObject->addChild(CCSprite::spriteWithFile("TestObject.png"));
    m_controlObject->setLocation(ccp(winSize.width/4, winSize.height/2));
    addChild(m_controlObject);
 
    return true;
}

void AccelerationTest4::wheelTouchUpdated(TouchWheel* sender, const Point& point, float dt)
{  
    float length = ccpLength(point);

    Vector2 accel = length > 0? (point/length * 240): VECTOR2_ZERO;
    m_controlObject->accelerateTo(accel, length*480*dt);
};
 