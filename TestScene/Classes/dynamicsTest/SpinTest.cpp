#include "DynamicsTestScene.h"

USING_NS_CC;
USING_NS_U;

bool SpinTest1::init()
{
    if(!TestLayer::init())
        return false;

    m_title = "Spin Test";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    MovableObject* object = MovableObject::create();
    object->addChild(CCSprite::spriteWithFile("TestObject.png"));
    object->setLocation(ccpFromSize(winSize)/2);
    addChild(object);

    SpinBy* actSpin1 = SpinBy::create(3.0f, 5*360.0f);
    SpinBy* actSpin2 = SpinBy::create(3.0f, -10*360.0f);
    SpinBy* actSpin3 = SpinBy::create(3.0f, 5*360.0f);
    CCAction* actSeq = Sequence::create(actSpin1, actSpin2, actSpin3, NULL);
    object->runAction(Repeat::create(actSeq));

    return true;
}


bool SpinTest2::init()
{
    if(!TestLayer::init())
        return false;

    m_title = "Spin To Target Velocity";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // object 1 with auto rate
    {
        MovableObject* object = MovableObject::create();
        object->addChild(CCSprite::spriteWithFile("TestObject.png"));
        object->setLocation(ccp(winSize.width/3, winSize.height/2));
        addChild(object);

        SpinTo* actSpin = SpinTo::create(false, 180.0f);
        object->runAction(Repeat::create(actSpin));
    }

    // object 2 with set rate
    {
        MovableObject* object = MovableObject::create();
        object->addChild(CCSprite::spriteWithFile("TestObject.png"));
        object->setLocation(ccp(winSize.width/3*2, winSize.height/2));
        addChild(object);

        SpinTo* actSpin = SpinTo::create(false, 180.0f, 180);
        object->runAction(Repeat::create(actSpin));
    }

    return true;
}