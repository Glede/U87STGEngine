#include "U87Dynamics.h"
#include "U87MovableObject.h"

USING_NS_CC;
USING_NS_U;



///////////////////////////////////////////////////////////////////////////////
// Action Dynamics
///////////////////////////////////////////////////////////////////////////////
 
void ActionDynamics::step(ccTime dt)
{
    MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
    CC_ASSERT(target);
    float rdt = dt * target->getTimeScale();
    CCActionInterval::step(rdt);
}

///////////////////////////////////////////////////////////////////////////////
// Accelerate By
///////////////////////////////////////////////////////////////////////////////

bool AccelerateBy::init(ccTime duration, const Vector2& velocity, bool related)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_related = related;
        m_delta = velocity;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void AccelerateBy::startWithTarget(CCNode *pTarget)
{
    CC_ASSERT(dynamic_cast<MovableObject*>(pTarget) != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
}

void AccelerateBy::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        Vector2 delta = (time - m_lastUpdate)*m_delta;
        m_lastUpdate = time;
        

        if(!m_related)
        {
            target->accelerate(delta);
        }
        else
        {
            delta = ccpRotateByAngle(delta, POINT_ZERO, CC_DEGREES_TO_RADIANS(-target->getRotation()));
            target->accelerate(delta);
        }
    }
}
 
///////////////////////////////////////////////////////////////////////////////
// Accelerate To
///////////////////////////////////////////////////////////////////////////////

bool AccelerateTo::init(ccTime duration, const Vector2& velocity, bool related)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_related = related;
        m_velocity = velocity;
        m_rate = 0;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 

bool AccelerateTo::init(ccTime duration, const Vector2& velocity, float rate, bool related)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_related = related;
        m_velocity = velocity;
        m_rate = rate;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void AccelerateTo::startWithTarget(CCNode *pTarget)
{
    MovableObject* target = dynamic_cast<MovableObject*>(pTarget);
    CC_ASSERT(target != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
    if(m_rate == 0)
    {
        Vector2 velocity;
        if(!m_related)
            velocity = m_velocity;
        else
            velocity = ccpRotateByAngle(m_velocity, POINT_ZERO, CC_DEGREES_TO_RADIANS(-target->getRotation()));

        float delta = ccpLength(velocity - target->getVelocity());
        m_rate = delta/m_fDuration;
    }
}

void AccelerateTo::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        Vector2 velocity;
        float rate;
        if(m_rate == INFINITE_RATE)        
            rate = INFINITE_RATE;
        else
            rate = (time - m_lastUpdate)*m_fDuration * m_rate;
        m_lastUpdate = time;
        if(!m_related)
            velocity = m_velocity;
        else
            velocity = ccpRotateByAngle(m_velocity, POINT_ZERO, CC_DEGREES_TO_RADIANS(-target->getRotation()));

        target->accelerateTo(velocity, rate);
    }
}
 
///////////////////////////////////////////////////////////////////////////////
// Spin By
///////////////////////////////////////////////////////////////////////////////

bool SpinBy::init(ccTime duration, float velocity)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = velocity;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void SpinBy::startWithTarget(CCNode *pTarget)
{
    CC_ASSERT(dynamic_cast<MovableObject*>(pTarget) != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
}

void SpinBy::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = (time - m_lastUpdate)*m_delta;
        m_lastUpdate = time;
        
        target->spin(delta);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Spin To
///////////////////////////////////////////////////////////////////////////////

bool SpinTo::init(ccTime duration, float velocity, float rate)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_velocity = velocity;
        m_rate = rate;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void SpinTo::startWithTarget(CCNode *pTarget)
{
    MovableObject* target = dynamic_cast<MovableObject*>(pTarget);
    CC_ASSERT(target != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;

    if(m_rate == 0)
    {
        float delta = normalizeDegree(m_velocity - target->getRotationalVelocity());
        m_rate = abs(delta)/m_fDuration;
    }
}

void SpinTo::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float rate; 
        if(m_rate == INFINITE_RATE)        
            rate = INFINITE_RATE;
        else
            rate = (time - m_lastUpdate)*m_fDuration * m_rate;
        
        target->spinTo(m_velocity, rate);

        m_lastUpdate = time;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Turn By
///////////////////////////////////////////////////////////////////////////////

bool TurnBy::init(ccTime duration, float angle)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = angle;
        m_lastUpdate = 0;
        m_timeScale = 0;
        return true;
    }

    return false;
}
 
void TurnBy::startWithTarget(CCNode *pTarget)
{
    MovableObject* target = dynamic_cast<MovableObject*>(pTarget);
    CC_ASSERT(target != NULL);

    m_timeScale = target->getTimeScale();

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
}

void TurnBy::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = (time - m_lastUpdate)*m_delta;
        m_lastUpdate = time;
        
        target->turn(delta);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Rotate By
///////////////////////////////////////////////////////////////////////////////

bool RotateBy::init(ccTime duration, float angle)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = angle;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void RotateBy::startWithTarget(CCNode *pTarget)
{
    CC_ASSERT(dynamic_cast<MovableObject*>(pTarget) != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
}

void RotateBy::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = (time - m_lastUpdate)*m_delta;
        m_lastUpdate = time;
        
        target->rotate(delta);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Roturn By
///////////////////////////////////////////////////////////////////////////////

bool RoturnBy::init(ccTime duration, float angle)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = angle;
        m_lastUpdate = 0;
        return true;
    }

    return false;
}
 
void RoturnBy::startWithTarget(CCNode *pTarget)
{
    CC_ASSERT(dynamic_cast<MovableObject*>(pTarget) != NULL);

    CCActionInterval::startWithTarget(pTarget);
    m_lastUpdate = 0;
}

void RoturnBy::update(ccTime time)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = (time - m_lastUpdate)*m_delta;
        m_lastUpdate = time;
        
        target->roturn(delta);
    }
}