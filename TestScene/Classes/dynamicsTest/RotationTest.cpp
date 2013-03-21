#include "DynamicsTestScene.h"

USING_NS_CC;
USING_NS_U;

bool RotationTest::init()
{
    if(!TestLayer::init())
        return false;

    m_title = "Rotation Test";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    MovableObject* object = MovableObject::create();
    object->addChild(CCSprite::spriteWithFile("TestObject.png"));
    object->setLocation(ccpFromSize(winSize)/2);
    addChild(object);
     
    RotateBy* actRot = RotateBy::create(2.0f, 360.0f);
    CCDelayTime* actDelay = CCDelayTime::actionWithDuration(1.0f);
    object->runAction(Sequence::create(actRot, actDelay, NULL));

    return true;
}