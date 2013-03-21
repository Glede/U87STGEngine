#include "U87Collision.h"

USING_NS_CC;
USING_NS_U;


HitTestResult* Body::hitTest(const Body* body)
{
    return HitTestResult::create(HTRT_NONE); 
}

///////////////////////////////////////////////////////////////////////////////
// strip
///////////////////////////////////////////////////////////////////////////////

Strip* Strip::create(const Point& begin, const Point& end, float width)
{
    Strip* pRet = new Strip();
    pRet->autorelease();
    pRet->begin = begin;
    pRet->end   = end;
    pRet->width = width;
    return pRet;
}

Strip* Strip::create(const Strip* copy)
{
    Strip* pRet = new Strip();
    pRet->autorelease();
    pRet->begin = copy->begin;
    pRet->end   = copy->end;
    pRet->width = copy->width;
    return pRet;
}


void Strip::setPosition(const Vector2& point, float rotation)      
{
    begin = point;
    end = point;
}

void Strip::updatePosition(const Vector2& point, float rotation)   
{
    begin = end;
    end = point;
}


HitTestResult* Strip::hitTest(const Body* body)
{
    switch(body->shape)
    {
    default:
    case SHAPE_NONE:
        return HitTestResult::create(HTRT_NONE); 
    case SHAPE_STRIP:
        return hitTest((Strip*)body);
    case SHAPE_ROUND:
        return hitTest((Round*)body);
    case SHAPE_RECT:
        return hitTest((Rect*)body);
    } 
}

///////////////////////////////////////////////////////////////////////////////
// round
///////////////////////////////////////////////////////////////////////////////

Round* Round::create(const Point& center, float radius)
{
    Round* pRet = new Round();
    pRet->autorelease();
    pRet->center = center;
    pRet->radius = radius;
    return pRet;
}

Round* Round::create(const Round* copy)
{
    Round* pRet = new Round();
    pRet->autorelease();
    pRet->center = copy->center;
    pRet->radius = copy->radius;
    return pRet;
}


void Round::setPosition(const Vector2& point, float rotation)      
{
    center = point;
}

void Round::updatePosition(const Vector2& point, float rotation)   
{
    center = point;
}

HitTestResult* Round::hitTest(const Body* body)
{
    switch(body->shape)
    {
    default:
    case SHAPE_NONE:
        return HitTestResult::create(HTRT_NONE); 
    case SHAPE_STRIP:
        return hitTest((Strip*)body);
    case SHAPE_ROUND:
        return hitTest((Round*)body);
    case SHAPE_RECT:
        return hitTest((Rect*)body);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Rect
///////////////////////////////////////////////////////////////////////////////

Rect* Rect::create(const Point& center, float width, float height, float angle)
{
    Rect* pRet = new Rect();
    pRet->autorelease();
    pRet->center = center;
    pRet->width  = width;
    pRet->height = height;
    pRet->angle  = angle;
    return pRet;
}

Rect* Rect::create(const Rect* copy)
{
    Rect* pRet = new Rect();
    pRet->autorelease();
    pRet->center = copy->center;
    pRet->width  = copy->width;
    pRet->height = copy->height;
    pRet->angle  = copy->angle;
    return pRet;
}


void Rect::setPosition(const Vector2& point, float rotation)      
{
    center = point;
	angle = rotation;
}

void Rect::updatePosition(const Vector2& point, float rotation)   
{
    center = point;
	angle = rotation;
}

HitTestResult* Rect::hitTest(const Body* body)
{
    switch(body->shape)
    {
    default:
    case SHAPE_NONE:
        return HitTestResult::create(HTRT_NONE); 
    case SHAPE_STRIP:
        return hitTest((Strip*)body);
    case SHAPE_ROUND:
        return hitTest((Round*)body);
    case SHAPE_RECT:
        return hitTest((Rect*)body);
    }
}