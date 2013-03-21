#include "U87World.h"

USING_NS_U;
USING_NS_CC;


World* World::s_sharedInstance = NULL;

/** create world for the first time */ 
World::World()
{
    m_camera = NULL;
    m_availableID = 0;
#if (COCOS2D_VERSION > 0x00020000)
    m_gameUnits = new CCDictionary();
#else
    m_gameUnits = new CCDictionary<int>();
#endif  
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

    return true;
}



CCArray* World::getTargetIndex(UnitType type)
{
    CCArray* pRet;

    //create a index for the first time 
    if(m_indexMap.find(type) == m_indexMap.end())
    {
        pRet = CCArray::array();
        pRet->retain();
        m_indexMap[type] = pRet;
    }
    else
    {
        pRet = m_indexMap[type];
    }

    return pRet;
}
 
// make a available id for gameunit, set it in the dictionary
// and add it as a child node
void World::addGameUnit(GameUnit* unit, int z)
{
    CCNode* node = dynamic_cast<CCNode*>(unit);
    CC_ASSERT(node != NULL);
    
    if(unit->getUnitID() == UID_NONE)
    {
        int start = m_availableID;
        while(m_gameUnits->objectForKey(m_availableID) != NULL)
        {
            m_availableID++;
            if(m_availableID >= MAX_ID)
                m_availableID = 0;
            else if(m_availableID == start) // no available id
            {
                CC_ASSERT(false);
                return;
            }
        }
        unit->setUnitID(m_availableID++);
    }
    else
    {
        if(m_gameUnits->objectForKey(unit->getUnitID()) != NULL)
        {
            CC_ASSERT(false);
            return;
        }
    }

    m_gameUnits->setObject(node, unit->getUnitID());
    this->addChild(node, z);
}
 
void World::removeGameUnit(GameUnit* unit)
{
    CCNode* node = dynamic_cast<CCNode*>(unit);
    CC_ASSERT(node != NULL);

    this->removeChild(node, true);
    m_gameUnits->removeObjectForKey(unit->getUnitID());
}

Camera* World::getWorldCamera()
{
    return m_camera;
}

void World::setWorldCamera(Camera* cam)
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
