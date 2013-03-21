#pragma once

#include "U87Engine.h"

class MenuScene: public cocos2d::CCScene
{
public:
    bool init();

    CREATE_FUNC(MenuScene);

    void runTestScene(cocos2d::CCObject* sender);
};