#include "U87GameUnit.h"
#include "U87World.h"
#include <algorithm>

USING_NS_CC;
USING_NS_U;
using namespace std;
 
///////////////////////////////////////////////////////////////////////////////
// GameUnit
///////////////////////////////////////////////////////////////////////////////
  
GameUnit::GameUnit()
{
    m_unitType = UT_NONE;
    m_unitID = UID_NONE;
    m_inWorld = false;
}

GameUnit::~GameUnit()
{ 
    if(m_inWorld)
        removeFromWorld();
}

void GameUnit::addToWorld(int id, int z)
{
    if(m_inWorld)
    {
        CC_ASSERT(false && m_inWorld);
        return;
    }
    World* world = World::sharedInstance(); 
    world->addGameUnit(this, id, z);
    m_inWorld = true;
}

void GameUnit::removeFromWorld()
{
    if(m_inWorld == false)
    {
        CC_ASSERT(false && m_inWorld);
        return;
    }

    World* world = World::sharedInstance(); 
    world->removeGameUnit(this);
    m_inWorld = false;
}
 

///////////////////////////////////////////////////////////////////////////////
// Camera Node
///////////////////////////////////////////////////////////////////////////////

bool CameraNode::init()
{ 
    MovableObject::init();

    m_location = POINT_ZERO;
    
    m_shockAmplitude    = 0;
    m_shockYScale       = 0.3f;
    m_shockDirection    = 0;

    m_sightSize             = CCDirector::sharedDirector()->getWinSize();

    m_shockMaxAmplitude     = 100.0f;
    m_shockEpsilonAmplitude = 0.0001f;
    m_shockLargeReduceRate  = 0.85f;
    m_shockTinyReduceRate   = 0.91f;
    m_shockReduceRateBorder = 15.0f;

    return true;
}

void CameraNode::shock(float amplitude)
{
    if(amplitude > 0)
    {
        m_shockAmplitude += amplitude; 
        if(m_shockAmplitude > m_shockMaxAmplitude)
            m_shockAmplitude = m_shockMaxAmplitude;

        schedule(schedule_selector(CameraNode::updateShock));
    }
}

void CameraNode::updateShock(float dt)
{
    // update offset
    if(m_shockAmplitude != 0)
    {
        m_shockOffset = ccp(
            m_shockAmplitude * sinf(3.1415f/2 * m_shockDirection),
            m_shockAmplitude * sinf(3.1415f/2 *( 2 * m_shockDirection + 1 ) * m_shockYScale)            
            );
    }
    else 
    {
        m_shockOffset = VECTOR2_ZERO;
    }

    // decline
    if(m_shockAmplitude > m_shockReduceRateBorder)
    {
        m_shockAmplitude *= m_shockLargeReduceRate;
    } 
    else 
    {
        m_shockAmplitude *= m_shockTinyReduceRate;
    }

    if(m_shockAmplitude < m_shockEpsilonAmplitude)
    {
        m_shockAmplitude = 0;
        unschedule(schedule_selector(CameraNode::updateShock));
    }

    m_shockDirection++;    
}
 
void CameraNode::setAsWorldCamera()
{
    World::sharedInstance()->setWorldCamera(this);
}


