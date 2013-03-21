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
}

GameUnit::~GameUnit()
{  

}

void GameUnit::addToWorld()
{  
    addToWorld(0);
}

void GameUnit::addToWorld(int z)
{   
    World* world = World::sharedInstance(); 
    
    world->addGameUnit(this, z);
}

void GameUnit::removeFromWorld()
{ 
    World* world = World::sharedInstance();
     
    world->removeGameUnit(this);
}

BodyNode* GameUnit::addBody(BodyNode* body)
{
    CC_ASSERT(body != NULL);

    body->setGameUnit(this);
    return body;
}

BodyNode* GameUnit::addBody(BodyNode* body, CTag self, CTag target, ...)
{
    CC_ASSERT(body != NULL);

    body->setGameUnit(this);
    body->setCTag(self);
    body->setTargetCTag(CLT_NONE);

	va_list params;
	va_start(params, target);
    
    CTag tag = target;
    while(tag != CLT_NONE)
    {
        body->addTargetCTag(tag);
        tag = va_arg(params, CTag);
    }

    va_end(params); 

    return body;
}
 

BodyNode* GameUnit::addBody(BodyNode* body, const Point& position, float rotation,
                            CTag self, CTag target, ...)
{
    CC_ASSERT(body != NULL);

    body->setGameUnit(this);
    body->setPosition(position);
    body->setRotation(rotation);
    body->setCTag(self);
    body->setTargetCTag(CLT_NONE);

	va_list params;
	va_start(params, target);
    
    CTag tag = target;
    while(tag != CLT_NONE)
    {
        body->addTargetCTag(tag);
        tag = va_arg(params, CTag);
    }

    va_end(params); 

    return body;
}
 