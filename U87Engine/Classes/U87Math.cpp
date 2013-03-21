//
//  Math.cpp
//  U87Engine
//
//  Created by Glede on Oct 1st 2012
//  Supplemented by CaiyZ(Mr.IroN)
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "U87Math.h"

USING_NS_CC;

namespace u87
{

    float normalizeDegree(float degree)
    {
        int r;
        if(degree >= 180)
        {
            r = (int)((degree + 180) / 360);
            degree -= r * 360;
        }
        else if(degree < 180)
        {
            r = (int)((degree - 180) / -360);
            degree += r * 360;
        }

        return degree;
    }

    bool isPointInRound(const Point& point, const Point& roundCenter, float radius)
    {
        return ccpDistanceSQ(point, roundCenter) <= sq(radius);
    }

    bool isPointInRoundSQ(const Point& point, const Point& roundCenter, float radiusSQ)
    {
        return ccpDistanceSQ(point, roundCenter) <= radiusSQ;
    }


    bool isPointInRect(const Point& point, const Point& topA, const Point& topB)
    {
        float minx = min(topA.x, topB.x);
        float maxx = max(topA.x, topB.x);
        float miny = min(topA.y, topB.y);
        float maxy = max(topA.y, topB.y);
        return point.x >= minx && point.x <= maxx
            && point.y >= miny && point.y <= maxy;
    }

    bool isPointInRect(const Point& point, const Point& topA, const cocos2d::CCSize& size)
    {
        float minx = min(topA.x, topA.x + size.width);
        float maxx = max(topA.x, topA.x + size.width);
        float miny = min(topA.y, topA.y + size.height);
        float maxy = max(topA.y, topA.y + size.height);
        return point.x >= minx && point.x <= maxx
            && point.y >= miny && point.y <= maxy;
    }

    bool isPointInRect(const Point& point, const Point& rect, float width, float height, const Point& angle)
    {
        Point relatedPoint = ccpUnrotate(point - rect, angle);
        relatedPoint.x = abs(relatedPoint.x);
        relatedPoint.y = abs(relatedPoint.y);    
    
        return relatedPoint.x <= width/2 && relatedPoint.y <= height/2;
    }

    bool isPointInRect(const Point& point, const Point& A, const Point& B, const Point& C, const Point& D)
    {
        Vector2 a = A - point;
        Vector2 b = B - point;
        Vector2 d = D - point;

        return ccpSegmentIntersect(a, b, POINT_ZERO, a - d);
    }



    Point u87::getPerpendicular(const Point& point)
    {
	    return ccpNormalize(ccp(-point.y / point.x, 1.0));
    }

    int gcd(int a, int b)
    {
	    if (a < b) {
		    int t = a;  a = b;  b = t;
	    }
	    int remain = 1;
	    while (remain > 0) {
		    remain = a % b;
		    a = b;  b = remain;
	    }
	    return a;
    }

    int sign(int a)
    {
	    if (a > 0)
		    return 1;
	    else if (a < 0)
		    return -1;
	    else
		    return 0;
    }

    int pointOnLineSide(const Point& point, const Point& lineStart, const Point& lineEnd)
    {
	    float crossResult = ccpCross(point - lineStart, point - lineEnd);
	    return sign(crossResult);
    }

    Point nearestPointOnSeg(const Point& P, const Point& A, const Point& B)
    {
        if(A == B)
        {
            return A;
        }

        Vector2 ab = (B - A);
        Point Q = P + ccp(ab.y, -ab.x);

        float t;
        float s;
        ccpLineIntersect(P, Q, A, B, &t, &s);
        if(s <= 0)
        {
            return A;
        } 
        else if ( s >= 1)
        {
            return B;
        } 
        else 
        {
            return A + (B - A)*s;
        }
    }


    bool isRoundCrossRound(const Point& centerA, float radiusA, 
        const Point& centerB, float radiusB)
    {
        float distanceSQ = ccpDistanceSQ(centerA, centerB);
        float radiusSumSQ = sq(radiusA + radiusB);
        return distanceSQ <= radiusSumSQ;
    }
    
    // 
    bool isRoundCrossRect(const Point& round, float radius,
        const Point& rect, float width, float height, const Point& angle, Point* rrp)
    {
        Point relatedRound = ccpUnrotate(round - rect, angle);
        if(rrp)
            *rrp = relatedRound;
        relatedRound.x = abs(relatedRound.x);
        relatedRound.y = abs(relatedRound.y);
        width /= 2;
        height /= 2;
        if(relatedRound.x > width && relatedRound.y > height)
        {
            return isPointInRound(ccp(width, height), relatedRound, radius);
        }
        else if(relatedRound.x > width)
        {
            return relatedRound.x - radius <= width;
        }
        else if(relatedRound.y > height)
        {
            return relatedRound.y - radius <= height;
        }
        else
        {
            return true;
        }
    }



