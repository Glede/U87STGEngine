#include "U87Control.h"

USING_NS_CC;
USING_NS_U;

///////////////////////////////////////////////////////////////////////////////
// Touch Wheel
///////////////////////////////////////////////////////////////////////////////

TouchWheel::TouchWheel()
{ 
    m_wheelSprite = NULL;
    m_touchSprite = NULL;
}

TouchWheel::~TouchWheel()
{

}

bool TouchWheel::init(CCSprite* panel, CCSprite* touch)
{
    bool bRet = false;

    do
    {  
        CC_BREAK_IF(!panel);
   
        m_handler = NULL;

        m_enabled = true;
        m_touchRadius = panel->getContentSize().width/2;
        m_innerRadius = 0;

        setWheelSprite(panel);
        setTouchSprite(touch);

        if(touch)
            touch->setIsVisible(false);

        bRet = true;
    } while(0);

    return bRet;
}

void TouchWheel::onEnter()
{
    if (m_enabled)
	{
		this->registerWithTouchDispatcher();
	}

	CCNode::onEnter();
}

void TouchWheel::onExit()
{
    if(m_enabled)
	{
        this->unregisterScriptHandler();
	}

	CCNode::onExit();
}

void TouchWheel::setEnable(bool var)
{ 
    if(m_enabled == var)
        return;

    m_enabled = var;
	if (m_bIsRunning)
	{
		if (m_enabled)
		{
			this->registerWithTouchDispatcher();
		}
		else
		{ 
            this->unregisterScriptHandler();
		}
	} 
}

bool TouchWheel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touch = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    CCPoint local = this->convertToNodeSpace(touch);
    float distanceSQ = ccpLengthSQ(local);
    if(distanceSQ > sq(m_touchRadius))
        return false;


    if(m_touchSprite)
    {
        m_touchSprite->setIsVisible(true);
    }
    touchOnLoaction(local);
    
    if(m_handler)
        m_handler->wheelTouchBegan(this, m_touchPoint);


    schedule(schedule_selector(TouchWheel::touchUpdate));
    return true;
}

void TouchWheel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touch = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    CCPoint local = this->convertToNodeSpace(touch);
    if(touchOnLoaction(local))
    {
        if(m_handler)
            m_handler->wheelTouchMoved(this, m_touchPoint);
    } 
}

void TouchWheel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touch = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    CCPoint local = this->convertToNodeSpace(touch);

    touchOnLoaction(local);
    if(m_handler)
        m_handler->wheelTouchEnded(this, m_touchPoint);

    unschedule(schedule_selector(TouchWheel::touchUpdate));

    if(m_touchSprite)
    {
        m_touchSprite->setIsVisible(false);
    }
}

void TouchWheel::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

float TouchWheel::getTouchRadius()
{
    return m_touchRadius;
}

void TouchWheel::setTouchRadius(float value)
{
    if(value > 0)
        m_touchRadius = value;
    else
        m_touchRadius = 0;
}

float TouchWheel::getInnerRadius()
{
    return m_innerRadius;
}

void TouchWheel::setInnerRadius(float value)
{
    if(value > 0)
        m_innerRadius = value;
    else
        m_innerRadius = 0;
}

CCSprite* TouchWheel::getWheelSprite()
{
    return m_wheelSprite;
}

void TouchWheel::setWheelSprite(CCSprite* sprite)
{
    CC_ASSERT(sprite != NULL);
    if(m_wheelSprite)
        m_wheelSprite->removeFromParentAndCleanup(true);
    m_wheelSprite = sprite;
    addChild(m_wheelSprite);
}

CCSprite* TouchWheel::getTouchSprite()
{
    return m_touchSprite;
}

void TouchWheel::setTouchSprite(CCSprite* sprite)
{
    if(m_touchSprite)
    {
        m_touchSprite->removeFromParentAndCleanup(true);
    }

    m_touchSprite = sprite;

    if(m_touchSprite)
    {
        addChild(m_touchSprite);
    }
}

// protected method

void TouchWheel::registerWithTouchDispatcher()
{

#if (COCOS2D_VERSION < 0x00020000)
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, TOUCH_PRIORITY_PANEL, true);
#else
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUCH_PRIORITY_PANEL, true);
#endif
}

void TouchWheel::unregisterWithTouchDispatcher()
{
    unschedule(schedule_selector(TouchWheel::touchUpdate));
    if(m_touchSprite)
    {
        m_touchSprite->setIsVisible(false);
    }

#if (COCOS2D_VERSION < 0x00020000)
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
#else
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, TOUCH_PRIORITY_PANEL, true);
#endif
}


bool TouchWheel::touchOnLoaction(Vector2 point)
{ 
    if(ccpLengthSQ(point) >= sq(m_innerRadius))
    {
        if(ccpLengthSQ(point) > sq(m_touchRadius))
            point = ccpNormalize(point)*m_touchRadius;

        if(m_innerRadius > 0)
            m_touchPoint = (point - (ccpNormalize(point)*m_innerRadius))/(m_touchRadius- m_innerRadius);
        else
            m_touchPoint = point/m_touchRadius;
        
        if(m_touchSprite)
        {
            m_touchSprite->setRotation(angleForVector2(point)); 
            m_touchSprite->setPosition(point);
            m_touchSprite->setIsVisible(true);
        }
        return true;
    }
    else 
    {
        m_touchPoint = POINT_ZERO;
        if(m_touchSprite)
        {
            m_touchSprite->setIsVisible(false);
        }
        return false;
    }
}
 
void TouchWheel::touchUpdate(float dt)
{
    if(m_handler)
        m_handler->wheelTouchUpdated(this, m_touchPoint, dt);
}
 