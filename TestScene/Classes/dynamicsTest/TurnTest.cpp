#include "DynamicsTestScene.h"

USING_NS_CC;
USING_NS_U;

bool TurnTest::init()
{
    if(!TestLayer::init())
        return false;

    m_title = "Turn Test";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    MovableObject* object = MovableObject::create();
    object->addChild(CCSprite::spriteWithFile("TestObject.png"));
    object->setLocation(ccpFromSize(winSize)/2);
    addChild(object);

    object->accelerateTo(ccp(150,0));
     
    TurnBy* actTurn = TurnBy::create(2.0f, 180.0f);
    CCDelayTime* actDelay = CCDelayTime::actionWithDuration(1.0f);
    object->runAction(Repeat::create(Sequence::create(actTurn, actDelay, NULL)));

    return true;
}