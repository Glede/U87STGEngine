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
        const Point& rect, float width, float height, const Point& angle)
    {
        Point relatedRound = ccpUnrotate(round - rect, angle);
        relatedRound.x = abs(relatedRound.x);
        relatedRound.y = abs(relatedRound.y);
        width /= 2;
        height /= 2;
        if(round.x > width && round.y > height)
        {
            return isPointInRound(ccp(width, height), round, radius);
        }
        else if(round.x > width)
        {
            return round.y - radius <= height;
        }
        else if(round.y > height)
        {
            return round.x - radius <= width;
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

            CC_BREAK_IF( isPointInRoundSQ(Atop0, (Btop0 + Btop2)/2, ccpDistanceSQ(Btop0, Btop2)/4) );
            CC_BREAK_IF( isPointInRoundSQ(Btop0, (Atop0 + Atop2)/2, ccpDistanceSQ(Atop0, Atop2)/4) );

            bRet = false;
        } while(0);

        return bRet;
    }


}