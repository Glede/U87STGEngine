//
//  Collision.cpp
//  ADGameScene
//
//  Created by Glede on Oct 1st 2012
//  Supplemented by CaiyZ(Mr.IroN)
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "U87Collision.h"
#include "U87Math.h"
#include <cmath>


USING_NS_CC;

namespace u87
{

    //#define HTR_NONE        HitTestResult(HTRT_NONE, VECTOR2_ZERO)
    //#define HTR_CONTACT(x)  HitTestResult(HTRT_CONTACT, (x))
    //#define HTR_CROSS(x)    HitTestResult(HTRT_CROSS, (x))
 
    //inline HitTestResult isPointInRound( const Vector2& P, const Round* R)
    //{
    //    float disSQ = ccpDistanceSQ(R->center, P);
    //    float Radius = sq(R->radius);

    //    if(disSQ == Radius)
    //        return HTR_CROSS(P);
    //    else if(disSQ < Radius)
    //        return HTR_CONTACT(P);
    //    else
    //        return HTR_NONE;
    //}

    //inline HitTestResult isPointOnSeg( const Segment* A, const Vector2& P )
    //{
	   // if( (P.y - A->begin.y) * ( P.x - A->end.x ) 
    //        == (P.y - A->end.y ) * (P.x - A->begin.x))
    //        return HTR_CONTACT(P);
    //    else 
    //        return HTR_NONE;
    //}

    //HitTestResult hitTestSegSeg( const Segment* a, const Segment* b )
    //{
    //    const Vector2& A1 = a->begin;
    //    const Vector2& A2 = a->end;
    //    const Vector2& B1 = b->begin;
    //    const Vector2& B2 = b->end;

    //    if( !ccpSegmentIntersect(A1, A2, B1, B2) )
    //        return HTR_NONE;
    //
    //    Vector2 hitpoint = ccpIntersectPoint(A1, A2, B1, B2);
    //    if(hitpoint == A1 || hitpoint == A2 || hitpoint == B1 || hitpoint == B2)
    //        return HTR_CONTACT(hitpoint);
    //    else
    //        return HTR_CROSS(hitpoint);
    //}

    ///** 
    // * @code 算法 线段 Seg 与圆 R 碰撞求交点
    // * 记交点向量 P 为 VP, Seg 起点为 VA 终点为 VB.
    // * 设变量 u 在[0, 1]内来表示P点位置
    // * VP = VA + u * (VB - VA);
    // * 即
    // * {  VP.x  = VA.x + u * (VB.x - VA.x);
    // * {  VP.y  = VA.y + u * (VB.y - VA.y);
    // * 
    // * 因为VP在 round R 上，所以
    // * distance( VP, R->center ) == R->radius
    // * 记 R->center 为 VC, radius 为 r.
    // * { (VP.x - VC.x)^2 + (VP.y - VC.y)^2 = r^2
    // * 
    // * 联立上式得方程
    // * A * u^2 + B * u + C == 0;
    // * 其中
    // * A = (VB.x - VA.x)^2 + (VB.y - VA.y)^2
    // * B = 2[(VB.x - VA.x)(VA.x - VC.x) + (VB.y - VA.y)(VA.y - VC.y)]
    // * C = (VC.x - VA.x)^2 + (VC.y - VA.y)^2 - r^2;
    // *
    // * 如果 u 无解，则直线与圆不相交，线段与圆不相交
    // * 如果 delta 为 0，则直线与圆相切，查看切点是否在线段上
    // * 如果 u 有两个不等的解
    // * 以解得 u 的值判断
    // * u的两个解都是小于0或者大于1的，则线段在圆外。
    // * u的两个解符号相反，一个小于0，一个大于1，则线段在圆内。
    // * u值中有一个是在0和1之间，另一个不是，线段和圆有一个交点。
    // * u值两个解都在0和1之间，线段与圆两个交点 
    // */
    HitTestResult* hitTestSegRound( const Segment* seg, const Round* R )
    {
        const Vector2& VA = seg->begin;
        const Vector2& VB = seg->end;
        const Vector2& VC = R->center;
        const float r    = R->radius;

        // is the seg a point?
	    if( VA == VB )	
        {
            if(isPointInRound( VA, R->center, R->radius ))
                return HitTestResult::create(HTRT_CROSS, VA, 0);
            else
                return HitTestResult::create(HTRT_NONE);
        }

        // formular A * u^2 + B * u + C == 0
        float A = sq(VB.x - VA.x) + sq(VB.y - VA.y);
        float B = 2 * ((VB.x - VA.x) * (VA.x - VC.x) + 
            (VB.y - VA.y) * (VA.y - VC.y));
        float C = sq(VC.x - VA.x) + sq(VC.y - VA.y) - sq(r);
    
        float delta = sq(B) - 4 * A * C;
        if(delta < 0)
            return HitTestResult::create(HTRT_NONE);    // no cross
        else if (delta == 0)    // the line contacts with the round
        {
            float u = (-B) / (2*A);
            if(u < 0 || u > 1)
            {
                return HitTestResult::create(HTRT_NONE);
            }
            else 
            {
                Point hitpoint = VA + (VB - VA)*u;
                float distance = ccpDistance(hitpoint, VA);
                return HitTestResult::create(HTRT_CROSS, hitpoint, distance);
            }
        } 
        else                    // the line crosses the round
        {
            float sqrtDelta = sqrt(delta);
            float u[2];
            u[0] = (-B - sqrtDelta) / (2 * A);
            u[1] = (-B + sqrtDelta) / (2 * A);
            int uin[2];
            for(int i = 0; i < 2; i++)
            {
                if(u[i] < 0)
                    uin[i] = 0;
                else if(u[i] > 1)
                    uin[i] = 1;
                else
                    uin[i] = u[i];
            } 

            if(uin[0] == 0)
            {
                return HitTestResult::create(HTRT_CROSS, VA + (VB - VA)*u[0], ccpLength((VB - VA)*u[0]));
            }
            else if (uin[1] == 0)
            {
                return HitTestResult::create(HTRT_CROSS, VA + (VB - VA)*u[1], ccpLength((VB - VA)*u[1]));
            }
            else if ((uin[0] < 0 && uin[1] < 0) || (uin[0] > 0 && uin[1] > 0))
                return HitTestResult::create(HTRT_NONE);
            else 
                return HitTestResult::create(HTRT_CROSS, VA, 0);
        }
    }
 

