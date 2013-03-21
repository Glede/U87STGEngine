#include "U87MovableObject.h"

USING_NS_CC;
USING_NS_U;


//////////////////////////////////////////////
// movable object
//////////////////////////////////////////////
 
MovableObject::MovableObject()          
{ 
    m_feet = NULL; 
}

MovableObject::~MovableObject() 
{ 
    CC_SAFE_DELETE_ARRAY(m_feet); 
}


MovableObject* MovableObject::create()
{
    MovableObject* pRet = new MovableObject();
    pRet->init();
    pRet->autorelease();
    return pRet;
}

void MovableObject::init()
{
    m_location      = POINT_ZERO;
    m_nextLocation  = POINT_ZERO;
    m_rotation      = 0;
    m_nextRotation  = 0;

    m_velocity                  = VECTOR2_ZERO;
    m_nextVelocity              = VECTOR2_ZERO;
    m_rotationalVelocity        = 0;
    m_nextRotationalVelocity    = 0;
     
    m_resistanceFactor  = 0;
    m_timeScale         = 1;
    m_mass              = 1;

    m_footCount = 0;
    CC_SAFE_DELETE_ARRAY(m_feet);

    schedule(schedule_selector(MovableObject::updateMotion)); 
}

/** Movable objects has does tow steps of updating
 * 1. Calculate the coordinates of next frame to the cache (m_nextBLABLA) using
 * current coordinates state.
 * Waits until other movable objects finished step 1, does steps 2.
 * 2. Check if all feet are on the ground.
 * 3. Update the cache coordinates to current.
 * 
 *
 * In this order will sperate a update method into to parts to call. Here is a 
 * way to restore them into one update method.
 * do step 3 then do step 1,2 in update method.
 *
 */
void MovableObject::updateMotion(float dt)
{
    CC_ASSERT(m_timeScale >= 0);
    float rdt = dt * m_timeScale; // real delta time
    float rrf = pow(1 - m_resistanceFactor, rdt); // real resistance factor

    // step 3
    // update next
    setLocation(m_nextLocation);
    setRotation(m_nextRotation);
    m_velocity = m_nextVelocity;
    m_rotationalVelocity = m_nextRotationalVelocity;
    
    // step 1
    // cache next
    m_nextLocation = m_nextLocation + rdt * m_velocity;
    m_nextRotation += rdt * m_rotationalVelocity;

    m_nextVelocity = m_nextVelocity * rrf;
    m_nextRotationalVelocity *= rrf;

    // step 2
    // check feet on ground
}

void MovableObject::accelerateTo(const Vector2& target, float maxRate)
{
    Vector2 delta = target - m_velocity;
    if(maxRate >= 0 && ccpLengthSQ(delta) > sq(maxRate))
    {
        delta = ccpNormalize(delta) * maxRate;
    }

    accelerate(delta);
}

void MovableObject::accelerateForward(float var)
{
    Vector2 delta;
    if(m_velocity == VECTOR2_ZERO)
        delta = vector2ForRotation(m_rotation) * var;
    else
        delta = ccpNormalize(m_velocity) * var;

    accelerate(delta);
}

void MovableObject::accelerateForwardTo(float target, float maxRate)
{
    float delta = target - ccpLength(m_velocity);
    if(maxRate > 0 && abs(delta) > maxRate)
    {
        delta = delta/abs(delta) * maxRate;
    }

    accelerateForward(delta);
}
 
void MovableObject::force(const Vector2& var)
{
    if(m_mass <= 0)
        return;

    accelerate(var/m_mass);
}

void MovableObject::forceForward(float var)
{
    if(m_mass <= 0)
        return;

    accelerateForward(var/m_mass);
}

void MovableObject::spinTo(float target, float maxRate)
{
    float delta = target - m_rotationalVelocity;
    if(maxRate >= 0 && abs(delta) > maxRate)
    {
        delta = delta/abs(delta) * maxRate;
    }
    
    spin(delta);
}
 
void MovableObject::rotateTo(float target, float maxRate)
{
    float delta = normalizeDegree(target - m_rotation);
    if(maxRate >= 0 && abs(delta) > maxRate)
    {
        delta = delta/abs(delta) * maxRate;
    }
    
    rotate(delta);
}

void MovableObject::turnTo(float target, float maxRate)
{
    float direction = angleForVector2(m_velocity);
    float delta = normalizeDegree(target - direction);
    if(maxRate >= 0 && abs(delta) > maxRate)
    {
        delta = delta/abs(delta) * maxRate;
    }
    
    turn(delta);
}
 
void MovableObject::roturnTo(float target, float maxRate)
{
    rotateTo(target, maxRate);
    turnTo(target, maxRate);
}

void MovableObject::setLocation(const Point& var)
{
    m_location = var;
    m_nextLocation = var;
    CCNode::setPosition(var);
}

void MovableObject::setVelocity(const Point& var)
{
    m_velocity = var;
    m_nextVelocity = var;
}

void MovableObject::setRotationalVelocity(float var)
{
    m_rotationalVelocity = var;
    m_nextRotationalVelocity = var;
}

void MovableObject::setTimeScale(float var)
{
    if(var >= 0)
    {
        m_timeScale = var;


    }
    else 
        CC_ASSERT(false);
}

void MovableObject::setMass(float var)
{
    if(var > 0 || INFINITE_MASS)
        m_mass = var;
    else 
        CC_ASSERT(false);
}

void MovableObject::setResistanceFactor(float var)
{
    if(var >= 0 && var <=1)
        m_resistanceFactor = var;
    else
        CC_ASSERT(false);
}

void MovableObject::setPosition(const cocos2d::CCPoint& var)
{
    m_location = var;
    m_nextLocation = var;
    CCNode::setPosition(var);
}

void MovableObject::setRotation(float var)
{
    m_rotation = var;
    m_nextRotation = var;
    CCNode::setRotation(var);
}
 

/** set target type. end with UT_NONE.
    this method works with the default getTargets() method.*/
void MovableObject::setFeet(Point* point, ...)
{
    // clear last target type and reset types
    CC_SAFE_DELETE_ARRAY(m_feet);
    m_footCount = 0;
    
	va_list params;
	va_start(params, point);

    std::vector<Point> temp;
    
    Point* ut = point;
    int count;
    while(ut != NULL)
    {
        temp.push_back(*ut);
        ut = va_arg(params, Point*);
    }

    count = temp.size();
    m_feet = new Point[count];
    
    for(int i = 0; i < count; i++)
        m_feet[i] = temp[i];
    m_footCount = count;

    va_end(params); 
}

