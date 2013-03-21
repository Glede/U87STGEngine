#include "U87Collision.h"
#include "U87World.h"
#include <algorithm>

USING_NS_CC;
USING_NS_U;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Body Node
///////////////////////////////////////////////////////////////////////////////

bool BodyNode::s_visible = false;

bool BodyNode::init(BodyType type)
{
    m_type = type; 
    m_CTag = CLT_NONE;
    m_vRadius = 0;
    m_vCenter = ccp(0,0);

    scheduleUpdate();

    return true;
}

void BodyNode::update(float dt)
{ 
    updateState(dt);
    updateCollision(dt);
}


// update collision test
void BodyNode::updateCollision(float dt)
{  
    CCNode* node = this; 
 
    // if not active, be quiet
    if(m_active == false || m_gameUnit == NULL)
        return;

    CCArray* targetCache = getTargetCache();
    BodyNode* target; 
    std::vector<HitRecord> hitCache;
    CCARRAY_TFOREACH(targetCache, target, BodyNode*)
    { 
        // inactive object
        if(target->isActive() == false) 
            continue;

        // no collision test with object itself
        if(target == this)  
            continue;

        HitTestResult* htr = hitTest(target);
        if(htr->isHit())
        {
            hitCache.push_back(HitRecord(target, htr->crossPoint, htr->distance));
        } 

        // release space 
        delete htr;
    } 

    // hit objects
    if(!hitCache.empty())
    {
        std::sort(hitCache.begin(), hitCache.end());

        for(unsigned int i = 0; i < hitCache.size(); i++)
        {
            CC_BREAK_IF(m_gameUnit->onCollision(hitCache[i]));
        }
    }
}


// remove from orginal tag, and add index to the new tag
void BodyNode::setCTag(CTag tag)
{
    if(m_CTag != CLT_NONE)
        World::sharedInstance()->getTargetIndex(m_CTag)->fastRemoveObject(this);

    m_CTag = tag;    
    World::sharedInstance()->getTargetIndex(m_CTag)->addObject(this);
}

/** set target type. end with UT_NONE.
    this method works with the default getTargets() method.*/
void BodyNode::setTargetCTag(CTag tag, ...)
{
    // clear last target type and reset types
    m_targetCTag.clear();

	va_list params;
	va_start(params, tag);
    
    CTag ut = tag;
    while(ut != CLT_NONE)
    {
        addTargetCTag(ut);
        ut = va_arg(params, CTag);
    }

    va_end(params); 
}

void BodyNode::addTargetCTag(CTag tag)
{
    CTagList::iterator it;
    it = find(m_targetCTag.begin(), m_targetCTag.end(), tag);
    if(it == m_targetCTag.end())
        m_targetCTag.push_back(tag);
}

void BodyNode::removeTargetCTag(CTag tag)
{
    CTagList::iterator it;
    it = find(m_targetCTag.begin(), m_targetCTag.end(), tag);
    if(it != m_targetCTag.end())
        m_targetCTag.erase(it);
}



CCArray* BodyNode::getTargetCache()
{
    CCArray* pRet = CCArray::array();
    World* world = World::sharedInstance();
    UINT count = m_targetCTag.size();

    for(UINT i = 0; i < count; i++)
    {
        CCArray* targets = world->getTargetIndex(m_targetCTag[i]);
        pRet->addObjectsFromArray(targets);
    }

    return pRet;
}

///////////////////////////////////////////////////////////////////////////////
// Round Body Node
///////////////////////////////////////////////////////////////////////////////

bool RoundBodyNode::init(float radius)
{
    BodyNode::init(BODY_ROUND);

    CC_ASSERT(radius >= 0);
    m_radius = radius;


    return true;
}

void RoundBodyNode::updateState(float dt)
{ 
    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform = CCAffineTransformIdentity;
    
    m_vRadius = m_radius;
    for(CCNode* p = this; p != world; p = p->getParent())
    {
        CC_ASSERT(p != NULL);
        m_vRadius *= p->getScale();
        transform = CCAffineTransformConcat(transform, p->nodeToParentTransform());
    }
    
    m_vCenter = CCPointApplyAffineTransform(POINT_ZERO, transform);
}


///////////////////////////////////////////////////////////////////////////////
// Strip Body Node 
///////////////////////////////////////////////////////////////////////////////

bool StripBodyNode::init(float width)
{
    BodyNode::init(BODY_STRIP);

    CC_ASSERT(width >= 0);
    m_width = width;
    m_updateBegin = true;

    return true;
}