    bool isRectCrossRect(
            const Point& Atop0, const Point& Atop1, const Point& Atop2, const Point& Atop3,
            const Point& Btop0, const Point& Btop1, const Point& Btop2, const Point& Btop3)
    {
        bool bRet = true;
        do
        {
            CC_BREAK_IF(ccpSegmentIntersect(Atop0, Atop1, Btop0, Btop1));
            CC_BREAK_IF(ccpSegmentIntersect(Atop0, Atop1, Btop1, Btop2));
            CC_BREAK_IF(ccpSegmentIntersect(Atop0, Atop1, Btop2, Btop3));
            CC_BREAK_IF(ccpSegmentIntersect(Atop0, Atop1, Btop3, Btop0));

            CC_BREAK_IF(ccpSegmentIntersect(Atop1, Atop2, Btop0, Btop1));
            CC_BREAK_IF(ccpSegmentIntersect(Atop1, Atop2, Btop1, Btop2));
            CC_BREAK_IF(ccpSegmentIntersect(Atop1, Atop2, Btop2, Btop3));
            CC_BREAK_IF(ccpSegmentIntersect(Atop1, Atop2, Btop3, Btop0));

            CC_BREAK_IF(ccpSegmentIntersect(Atop2, Atop3, Btop0, Btop1));
            CC_BREAK_IF(ccpSegmentIntersect(Atop2, Atop3, Btop1, Btop2));
            CC_BREAK_IF(ccpSegmentIntersect(Atop2, Atop3, Btop2, Btop3));
            CC_BREAK_IF(ccpSegmentIntersect(Atop2, Atop3, Btop3, Btop0));

            CC_BREAK_IF(ccpSegmentIntersect(Atop3, Atop0, Btop0, Btop1));
            CC_BREAK_IF(ccpSegmentIntersect(Atop3, Atop0, Btop1, Btop2));
            CC_BREAK_IF(ccpSegmentIntersect(Atop3, Atop0, Btop2, Btop3));
            CC_BREAK_IF(ccpSegmentIntersect(Atop3, Atop0, Btop3, Btop0));

            CC_BREAK_IF(isPointInRect((Atop0 + Atop2)/2, Btop0, Btop1, Btop2, Btop3));
            CC_BREAK_IF(isPointInRect((Btop0 + Btop2)/2, Atop0, Atop1, Atop2, Atop3));

            bRet = false;
        } while(0);
        return bRet;
    }

#define TESTSEG(a, b, c, d) \
        if(ccpLineIntersect(a, b, c, d, &s, &t)) \
        { \
            if(s >= 0 && s <= 1 && t >= 0 && t<= 1)      \
            { \
                cp[*cpcount] = (b - a) * s + a; \
                (*cpcount)++; \
            } \
        } \
         
    bool isRectCrossRect(
            const Point& Atop0, const Point& Atop1, const Point& Atop2, const Point& Atop3,
            const Point& Btop0, const Point& Btop1, const Point& Btop2, const Point& Btop3,
            int* cpcount, Point* cp)
    {
        CC_ASSERT(cpcount != NULL && cp != NULL);
        
        *cpcount = 0;
        float s, t;

        TESTSEG(Atop0, Atop1, Btop0, Btop1);
        TESTSEG(Atop0, Atop1, Btop1, Btop2);
        TESTSEG(Atop0, Atop1, Btop2, Btop3);
        TESTSEG(Atop0, Atop1, Btop3, Btop0);

        TESTSEG(Atop1, Atop2, Btop0, Btop1);
        TESTSEG(Atop1, Atop2, Btop1, Btop2);
        TESTSEG(Atop1, Atop2, Btop2, Btop3);
        TESTSEG(Atop1, Atop2, Btop3, Btop0);

        TESTSEG(Atop2, Atop3, Btop0, Btop1);
        TESTSEG(Atop2, Atop3, Btop1, Btop2);
        TESTSEG(Atop2, Atop3, Btop2, Btop3);
        TESTSEG(Atop2, Atop3, Btop3, Btop0);

        TESTSEG(Atop3, Atop0, Btop0, Btop1);
        TESTSEG(Atop3, Atop0, Btop1, Btop2);
        TESTSEG(Atop3, Atop0, Btop2, Btop3);
        TESTSEG(Atop3, Atop0, Btop3, Btop0);
        
        if(*cpcount != 0)
            return true;

        if(isPointInRect((Atop0 + Atop2)/2, Btop0, Btop1, Btop2, Btop3))
        {
            *cpcount = RECTCROSS_A_IN_B;
            return true;
        }

        if(isPointInRect((Btop0 + Btop2)/2, Atop0, Atop1, Atop2, Atop3))
        {
            *cpcount = RECTCROSS_B_IN_A;
            return true;
        }

        return false;
    }

#undef TESTSEG

}