    //HitTestResult hitTestRoundRound( const Round* R1, const Round* R2 )
    //{

    //    float distanceSQ = ccpDistanceSQ(R1->center, R2->center);
    //    float radiusSumSQ = sq(R1->radius + R2->radius);

    //    if(distanceSQ < radiusSumSQ)
    //    {
    //        float distance = sqrt(distanceSQ);
    //        float scaleA = R1->radius / distance;
    //        float scaleB = R2->radius / distance;
    //        Vector2 p1 = R1->center + scaleA * (R2->center - R1->center);
    //        Vector2 p2 = R2->center + scaleB * (R1->center - R2->center);

    //        return HTR_CROSS((p1 + p2)/2);
    //    }
    //    else if (distanceSQ == radiusSumSQ)
    //    {
    //        float distance = sqrt(distanceSQ);
    //        float scale = R1->radius / distance;
    //        return HTR_CONTACT( R1->center + scale * (R2->center - R1->center));
    //    } 
    //    else 
    //        return HTR_NONE;
    //}

    ////
    ////HitTestResult u87::hitTest(const Body* a, const Body* b)
    ////{
    ////    switch( a->shape )
    ////    {
    ////    case SHAPE_SEGMENT:
    ////        {
    ////            switch(b->shape)
    ////            {
    ////            case SHAPE_SEGMENT:
    ////                return hitTestSegSeg((Segment*)a, (Segment*)b);
    ////            case SHAPE_ROUND:
    ////                return hitTestSegRound((Segment*)a, (Round*)b);
    ////            }
    ////        }
    ////    case SHAPE_ROUND:
    ////        {
    ////            switch(b->shape)
    ////            {
    ////            case SHAPE_SEGMENT:
    ////                return hitTestSegRound((Segment*)b, (Round*)a);
    ////            case SHAPE_ROUND:
    ////                return hitTestRoundRound((Round*)a, (Round*)b);
    ////            }
    ////        }
    ////    }
    ////
    ////    return HTR_NONE;
    ////}
    //// 

    bool hitTestSegSeg(const Point& a, const Point& b, const Point& c, const Point& d, Point& intersectPoint)
    {
        float s, t;
        if(ccpLineIntersect(a, b, c, d, &s, &t)
            && isNotIn(s, 0, 1) == 0 && isNotIn(t, 0, 1) == 0)
        {   
            intersectPoint = a + s*(b - a);
            return true;
        }
        return false;
    }
     
    ///////////////////////////////////////////////////////////////////////////
    // strip hit test implementation
    ///////////////////////////////////////////////////////////////////////////

    // to implement
    HitTestResult* Strip::hitTest(const Strip* target)
    {
        bool hit = false;
        Point finalPoint;

        hit = hitTestSegSeg(begin, end, target->begin, target->end, finalPoint);

        if(width != 0 && target->width != 0)
        {
 
            
        }

        //return HitTestResult::create(HTRT_NONE, finalPoint);
        return NULL;
    }

