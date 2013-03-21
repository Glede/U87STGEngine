#include "DynamicsTestScene.h"

USING_NS_U;
USING_NS_CC;

bool TimeControlTest::init()
{
    if(!TestLayer::init())
        return false;

    m_title = "Time Control Test";

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
     
    // time scale modified object
    {
        MovableObject* object = MovableObject::create();
        object->addChild(CCSprite::spriteWithFile("TestObject.png"));
        object->setLocation(ccpFromSize(winSize)/2);
        addChild(object);

        object->accelerateTo(ccp(150,0));
     
        RoturnBy* actRoturn = RoturnBy::create(3600.0f, 3600 * 90.0f);
        object->runAction(Repeat::create(actRoturn));

        object->setTimeScale(0.5f);

        m_controlObject = object;
    }
 
    // reference object
    {
        MovableObject* object = MovableObject::create();
        CCSprite* visual = CCSprite::spriteWithFile("TestObject.png");
        visual->setOpacity(155);
        object->addChild(visual);        
        object->setLocation(ccpFromSize(winSize)/2);
        addChild(object);

        object->accelerateTo(ccp(150,0));
     
        RoturnBy* actRoturn = RoturnBy::create(3600.0f, 3600 * 90.0f);
        object->runAction(Repeat::create(actRoturn));
    }
     


    return true;
}