///////////////////////////////////////////////////////////////////////////////
//  Target Unit
///////////////////////////////////////////////////////////////////////////////
//
//TargetUnit::TargetUnit()
//{
//    m_unitType          = 0;
//    m_active            = false;
//    m_body              = NULL;
//}
//
//TargetUnit::~TargetUnit()
//{ 
//    CC_SAFE_RELEASE(m_body);
//}
//
//
//void TargetUnit::init()
//{ 
//    MovableObject::init();
//
//    m_unitType = 0;
//    m_active = false; 
//    m_firstUpdateCollision = false;
//
//    setBody(NULL);
//
//    schedule(schedule_selector(TargetUnit::updateCollisionTest));
//}
//
//void TargetUnit::addToWorld(int z)
//{
//    CCNode* node = this;
//    CC_ASSERT(node != NULL);
//
//    GameUnit::addToWorld(z); 
//    World* world = World::sharedInstance();
//    vector<CollisionTag>::iterator it;
//    for(it = m_CTag.begin(); it != m_CTag.end(); it++)
//    {
//        world->getTargetIndex(*it)->addObject(this);
//    } 
//    world->getTargetIndex(m_unitType)->addObject(this);
//    setActive(true);
//}
//
//void TargetUnit::removeFromWorld()
//{
//    CCNode* node = this;
//    CC_ASSERT(node != NULL);
//
//    setActive(false);
//    World* world = World::sharedInstance();
//    vector<CollisionTag>::iterator it;
//    for(it = m_CTag.begin(); it != m_CTag.end(); it++)
//    {
//        world->getTargetIndex(*it)->fastRemoveObject(this);
//    } 
//    world->getTargetIndex(m_unitType)->fastRemoveObject(this);
//    GameUnit::removeFromWorld(); 
//}
//
//// update collision test
//void TargetUnit::updateCollisionTest(float dt)
//{  
//    CCNode* node = this;
//    CC_ASSERT(node != NULL);
//
//    // update first body place
//    if(m_body)
//    {
//        if(m_firstUpdateCollision)
//        {
//            m_firstUpdateCollision = false;
//            m_body->setPosition(node->getPosition(), node->getRotation());
//        }
//        else
//            m_body->updatePosition(node->getPosition(), node->getRotation());
//    }
//    
//    // if not active, be quiet
//    if(m_active == false || m_body == NULL)
//        return;
//
//    CCArray* targetCache = getTargetCache();
//    TargetUnit* target; 
//    std::vector<HitRecord> hitCache;
//    CCARRAY_TFOREACH(targetCache, target, TargetUnit*)
//    { 
//        if(target == this)  // no collision test with object itself
//            continue;
//        HitTestResult* htr = m_body->hitTest(target->getBody());
//        if(htr->isHit())
//        {
//            hitCache.push_back(HitRecord(target, htr->crossPoint, htr->distance));
//        } 
//
//        // release space
//        delete htr;
//    } 
//
//    // hit objects
//    if(!hitCache.empty())
//    {
//        std::sort(hitCache.begin(), hitCache.end());
//
//        for(unsigned int i = 0; i < hitCache.size(); i++)
//        {
//            CC_BREAK_IF(collisionWith(hitCache[i].target, hitCache[i].hitPoint));
//        }
//    }
//}
//
//
///** set target type. end with UT_NONE.
//    this method works with the default getTargets() method.*/
//void TargetUnit::setCollisionTag(CollisionTag tag, ...)
//{
//    // clear last target type and reset types
//    if(getParent() != NULL)
//    {
//        World* world = World::sharedInstance();
//        std::vector<CollisionTag>::iterator it;
//        for(it = m_CTag.begin(); it != m_CTag.end(); it++)
//        {
//            world->getTargetIndex(*it)->fastRemoveObject(this);
//        }
//    }
//    m_CTag.clear();
//
//
//	va_list params;
//	va_start(params, tag);
//    
//    CollisionTag ut = tag;
//    while(ut != CT_NONE)
//    {
//        addCollisionTag(ut);
//        ut = va_arg(params, CollisionTag);
//    }
//
//    va_end(params); 
//}
//
//// prevent for tag twice
//void TargetUnit::addCollisionTag(CollisionTag tag)
//{
//    vector<CollisionTag>::iterator it;
//    it = find(m_CTag.begin(), m_CTag.end(), tag);
//    if(it == m_CTag.end())
//    {
//        m_CTag.push_back(tag);
//        if(getParent() != NULL) // is in world
//        {
//            World::sharedInstance()->getTargetIndex(tag)->addObject(this);
//        }
//    }
//}
//
//void TargetUnit::removeCollisionTag(CollisionTag tag)
//{
//    vector<CollisionTag>::iterator it;
//    it = find(m_CTag.begin(), m_CTag.end(), tag);
//    if(it != m_CTag.end())
//    {
//        m_CTag.erase(it);
//        if(getParent() != NULL) // is in world
//        {
//            World::sharedInstance()->getTargetIndex(tag)->fastRemoveObject(this);
//        }
//    }
//}
//
//
///** set target type. end with UT_NONE.
//    this method works with the default getTargets() method.*/
//void TargetUnit::setTargetCollisionTag(CollisionTag tag, ...)
//{
//    // clear last target type and reset types
//    m_targetCTag.clear();
//
//	va_list params;
//	va_start(params, tag);
//    
//    CollisionTag ut = tag;
//    while(ut != CT_NONE)
//    {
//        addTargetCollisionTag(ut);
//        ut = va_arg(params, CollisionTag);
//    }
//
//    va_end(params); 
//}
//
//void TargetUnit::addTargetCollisionTag(CollisionTag tag)
//{
//    vector<CollisionTag>::iterator it;
//    it = find(m_targetCTag.begin(), m_targetCTag.end(), tag);
//    if(it == m_targetCTag.end())
//        m_targetCTag.push_back(tag);
//}
//
//void TargetUnit::removeTargetCollisionTag(CollisionTag tag)
//{
//    vector<CollisionTag>::iterator it;
//    it = find(m_targetCTag.begin(), m_targetCTag.end(), tag);
//    if(it != m_targetCTag.end())
//        m_targetCTag.erase(it);
//}
//
//// get body method controls active body to be tested/
//// if not active, BODY_NONE will be tested (never cross)
//Body* TargetUnit::getBody()
//{
//    if(m_active == false || m_body == NULL)
//        return BODY_NONE;
//
//    return m_body;
//}
//
//void TargetUnit::setBody(Body* body)
//{
//    CC_SAFE_RELEASE(m_body);
//    m_body = body;
//    m_firstUpdateCollision = false;
//    CC_SAFE_RETAIN(m_body);
//}
//
//CCArray* TargetUnit::getTargetCache()
//{
//    CCArray* pRet = CCArray::array();
//    World* world = World::sharedInstance();
//    UINT count = m_targetCTag.size();
//    for(UINT i = 0; i < count; i++)
//    {
//        CCArray* targets = world->getTargetIndex(m_targetCTag[i]);
//        pRet->addObjectsFromArray(targets);
//    }
//
//    return pRet;
//}