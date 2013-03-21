//
//  Math.h
//  U87Engine
//
//  Created by Glede on Oct 1st 2012
//  Supplemented by CaiyZ(Mr.IroN)
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "U87Util.h"

namespace u87
{
    typedef cocos2d::CCPoint Vector2;
    typedef cocos2d::CCPoint Point;
    const Vector2 POINT_ZERO = cocos2d::CCPointZero;
    const Vector2 VECTOR2_ZERO = cocos2d::CCPointZero;
	const int POS_ON_THE_LEFT = 1;
	const int POS_ON_THE_LINE = 0;
	const int POS_ON_THE_RIGHT = -1;
     

    struct Segment
    {
        Segment();
        Segment(const Point& begin, const Point& end);

        Point begin;
        Point end;
    };

	inline Vector2 operator + (const Vector2& v1, const Vector2 v2)
    {
        return ccp(v1.x + v2.x, v1.y + v2.y);
    }

	inline Vector2 operator - (const Vector2& v1, const Vector2 v2)
    {
        return ccp(v1.x - v2.x, v1.y - v2.y);
    }

	inline Vector2 operator - (const Vector2& v)
    {
		return ccp(-v.x, -v.y);
    }

    inline Vector2 operator * (const Vector2& v1, float scale)
    {
        return ccp(v1.x * scale, v1.y * scale);        
    }

    inline Vector2 operator * (float scale, const Vector2& v1)
    {
        return ccp(v1.x * scale, v1.y * scale);        
    }
    
    inline Vector2 operator / (const Vector2& v1, float scale)
    {
        return ccp(v1.x / scale, v1.y / scale);        
    }

	inline bool operator == (const Vector2& v1, const Vector2& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y);
    }

	inline bool operator != (const Vector2& v1, const Vector2& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y);
    }

	inline void swap(Vector2 &p1, Vector2 &p2)
    {
        Vector2& pt = p1;
        p1 = p2;
        p2 = pt;
    }

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef WIN32
    inline float abs(float x)
    {
        return x >= 0? x: -x;
    }
#endif

    inline float sq(float x)
    {
        return x * x;
    }
 
    inline int isNotIn(float x, float min, float max)
    {
        if(x < min)
            return -1;
        else if (x > max)
            return 1;
        else 
            return 0;
    }


#if (COCOS2D_VERSION < 0x00020000)
    inline float ccpDistanceSQ(const Vector2& v1, const Vector2& v2)
    {
        return sq(v1.x-v2.x) + sq(v1.y-v2.y);
    }
#endif

    /** rotation in degrees */
    inline Vector2 vector2ForRotation(float rot)
    {
        return cocos2d::ccpForAngle(CC_DEGREES_TO_RADIANS(-rot));
    }

    /** rotation in degrees */
    inline float angleForVector2(const Vector2& vec)
    {
        return -CC_RADIANS_TO_DEGREES(cocos2d::ccpToAngle(vec));
    }

    /** normalize degree to [-180 180)*/
    float normalizeDegree(float degree); 


    inline bool isPointOnLine(const Point& p, const Point& A, const Point& B)
    {
	    return ((p.y - A.y) * ( p.x - B.x ) == (p.y - B.y ) * (p.x - A.x));
    }

    inline bool isPointBetween(const Point& p, const Point& A, const Point& B)
    {
        return p.x <= max(A.x, B.x) && p.x >= min(A.x, B.x); 
    }

    inline bool isPointOnSeg(const Point& p, const Point& A, const Point& B)
    {
        return isPointOnLine(p, A, B) && isPointBetween(p, A, B);
    }

    bool isPointInRound(const Point& point, const Point& roundCenter, float radius);
    
    bool isPointInRoundSQ(const Point& point, const Point& roundCenter, float radiusSQ);

    bool isPointInRect(const Point& point, const Point& topA, const Point& topB);

    bool isPointInRect(const Point& point, const Point& topA, const cocos2d::CCSize& size);

    bool isPointInRect(const Point& point, const Point& rect, float width, float height, const Point& angle);
     
    bool isPointInRect(const Point& point, const Point& A, const Point& B, const Point& C, const Point& D);

    /* return if roundA crossed roundB*/
    bool isRoundCrossRound(const Point& centerA, float radiusA, 
        const Point& centerB, float radiusB);

    /** return if round crossed rect,
        @param rrp(optional) [out] related coordinates of round in the rect's coordinate system */
    bool isRoundCrossRect(const Point& round, float radius,
        const Point& rect, float width, float height, const Point& angle, Point* rrp = NULL);

    /** return if rectA crossed rectB */
    bool isRectCrossRect(
            const Point& Atop0, const Point& Atop1, const Point& Atop2, const Point& Atop3,
            const Point& Btop0, const Point& Btop1, const Point& Btop2, const Point& Btop3);


    const int RECTCROSS_A_IN_B = -1;
    const int RECTCROSS_B_IN_A = -2;

    /** return if rectA crossed rectB, 
        @param cpcount(optional) return the number of the cross points
        @param cp(optional) return the cross point coordinates, prepare a array for 8 points*/
    bool isRectCrossRect(
            const Point& Atop0, const Point& Atop1, const Point& Atop2, const Point& Atop3,
            const Point& Btop0, const Point& Btop1, const Point& Btop2, const Point& Btop3,
            int* cpcount = NULL, Point* cp = NULL);


	Point getPerpendicular(const Point& point);

	int gcd(int a, int b);

	int sign(int a);

	int pointOnLineSide(const Point& point, const Point& lineStart, const Point& lineEnd);

    /** return the nearest point to P on segment AB */
    Point nearestPointOnSeg(const Point& P, const Point& A, const Point& B);
}
