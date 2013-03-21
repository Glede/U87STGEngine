//
//  Collision.h
//  ADGameScene
//
//  Created by Glede on Oct 1st 2012
//  Supplemented by CaiyZ(Mr.IroN)
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "U87Math.h"
#include <cmath>
#include <iostream>


namespace u87
{
    class HitTestResult;
    class Body;
    class Round;
    class Strip;
    class Rect;

    ///////////////////////////////////////////////////////////////////////////
    // shapes geometries 
    ///////////////////////////////////////////////////////////////////////////

    enum BodyShape
    {
        SHAPE_NONE,
        SHAPE_STRIP,
        SHAPE_ROUND,
        SHAPE_RECT,
    };
    
    /** Body for collision */
    class Body: public cocos2d::CCObject
    {
    public:
        Body():shape(SHAPE_NONE) { } // constructor for none shape
        virtual ~Body(void) {}

        virtual HitTestResult* hitTest(const Body* body);
	
        virtual void setPosition(const Vector2& point, float rotation)      {}
        virtual void updatePosition(const Vector2& point, float rotation)   {}  

    public:
        const BodyShape shape;
    protected:
        Body(BodyShape bodyShape):shape(bodyShape) { } // constructor for sub class
    };

    Body* const BODY_NONE = new Body();

    /* Strip body */
    class Strip: public Body
    {
    public:
        Strip(void):Body(SHAPE_STRIP) { width = 0; }
        virtual ~Strip(void) {}

        static Strip* create(const Point& begin, const Point& end, float width);
        static Strip* create(const Strip* copy);
        
        virtual void setPosition(const Vector2& point, float rotation);
        virtual void updatePosition(const Vector2& point, float rotation);
        
        virtual HitTestResult* hitTest(const Body* target);
        
        HitTestResult* hitTest(const Strip* target);
        HitTestResult* hitTest(const Round* target);
        HitTestResult* hitTest(const Rect* target);

    public:
        Point begin;
        Point end;
        float width;
    };

    /** Round body */
	class Round : public Body
	{    
    public:
		Round(void):Body(SHAPE_ROUND)  { radius = 0; }
        virtual ~Round(void) {}

        static Round* create(const Point& center, float radius);
        static Round* create(const Round* copy);

        virtual void setPosition(const Vector2& point, float rotation);
        virtual void updatePosition(const Vector2& point, float rotation);
        
        virtual HitTestResult* hitTest(const Body* target);

        HitTestResult* hitTest(const Strip* target);
        HitTestResult* hitTest(const Round* target);
        HitTestResult* hitTest(const Rect* target);
        
    public:
        Point center;
        float radius;
	};

    /** Rectanle body, angle in degrees */
    class Rect: public Body
    {
    public:
        Rect(void):Body(SHAPE_RECT) {}
        virtual ~Rect() {}

        static Rect* create(const Point& center, float width, float height, float angle);
        static Rect* create(const Rect* copy);

        virtual void setPosition(const Vector2& point, float rotation);
        virtual void updatePosition(const Vector2& point, float rotation);
        
        virtual HitTestResult* hitTest(const Body* target);

        HitTestResult* hitTest(const Strip* target);
        HitTestResult* hitTest(const Round* target);
        HitTestResult* hitTest(const Rect* target);
                
    public:
        Point center;
        float width;
        float height;
        float angle;
    };

    ///////////////////////////////////////////////////////////////////////////
    // hit test result;
    ///////////////////////////////////////////////////////////////////////////


    /** Type of Hit Test Result 
     * HTRT_NONE    no intersection
     * HTRT_CONTACT intersect on border
     * HTRT_CROSS   intersect cross
     */
    typedef unsigned int HitTestResultType;
    const HitTestResultType HTRT_NONE     = 0;
    const HitTestResultType HTRT_CONTACT  = 1;
    const HitTestResultType HTRT_CROSS    = 3;

    /** Result of a Hit Test , pass back intersect points*/
    class HitTestResult: public cocos2d::CCObject
    {
    public:
        HitTestResult(HitTestResultType resultType, const Point& point = POINT_ZERO, const float dist = 0.0)
            :resultType(resultType)
            ,crossPoint(point)
			,distance(dist)
        {  }

        bool isHit(void)    { return resultType != HTRT_NONE;    }
        bool isCross(void)  { return resultType == HTRT_CROSS;   }

        inline static HitTestResult* create(HitTestResultType type, const Point& point = cocos2d::CCPointZero, const float dist = 0.0)
        {
            HitTestResult* pRet = new HitTestResult(type, point, dist);
            pRet->autorelease();
            return pRet;
        }

    public:
        const HitTestResultType resultType;
        const Point crossPoint;
		const float distance;
    }; 
}