void StripBodyNode::updateState(float dt)
{ 
    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform = CCAffineTransformIdentity;
    float scale = getScale();
    m_vWidth = m_width;

    for(CCNode* p = this->getParent(); p != world; p = p->getParent())
    {
        CC_ASSERT(p != NULL);
        scale *= p->getScale();
        transform = CCAffineTransformConcat(transform, p->nodeToParentTransform());
    }
    
    Point vEnd = CCPointApplyAffineTransform(m_tPosition, transform);
    if(m_updateBegin)
    {
        m_vBegin = vEnd;
        m_vEnd = vEnd;
        m_updateBegin = false;
    }
    else
    {
        if(m_vEnd != vEnd)
            m_vBegin = m_vEnd;
        m_vEnd = vEnd;
    }

    m_vWidth *= scale;
    m_vRadius = sqrt(ccpDistanceSQ(m_vBegin, m_vEnd) + sq(m_vWidth/2));
    m_vCenter = (m_vBegin + m_vEnd)/2;
}


void StripBodyNode::setPosition(const Point& position)
{
    CCNode::setPosition(position);
    CCAffineTransform transform = CCAffineTransformIdentity;

    m_updateBegin = true;
}

void StripBodyNode::getTops(Point* topA, Point* topB, Point* topC, Point* topD)
{
    Vector2 v = ccpNormalize(m_vEnd - m_vBegin);
    v = ccp(v.y, -v.x) * m_vWidth / 2;
    *topA = m_vBegin + v;
    *topB = m_vBegin - v;
    *topC = m_vEnd - v;
    *topD = m_vEnd + v;
}


///////////////////////////////////////////////////////////////////////////////
// Rect Body Node 
///////////////////////////////////////////////////////////////////////////////

bool RectBodyNode::init(float width, float height)
{
    BodyNode::init(BODY_RECT);

    CC_ASSERT(width >= 0 && height >= 0);

    m_width = width;
    m_height = height;

    return true;
}

void RectBodyNode::updateState(float dt)
{ 
    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform = CCAffineTransformIdentity;
    m_vAngle = 0;
    m_vWidth = m_width;
    m_vHeight = m_height;

    for(CCNode* p = this; p != world; p = p->getParent())
    {
        CC_ASSERT(p != NULL);
        m_vWidth *= p->getScale();
        m_vHeight *= p->getScale();
        m_vAngle += p->getRotation();
        transform = CCAffineTransformConcat(transform, p->nodeToParentTransform());
    }
    
    m_vCenter = CCPointApplyAffineTransform(POINT_ZERO, transform);
    m_vRadius = sqrt(sq(m_vWidth) + sq(m_vHeight))/2;
}




void RectBodyNode::getTops(Point* topA, Point* topB, Point* topC, Point* topD)
{
    World* world = World::sharedInstance();
    CCAffineTransform transform = CCAffineTransformIdentity; 
    for(CCNode* p = this; p != world; p = p->getParent())
    {
        CC_ASSERT(p != NULL);
        transform = CCAffineTransformConcat(transform, p->nodeToParentTransform());
    }
    *topA = ccp(m_width/2, m_height/2);
    *topB = ccp(-m_width/2, m_height/2);
    *topC = ccp(-m_width/2, -m_height/2);
    *topD = ccp(m_width/2, -m_height/2);
    *topA = CCPointApplyAffineTransform(*topA, transform);
    *topB = CCPointApplyAffineTransform(*topB, transform);
    *topC = CCPointApplyAffineTransform(*topC, transform);
    *topD = CCPointApplyAffineTransform(*topD, transform);
}


////////////////////////////////////////////////////////////////////////////////////////////
// draw fucntions
/////////////////////////////////////////////////////////////////////////////////////////////

void StripBodyNode::draw()
{  
    if(s_visible)
    {
        if(m_updateBegin)
            return;

        Point topA, topB, topC, topD;
        Point last = World::sharedInstance()->convertToWorldSpace(m_vBegin);
        last = convertToNodeSpace(last);

        Vector2 v = ccpNormalize(last);
        v = ccp(v.y, -v.x) * m_width / 2;
        topA = v;
        topB = -v;
        topC = last - v;
        topD = last + v;
        
        ccDrawLine(topA, topB);
        ccDrawLine(topB, topC);
        ccDrawLine(topC, topD);
        ccDrawLine(topD, topA);
    } 
}



void RectBodyNode::draw()
{  
    if(s_visible)
    {
 
        Point topA, topB, topC, topD;
        topA = ccp(m_width/2, m_height/2);
        topB = ccp(-m_width/2, m_height/2);
        topC = ccp(-m_width/2, -m_height/2);
        topD = ccp(m_width/2, -m_height/2);

        ccDrawLine(topA, topB);
        ccDrawLine(topB, topC);
        ccDrawLine(topC, topD);
        ccDrawLine(topD, topA);
    }

}


void RoundBodyNode::draw()
{ 
    if(s_visible)
    {
        ccDrawCircle(POINT_ZERO, m_radius, 3.1415f, 16, 0);
    }
}
