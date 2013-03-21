#pragma once

#include "U87Util.h"
#include "U87Math.h"

namespace u87
{
    const int TOUCH_PRIORITY_PANEL = 0;

    class TouchWheelInputHandler;
    class TouchWheel;
    class SlidebarInputHandler;
    class Slidebar;

    /** interface : touch wheel input handler */
    class TouchWheelInputHandler
    {
    public:
        /** optional , when touch on a wheel is began*/
        virtual void wheelTouchBegan(TouchWheel* sender, const Point& point) {}

        /** optional , when touch on a wheel is moved*/
        virtual void wheelTouchMoved(TouchWheel* sender, const Point& point) {}

        /** optional , when touch on a wheel is ended*/
        virtual void wheelTouchEnded(TouchWheel* sender, const Point& point) {}

        /** optional , when touch on a wheel is updated*/
        virtual void wheelTouchUpdated(TouchWheel* sender, const Point& point, float dt) {}

    };

    /** a touchable wheel will return touch normalized coordinates to the handler*/
    class TouchWheel: public cocos2d::CCNode, public cocos2d::CCTouchDelegate
    {
    public:
        TouchWheel();
        virtual ~TouchWheel();
        bool init(cocos2d::CCSprite* wheel, cocos2d::CCSprite* touch);
        CREATE_FUNC2(TouchWheel,
            cocos2d::CCSprite*, wheel, 
            cocos2d::CCSprite*, touch);
        
        virtual void onEnter();
        virtual void onExit();
         
        bool getIsEnabled() const { return m_enabled; }
        void setEnable(bool var);

	    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent); 

        /** touches in touch radius will be caught by wheel */
        CC_PROPERTY(float, m_touchRadius, TouchRadius);
        /** touches in inner radius will not trigger to event */
        CC_PROPERTY(float, m_innerRadius, InnerRadius);

        CC_PROPERTY(cocos2d::CCSprite*, m_wheelSprite, WheelSprite);
        CC_PROPERTY(cocos2d::CCSprite*, m_touchSprite, TouchSprite);

        CC_SYNTHESIZE(TouchWheelInputHandler*, m_handler, Handler);

        /** get normalized touch point location */
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(Point, m_touchPoint, TouchPoint); 

    protected:
        void registerWithTouchDispatcher();
        void unregisterWithTouchDispatcher();
        bool touchOnLoaction(Vector2 point);
        void touchUpdate(float dt);

    protected:         
        bool m_enabled; 
    };


    /** interface : touch wheel input handler */
    class SlidebarInputHandler
    {
    public:

        /** optional, when slide is began */
        virtual void slideBegan(Slidebar* sender, float progress) {}

        /** optional, when slide is moved */
        virtual void slideMoved(Slidebar* sender, float progress) {}

        /** optional, when slide is ended */
        virtual void slideEnded(Slidebar* sender, float progress) {}

        /** updated, when slide is updated*/
        virtual void slideUpdated(Slidebar* sender, float progress, float dt) {}

    };

    /** general slide bar for displaying scrolling views or adjusting*/
    class Slidebar
    {

    };
}