    // to implement
    HitTestResult* Strip::hitTest(const Round* target)
    {
        if(width == 0)
        { 

        }

		//Point projBase = this->begin + (this->width / 2.0) * getPerpendicular(this->end - this->begin);
		//Point footPoint = projBase + ccpProject(target->center - projBase, this->begin - projBase);
		//float drift = ccpDistance(footPoint, this->begin);
		//if (drift > target->radius + this->width / 2.0) {
		//	return HitTestResult::create(HTRT_NONE);
		//}
		//else if (drift > this->width / 2.0) {
		//	Point projBase2 = this->begin - (this->width / 2.0) * getPerpendicular(this->end - this->begin);
		//	if (ccpDistance(footPoint, projBase2) < ccpDistance(footPoint, projBase)) {
		//		projBase = projBase2;
		//	}
		//	float deviate = ccpDistance(footPoint, projBase);
		//	float distance = ccpDistance(target->center, footPoint) - sqrt(target->radius * target->radius - deviate * deviate);
		//	if (distance < ccpDistance(this->end, this->begin)) {
		//		Point crossPoint = projBase + distance * ccpNormalize(this->end - this->begin);
		//		return HitTestResult::create(HTRT_CROSS, crossPoint, distance);
		//	}
		//	else {
		//		return HitTestResult::create(HTRT_NONE);
		//	}
		//}
		//else {
		//	float distance = ccpDistance(target->center, footPoint) - target->radius;
		//	if (distance < ccpDistance(this->end, this->begin)) {
		//		Point crossPoint = footPoint + distance * ccpNormalize(this->end - this->begin);
		//		return HitTestResult::create(HTRT_CROSS, crossPoint, distance);
		//	}
		//	else {
		//		return HitTestResult::create(HTRT_NONE);
		//	}
		//}

        return HitTestResult::create(HTRT_NONE);
    }

    // to implement
    HitTestResult* Strip::hitTest(const Rect* target)
    {
        return NULL;
    }


    ///////////////////////////////////////////////////////////////////////////
    // round hit test implementation
    ///////////////////////////////////////////////////////////////////////////

    HitTestResult* hitTestRoundRect(Round* round, const Point& a, 
        const Point& b, const Point& c, const Point& d)
    {
        Point p[4];
        float dsq[4];
        p[0] = nearestPointOnSeg(round->center, a, b);
        p[1] = nearestPointOnSeg(round->center, b, c);
        p[2] = nearestPointOnSeg(round->center, c, d);
        p[3] = nearestPointOnSeg(round->center, d, a);
        int minI = 0;
        dsq[0] = ccpDistanceSQ(round->center, p[0]);

        for(int i = 1; i < 4; i++)
        {
            dsq[i] = ccpDistanceSQ(round->center, p[i]);

            if(dsq[i] < dsq[0])
            {
                dsq[0] = dsq[i];
                minI = i;
            }
        }

        if(dsq[minI] <= sq(round->radius))
        {
            return HitTestResult::create(HTRT_CROSS, p[minI], sqrt(dsq[minI]));
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }

    // to implement
    HitTestResult* Round::hitTest(const Strip* target)
    {
        if(target->width == 0) // special for line 
        {
            Point hp = nearestPointOnSeg(center, target->begin, target->end);
            float dsq = ccpDistanceSQ(center, hp);
            if(dsq > sq(radius))
                return HitTestResult::create(HTRT_NONE);
            else
                return HitTestResult::create(HTRT_CROSS, hp, sqrt(dsq));
        }

        Vector2 vec = ccpNormalize(target->end - target->begin);
        vec = ccp(vec.y, -vec.x) * target->width/2;

        Point a = target->begin + vec;
        Point b = target->end + vec;
        Point c = target->end - vec;
        Point d = target->begin - vec;

        return hitTestRoundRect(this, a, b, c, d);
    }

    // to implement
    HitTestResult* Round::hitTest(const Round* target)
    {
		float centerDist = ccpDistance(this->center, target->center);
		if (centerDist < this->radius + target->radius) {
			float distance = centerDist - target->radius;
			Point crossPoint = this->center + distance * ccpNormalize(target->center - this->center);
			return HitTestResult::create(HTRT_CROSS, crossPoint, distance);
		}
		else {
			return HitTestResult::create(HTRT_NONE);
		}
    }

    // to implement
    HitTestResult* Round::hitTest(const Rect* target)
    { 
        Vector2 wv = vector2ForRotation(target->angle);
        Vector2 hv = ccp(wv.y, -wv.x);
        Vector2 wl = target->width/2 * wv;
        Vector2 hl = target->height/2 * hv;

        Point a = target->center + wl + hl;
        Point b = target->center - wl + hl;
        Point c = target->center - wl - hl;
        Point d = target->center + wl - hl;
        return hitTestRoundRect(this, a, b, c, d);
    }

    
    ///////////////////////////////////////////////////////////////////////////
    // rect hit test implementation
    ///////////////////////////////////////////////////////////////////////////

    // to implement
    HitTestResult* Rect::hitTest(const Strip* target)
    {
 
        return NULL;
    }

    // to implement
    HitTestResult* Rect::hitTest(const Round* target)
    { 
        return NULL;
    }

    // to implement
    HitTestResult* Rect::hitTest(const Rect* target)
    { 
        return NULL;
    }
}