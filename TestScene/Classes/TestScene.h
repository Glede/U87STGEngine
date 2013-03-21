#pragma once

#include "U87Engine.h"
 
class TestLayer;

class TestScene: public cocos2d::CCScene
{
public:
    ~TestScene();
    bool init();
    CREATE_FUNC(TestScene);

    virtual void onEnter();

    void reset(cocos2d::CCObject* sender);
    void next(cocos2d::CCObject* sender);
    void prev(cocos2d::CCObject* sender);
    void back(cocos2d::CCObject* sender);
     
    
    CC_SYNTHESIZE(cocos2d::CCLabelTTF*, m_title, Title);
    CC_SYNTHESIZE(cocos2d::CCLabelTTF*, m_index, Index);

protected:
    void runTestLayer();
    TestLayer* getCurrentTestLayer();

protected:
    cocos2d::CCArray* m_testLayers;
    int m_current;
};

class TestLayer: public cocos2d::CCLayer
{
public:
    virtual bool init();
    virtual void reset();

    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_title, Title);
};

