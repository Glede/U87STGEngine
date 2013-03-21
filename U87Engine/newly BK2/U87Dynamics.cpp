#include "U87Dynamics.h"
#include "U87MovableObject.h"

USING_NS_CC;
USING_NS_U;



///////////////////////////////////////////////////////////////////////////////
// Action Dynamics bases
///////////////////////////////////////////////////////////////////////////////
 
void ActionDynamics::step(float dt)
{
    MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
    CC_ASSERT(target);
    float rdt = dt * target->getTimeScale();
    update(rdt);
}

void ActionDynamics::startWithTarget(CCNode* pTarget)
{
    CC_ASSERT(dynamic_cast<MovableObject*>(pTarget) != NULL);
    CCAction::startWithTarget(pTarget);
}



bool ActionDynamicsTimeLimted::init(float duration)
{
    if(duration > 0)
        m_duration = duration;
    else
        m_duration = FLT_EPSILON;

    return true;
}

bool ActionDynamicsTimeLimted::isDone()
{
    return m_remain == 0;
}

void ActionDynamicsTimeLimted::step(float dt)
{
    MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
    CC_ASSERT(target);
    float rdt = dt * target->getTimeScale();

    if(rdt > m_remain)
    {
        rdt = m_remain;
        m_remain = 0;
    }
    else
    {
        m_remain -= rdt;
    }

    update(rdt);
}

void ActionDynamicsTimeLimted::startWithTarget(CCNode* pTarget)
{
    ActionDynamics::startWithTarget(pTarget);

    m_remain = m_duration;
}

///////////////////////////////////////////////////////////////////////////////
// Accelerate By
///////////////////////////////////////////////////////////////////////////////
 
bool AccelerateBy::init(float duration, const Vector2& delta, bool related)
{
    ActionDynamicsTimeLimted::init(duration);
    m_related = related;
    m_rate = delta / m_duration;
    return true;
}

void AccelerateBy::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        Vector2 delta = m_rate * rdt;

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
// Spin By
////////////////////////////////////////////////////////////// /////////////////

bool SpinBy::init(float duration, float delta)
{
    ActionDynamicsTimeLimted::init(duration);

    m_rate = delta / m_duration;

    return true;
}
 
void SpinBy::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = rdt * m_rate; 
        
        target->spin(delta);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Turn By
///////////////////////////////////////////////////////////////////////////////

bool TurnBy::init(float duration, float delta)
{
    ActionDynamicsTimeLimted::init(duration);

    m_rate = delta / m_duration;

    return true;
}

void TurnBy::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = rdt * m_rate; 
        
        target->turn(delta);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Rotate By
///////////////////////////////////////////////////////////////////////////////

bool RotateBy::init(float duration, float delta)
{
    ActionDynamicsTimeLimted::init(duration);

    m_rate = delta / m_duration;

    return true;
}
  
void RotateBy::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = rdt * m_rate; 
        
        target->rotate(delta);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Roturn By
///////////////////////////////////////////////////////////////////////////////

bool RoturnBy::init(float duration, float delta)
{
    ActionDynamicsTimeLimted::init(duration);

    m_rate = delta / m_duration;

    return true;
}
  
void RoturnBy::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        float delta = rdt * m_rate; 
        
        target->roturn(delta);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Accelerate To
///////////////////////////////////////////////////////////////////////////////
 

bool AccelerateTo::init(bool stop, const Vector2& velocity, float rate)
{
    m_stop = stop;
    m_velocity = velocity;
    if(rate == 0) 
        m_rate = INFINITE_RATE;
    else
        m_rate = rate;
    m_done = false;
    return true; 
}
  
 
bool AccelerateTo::isDone()
{
    return m_stop && m_done;
}

void AccelerateTo::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        Vector2 targetVel =  ccpRotateByAngle(m_velocity, POINT_ZERO, 
            CC_DEGREES_TO_RADIANS(-target->getRotation()));

        if(target->getVelocity() == targetVel)
        {
            m_done = true;
        }
        else
        {
            float rate; 
            if(m_rate == INFINITE_RATE)
                rate = FLT_MAX;
            else
                rate = rdt * m_rate;

            target->accelerateTo(targetVel, rate);
        }
    }
}

void AccelerateTo::startWithTarget(CCNode* pTarget)
{
    ActionDynamics::startWithTarget(pTarget);

    m_done = false;
}

///////////////////////////////////////////////////////////////////////////////
// Spin To
///////////////////////////////////////////////////////////////////////////////

bool SpinTo::init(bool stop, float velocity, float rate)
{
    m_stop = false;
    m_velocity = velocity;
    if(rate == 0) 
        m_rate = INFINITE_RATE;
    else
        m_rate = rate;
    return true;
}
 

bool SpinTo::isDone()
{
    return m_stop && m_done;
}

void SpinTo::startWithTarget(CCNode *pTarget)
{
    ActionDynamics::startWithTarget(pTarget);
    m_done = false;
}

void SpinTo::update(float rdt)
{
    if(m_pTarget)
    {
        MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
        CC_ASSERT(target != NULL);

        if(target->getRotationalVelocity() == m_velocity)
        {
            m_done = true;
        }
        else
        {
            float rate; 
            if(m_rate == INFINITE_RATE)        
                rate = FLT_MAX;
            else
                rate = rdt * m_rate;
        
            target->spinTo(m_velocity, rate);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Move To  Not Implemented yet
///////////////////////////////////////////////////////////////////////////////
// 
//bool MoveTo::init(const Point& destination, float speed, float accel, float breaking)
//{ 
//    m_destination = destination;
//    m_speed = speed;
//    m_accel = accel;
//    if(breaking > 0)
//        m_breaking = accel;
//    else
//        m_breaking = breaking;
//
//    m_done = false;
//    return true;
//}
//
//bool MoveTo::isDone()
//{
//    return m_done;
//}
//
//void MoveTo::startWithTarget(CCNode* pTarget)
//{
//    ActionDynamics::startWithTarget(pTarget);
//
//    m_lastPos = pTarget->getPosition();
//    m_done = false;
//}
//
//void MoveTo::stop()
//{
//    ActionDynamics::stop();
//    m_done = false;
//}
//
//void MoveTo::update(float rdt)
//{
//    MovableObject* target = dynamic_cast<MovableObject*>(m_pTarget);
//    CC_ASSERT(target);
//
//    Point current = target->getPosition();
//    if(isPointBetween(m_destination, m_lastPos, current))
//    {
//        m_done = true;
//    }
//    else // continue moving
//    {
//        Point deltaDirection = ccpNormalize(m_destination - current);
//        float distance = ccpDistance(m_destination, current);
//        float approachingSpeed = ccpDot(target->getVelocity(), deltaDirection);
//        
//        if(approachingSpeed * rdt > distance)
//        {
//            target->accelerate(-deltaDirection * m_breaking * rdt);            
//        }
//        else if(approachingSpeed < m_speed)
//        {
//            target->accelerateTo(deltaDirection * m_speed, m_accel * rdt);
//        }
//    }
//}