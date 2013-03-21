#include "U87Geometry.h"
#include "U87World.h"

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
    scheduleUpdate();

    return true;
}

void BodyNode::update(float dt)
{
    CC_UNUSED_PARAM(dt); 
}

void BodyNode::addHitRecord(BodyNode* body)
{
    CC_ASSERT(body != NULL);
    std::vector<BodyNode*>::iterator it;
    for(it == m_hitRecord.begin(); it != m_hitRecord.end(); it++ )
    {
        if(*it == body)
            return;
    }
    m_hitRecord.push_back(body);
}


/** set target type. end with UT_NONE.
    this method works with the default getTargets() method.*/
void BodyNode::setCTag(CTag tag, ...)
{
    // clear last target type and reset types
    World* world = World::sharedInstance();
    std::vector<CTag>::iterator it;
    for(it = m_CTag.begin(); it != m_CTag.end(); it++)
    {
        world->getBodyIndex(*it)->fastRemoveObject(this);
    }
    m_CTag.clear();


	va_list params;
	va_start(params, tag);
    
    CTag ut = tag;
    while(ut != CT_NONE)
    {
        addCollisionTag(ut);
        ut = va_arg(params, CTag);
    }

    va_end(params); 
}

// prevent for indexing twice
void BodyNode::addCollisionTag(CTag tag)
{
    vector<CTag>::iterator it;
    it = find(m_CTag.begin(), m_CTag.end(), tag);
    if(it == m_CTag.end())
    {
        m_CTag.push_back(tag);
        World::sharedInstance()->getBodyIndex(tag)->addObject(this);
    }
}

void BodyNode::removeCollisionTag(CTag tag)
{
    vector<CTag>::iterator it;
    it = find(m_CTag.begin(), m_CTag.end(), tag);
    if(it != m_CTag.end())
    {
        m_CTag.erase(it);
        World::sharedInstance()->getBodyIndex(tag)->fastRemoveObject(this);
    }
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

void RoundBodyNode::update(float dt)
{
    BodyNode::update(dt);
    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform;
    
    m_vRadius = m_radius;
    for(CCNode* p = this; p != world; p = p->getParent())
    {
        m_vRadius *= p->getScale();
        transform = CCAffineTransformConcat(transform, p->nodeToWorldTransform());
    }
    
    m_vCenter = CCPointApplyAffineTransform(POINT_ZERO, transform);

    m_vCCRadius = m_vRadius;
}


void RoundBodyNode::draw()
{ 
    if(s_visible)
    {
        if(m_hitRecord.empty())
            ccDrawColor4B(255,255,255,155);
        else
            ccDrawColor4B(255,0,0,155);
        ccDrawCircle(m_vCenter, m_vRadius, 3.1415f, 16, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Strip Body Node 
///////////////////////////////////////////////////////////////////////////////

bool StripBodyNode::init(float width)
{
    BodyNode::init(BODY_STRIP);

    CC_ASSERT(width >= 0);
    m_width = width;


    return true;
}

void StripBodyNode::update(float dt)
{
    BodyNode::update(dt);

    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform;
    float scale = getScale();
    m_vWidth = m_width;

    for(CCNode* p = this->getParent(); p != world; p = p->getParent())
    {
        scale *= p->getScale();
        transform = CCAffineTransformConcat(transform, p->nodeToWorldTransform());
    }
    
    m_begin = CCPointApplyAffineTransform(m_last, transform);
    m_end = CCPointApplyAffineTransform(m_tPosition, transform);

    m_last = m_tPosition;

    m_vWidth *= scale;
    m_vCCRadius = sqrt(ccpDistanceSQ(m_begin, m_end) + sq(m_vWidth/2));
    m_vCenter = (m_begin + m_end)/2;
}


void StripBodyNode::setPosition(const Point& position)
{
    CCNode::setPosition(position);
    m_last = position; 
}

void StripBodyNode::draw()
{ 
    if(s_visible)
    {
        if(m_hitRecord.empty())
            ccDrawColor4B(255,255,255,155);
        else
            ccDrawColor4B(255,0,0,155);

        Point topA, topB, topC, topD;
        getTops(&topA, &topB, &topC, &topD);

        ccDrawLine(topA, topB);
        ccDrawLine(topB, topC);
        ccDrawLine(topC, topD);
        ccDrawLine(topD, topA);
    }
}

void StripBodyNode::getTops(Point* topA, Point* topB, Point* topC, Point* topD)
{
    Vector2 v = ccpNormalize(m_end - m_begin);
    v = ccp(v.y, -v.x) * 2/m_vWidth;
    *topA = m_begin + v;
    *topB = m_begin - v;
    *topC = m_end - v;
    *topD = m_end + v;
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

void RectBodyNode::update(float dt)
{
    BodyNode::update(dt);
    CC_UNUSED_PARAM(dt);
    World* world = World::sharedInstance();
    CCAffineTransform transform;
    m_vAngle = 0;
    m_vWidth = m_width;
    m_vHeight = m_height;

    for(CCNode* p = this; p != world; p = p->getParent())
    {
        m_vWidth *= p->getScale();
        m_vHeight *= p->getScale();
        m_vAngle += p->getRotation();
        transform = CCAffineTransformConcat(transform, p->nodeToWorldTransform());
    }
    
    m_vCenter = CCPointApplyAffineTransform(POINT_ZERO, transform);
}


void RectBodyNode::draw()
{ 
    if(s_visible)
    {
        if(m_hitRecord.empty())
            ccDrawColor4B(255,255,255,155);
        else
            ccDrawColor4B(255,0,0,155);

        Point topA, topB, topC, topD;
        getTops(&topA, &topB, &topC, &topD);

        ccDrawLine(topA, topB);
        ccDrawLine(topB, topC);
        ccDrawLine(topC, topD);
        ccDrawLine(topD, topA);
    }
}


void RectBodyNode::getTops(Point* topA, Point* topB, Point* topC, Point* topD)
{
    World* world = World::sharedInstance();
    CCAffineTransform transform; 
    for(CCNode* p = this; p != world; p = p->getParent())
    {
        if(p == NULL)
            return;
        transform = CCAffineTransformConcat(transform, p->nodeToWorldTransform());
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

