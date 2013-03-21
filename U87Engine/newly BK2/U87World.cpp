#include "U87World.h"
#include "U87Collision.h"
#include <algorithm>

USING_NS_U;
USING_NS_CC;
using namespace std;
 

World* World::s_sharedInstance = NULL;

/** create world for the first time */ 
World::World()
{
    m_camera = NULL;
    m_gameUnits = CCDictionary::dictionary();
    m_gameUnits->retain();
}

World::~World()
{
    m_gameUnits->release();

    for(ArrayMap::iterator i = m_indexMap.begin(); i != m_indexMap.end(); i++)
    {
        i->second->release();
    }

    if(s_sharedInstance == this)
        s_sharedInstance = NULL;
}
 
bool World::init()
{
    if(s_sharedInstance != NULL)
        return false;

    CCSize win = CCDirector::sharedDirector()->getWinSize();

    m_camera = NULL;
    s_sharedInstance = this;
    m_border = CCRectMake(-win.width, -win.height, win.width*3, win.height*3);
    m_gameUnits->removeAllObjects();
    m_nextAvailableID = 0;

    return true;
}

void World::update(float dt)
{
    // do all the collision test
    for(RuleList::iterator it = m_ruleList.begin(); it != m_ruleList.end(); it++)
    {
        CollisionRule& rule = (*it);
        
        CCArray* ta = CCArray::arrayWithArray(getBodyIndex(rule.tagA));
        CCArray* tb = CCArray::arrayWithArray(getBodyIndex(rule.tagB));
        BodyNode* _a;
        BodyNode* _b;

        CCARRAY_TFOREACH(ta, _a, BodyNode*)
        CCARRAY_TFOREACH(tb, _b, BodyNode*)
        {
            if(collisionTest(_a, _b))
            {
                rule.handler->onCollision(_a, _b);
            }
        }
    }
}

CCArray* World::getBodyIndex(CTag tag)
{
    CCArray* pRet;

    //create a index for the first time 
    if(m_indexMap.find(tag) == m_indexMap.end())
    {
        pRet = CCArray::array();
        pRet->retain();
        m_indexMap[tag] = pRet;
    }
    else
    {
        pRet = m_indexMap[tag];
    }

    return pRet;
}
 
void World::addGameUnit(GameUnit* unit, UnitID id, int z)
{
    // a game unit must be a node
    CCNode* node = dynamic_cast<CCNode*>(unit);
    CC_ASSERT(node != NULL);
    
    if(id == UID_NONE)
        unit->m_unitID = m_nextAvailableID++;
    else
        unit->m_unitID = id;

    CC_ASSERT(m_gameUnits->objectForKey(unit->m_unitID) == NULL);
    m_gameUnits->setObject(node, unit->m_unitID);    
    this->addChild(node, z);
}
 
void World::removeGameUnit(GameUnit* unit)
{
    CCNode* node = dynamic_cast<CCNode*>(unit);
    CC_ASSERT(node != NULL);

    this->removeChild(node, true);
    m_gameUnits->removeObjectForKey(unit->m_unitID);
}

ICamera* World::getWorldCamera()
{
    return m_camera;
}

void World::setWorldCamera(ICamera* cam)
{
    m_camera = cam;
    if(m_camera != NULL)
    {
        schedule(schedule_selector(World::updateFollowCameraNode));
    }
    else
    {
        unschedule(schedule_selector(World::updateFollowCameraNode));
    }
}

void World::updateFollowCameraNode(float dt)
{
    CC_ASSERT(m_camera != NULL);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 

    Point camPos = m_camera->getCameraPosition();
    CCSize camSight = m_camera->getSightSize();
    float camRot = m_camera->getCameraRotation();
    
    setRotation(-camRot);
    setScaleX(winSize.width/camSight.width);
    setScaleY(winSize.height/camSight.height);


    Vector2 sightOffset = ccpFromSize(camSight)/2;
    float rotRadius = CC_DEGREES_TO_RADIANS(-camRot);
    setPosition(sightOffset + ccpRotateByAngle(- camPos, POINT_ZERO, -rotRadius) );
}


void World::addCollisionRule(const CollisionRule& rule)
{
    RuleList::iterator it;
    for(it = m_ruleList.begin(); it != m_ruleList.end(); it++)
    {
        if(it->priority < rule.priority)
            break;
    }
    m_ruleList.insert(it, rule);
}

void World::removeCollisionRule(const CollisionRule& rule)
{
    RuleList::iterator it = find(m_ruleList.begin(), m_ruleList.end(), rule);
    if(it != m_ruleList.end())
    {
        m_ruleList.erase(it);
    }
}

