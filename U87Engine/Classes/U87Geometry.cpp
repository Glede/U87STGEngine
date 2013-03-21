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
     inline bool exclude(BodyNode* a, BodyNode* b)
     {
         return ccpDistanceSQ(a->getCenter(), b->getCenter()) > sq(a->getRadius() + b->getRadius());
     }

     HitTestResult* BodyNode::hitTest(BodyNode* body)
     {
         return HitTestResult::create(HTRT_NONE);
     }

     HitTestResult* RoundBodyNode::hitTest(BodyNode* body)
     {
         if(exclude(this, body))
             return HitTestResult::create(HTRT_NONE);

         switch (body->getType())
         {
         default:
         case BODY_NONE:
             return HitTestResult::create(HTRT_NONE);
         case BODY_ROUND:
             return hitTestRoundRound(this, (RoundBodyNode*)body);
         case BODY_RECT:
             return hitTestRoundRect(this, (RectBodyNode*)body);
         case BODY_STRIP:
             return hitTestRoundStrip(this, (StripBodyNode*)body);
         }
     }


     HitTestResult* RectBodyNode::hitTest(BodyNode* body)
     {
         if(exclude(this, body))
             return HitTestResult::create(HTRT_NONE);

         switch (body->getType())
         {
         default:
         case BODY_NONE:
             return HitTestResult::create(HTRT_NONE);
         case BODY_ROUND:
             return hitTestRectRound(this, (RoundBodyNode*)body);
         case BODY_RECT:
             return hitTestRectRect(this, (RectBodyNode*)body);
         case BODY_STRIP:
             return hitTestRectStrip(this, (StripBodyNode*)body);
         }
     }


     HitTestResult* StripBodyNode::hitTest(BodyNode* body)
     {
         if(exclude(this, body))
             return HitTestResult::create(HTRT_NONE);

         switch (body->getType())
         {
         default:
         case BODY_NONE:
             return HitTestResult::create(HTRT_NONE);
         case BODY_ROUND:
             return hitTestStripRound(this, (RoundBodyNode*)body);
         case BODY_RECT:
             return hitTestStripRect(this, (RectBodyNode*)body);
         case BODY_STRIP:
             return hitTestStripStrip(this, (StripBodyNode*)body);
         }
     }


     //////////////////////////////////////////////////////////////////////////
     // algorithms
     //////////////////////////////////////////////////////////////////////////

    HitTestResult* hitTestRoundRound(RoundBodyNode* a, RoundBodyNode* b)
    {
        Point ca = a->getCenter();
        Point cb = b->getCenter();
        float rb = b->getRadius();
        float distance = ccpDistance(ca, cb);
        Point hp = cb + (ca - cb)/distance * rb;
        
        return HitTestResult::create(HTRT_CROSS, hp, distance - rb);
    }

    HitTestResult* hitTestRoundRect(RoundBodyNode* a, RectBodyNode* b)
    {
        Point ca = a->getCenter();
        float ra = a->getRadius();
        Point cb = b->getCenter();
        float wb = b->getWidth();
        float hb = b->getHeight();
        float ab = CC_DEGREES_TO_RADIANS(-b->getAngle());
        float rb = b->getRadius();
        Point rrp;
        bool hit = isRoundCrossRect(ca, ra, cb, wb, hb, ccpForAngle(ab), &rrp);

        if( hit )
        {
            float distance;
            float angle = ccpToAngle(rrp);
            Point hp = ccp(cos(angle), sin(angle)) * rb;

            if(hp.x > wb/2)
                hp.x = wb/2;
            else if(hp.x < -wb/2)
                hp.x = -wb/2;

            if(hp.y > hb/2)
                hp.y = hb/2;
            else if(hp.y < -hb/2)
                hp.y = -hb/2;

            hp = cb + ccpRotateByAngle(hp, POINT_ZERO, ab);
            distance = ccpDistance(ca, hp);
            return HitTestResult::create(HTRT_CROSS, hp, distance);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }

    HitTestResult* hitTestRoundStrip(RoundBodyNode* a, StripBodyNode* b)
    {
        Point ca = a->getCenter();
        float ra = a->getRadius();
        Point cb = b->getCenter();
        Vector2 avb = b->getEnd() - b->getBegin();
        float hb = b->getWidth();
        float wb = ccpLength(avb); 
        float rb = b->getRadius();
        Point rrp;

        avb = avb/wb;
        bool hit = isRoundCrossRect(ca, ra, cb, wb, hb, avb, &rrp);
        if(hit)
        {
            float distance;
            float angle = ccpToAngle(rrp);
            Point hp = ccp(cos(angle), sin(angle)) * rb;

            if(hp.x > wb/2)
                hp.x = wb/2;
            else if(hp.x < -wb/2)
                hp.x = -wb/2;

            if(hp.y > hb/2)
                hp.y = hb/2;
            else if(hp.y < -hb/2)
                hp.y = -hb/2;

            hp = ccpRotate(hp, avb) + cb;
            distance = ccpDistance(ca, hp);
            return HitTestResult::create(HTRT_CROSS, hp, distance);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }

    HitTestResult* hitTestRectRound(RectBodyNode* a, RoundBodyNode* b)
    {
        Point ca = a->getCenter();
        float wa = a->getWidth();
        float ha = a->getHeight();
        float aa = CC_DEGREES_TO_RADIANS(-a->getAngle());
        float ra = a->getRadius();
        Point cb = b->getCenter();
        float rb = b->getRadius();
        bool hit = isRoundCrossRect(cb, rb, ca, wa, ha, ccpForAngle(aa));
        if(hit)
        {
            Point hp;
            float distance;
            Vector2 delta = ca - cb;
            float deltaLength = ccpLength(delta);
            hp = cb + delta/deltaLength * rb;

            distance = deltaLength - rb;
            return HitTestResult::create(HTRT_CROSS, hp, distance);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }


    HitTestResult* hitTestRectRect(RectBodyNode* a, RectBodyNode* b)
    {
        Point Atop[4];
        Point Btop[4];
        a->getTops(Atop, Atop + 1, Atop + 2, Atop + 3);
        b->getTops(Btop, Btop + 1, Btop + 2, Btop + 3);
        int cpcount = 0;
        Point cp[12];
        bool hit = isRectCrossRect(Atop[0], Atop[1], Atop[2], Atop[3],
            Btop[0], Btop[1], Btop[2], Btop[3], &cpcount, cp);

        if(hit)
        {
            if(cpcount < 0)
                cpcount = 0;

            Point ca = a->getCenter();
            float wa = a->getWidth();
            float ha = a->getHeight();
            Vector2 aa = vector2ForRotation(a->getAngle());
            for(int i = 0; i < 4; i++)
            {
                if(isPointInRect(Btop[i], ca, wa, ha, aa))
                {
                    cp[cpcount++] = Btop[i];
                }
            }

            // use dot result for comparing directly
            // get true distance when receive a minimun dot result
            int iMin = 0;
            float dMin = 0;
            Vector2 direction = vector2ForRotation(a->getAngle());
            for(int i = 0; i < cpcount; i++)
            {
                float d = ccpDistanceSQ(ca, cp[i]);
                if( i == 0 && d < dMin)
                {
                    dMin = d;
                    iMin = i;
                }
            }

            dMin = sq(dMin);
            return HitTestResult::create(HTRT_CROSS, cp[iMin], dMin);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }


    HitTestResult* hitTestRectStrip(RectBodyNode* a, StripBodyNode* b)
    {
        Point Atop[4];
        Point Btop[4];
        a->getTops(Atop, Atop + 1, Atop + 2, Atop + 3);
        b->getTops(Btop, Btop + 1, Btop + 2, Btop + 3);
        int cpcount = 0;
        Point cp[12];
        bool hit = isRectCrossRect(Atop[0], Atop[1], Atop[2], Atop[3],
            Btop[0], Btop[1], Btop[2], Btop[3], &cpcount, cp);

        if(hit)
        {
            if(cpcount < 0)
                cpcount = 0;

            Point ca = a->getCenter();
            float wa = a->getWidth();
            float ha = a->getHeight();
            Vector2 aa = vector2ForRotation(a->getAngle());
            for(int i = 0; i < 4; i++)
            {
                if(isPointInRect(Btop[i], ca, wa, ha, aa))
                {
                    cp[cpcount++] = Btop[i];
                }
            }

            // use dot result for comparing directly
            // get true distance when receive a minimun dot result
            int iMin = 0;
            float dMin = 0;
            Vector2 direction = vector2ForRotation(a->getAngle());
            for(int i = 0; i < cpcount; i++)
            {
                float d = ccpDistanceSQ(ca, cp[i]);
                if( i == 0 && d < dMin)
                {
                    dMin = d;
                    iMin = i;
                }
            }

            dMin = sq(dMin);
            return HitTestResult::create(HTRT_CROSS, cp[iMin], dMin);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }


    HitTestResult* hitTestStripRound(StripBodyNode* a, RoundBodyNode* b)
    {
        Point cb = b->getCenter();
        float rb = b->getRadius();
        Point ca = a->getCenter();
        Vector2 ava = a->getEnd() - a->getBegin();
        float ha = a->getWidth();
        float wa = ccpLength(ava); 
        float ra = a->getRadius();
        Point rrp;
        ava = ava/wa;
        bool hit = isRoundCrossRect(cb, rb, ca, wa, ha, ava);
        if(hit)
        {
            Point hp;
            float distance;
            Point ba = a->getBegin();
            Vector2 delta = ba - cb;
            float deltaLength = ccpLength(delta);
            hp = cb + delta/deltaLength * rb;

            distance = deltaLength - rb;
            return HitTestResult::create(HTRT_CROSS, hp, distance);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }


    HitTestResult* hitTestStripRect(StripBodyNode* a, RectBodyNode* b)
    {
        Point Atop[4];
        Point Btop[4];
        a->getTops(Atop, Atop + 1, Atop + 2, Atop + 3);
        b->getTops(Btop, Btop + 1, Btop + 2, Btop + 3);
        int cpcount = 0;
        Point cp[12];
        bool hit = isRectCrossRect(Atop[0], Atop[1], Atop[2], Atop[3],
            Btop[0], Btop[1], Btop[2], Btop[3], &cpcount, cp);

        if(hit)
        {
            if(cpcount < 0)
                cpcount = 0;

            Point ca = a->getCenter();
            float wa = a->getWidth();
            float ha = ccpDistance(a->getBegin(), a->getEnd());
            Vector2 aa = ccpNormalize(a->getEnd() - a->getBegin());
            for(int i = 0; i < 4; i++)
            {
                if(isPointInRect(Btop[i], ca, wa, ha, aa))
                {
                    cp[cpcount++] = Btop[i];
                }
            }

            // use dot result for comparing directly
            // get true distance when receive a minimun dot result
            int iMin = 0;
            float dMin = 0;
            Vector2 direction = a->getEnd() - a->getBegin();
            for(int i = 0; i < cpcount; i++)
            {
                float d = ccpDot(cp[i], direction);
                if( i == 0 && d < dMin)
                {
                    dMin = d;
                    iMin = i;
                }
            }

            dMin /= ccpLength(direction);
            return HitTestResult::create(HTRT_CROSS, cp[iMin], dMin);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }


    HitTestResult* hitTestStripStrip(StripBodyNode* a, StripBodyNode* b)
    {
        Point Atop[4];
        Point Btop[4];
        a->getTops(Atop, Atop + 1, Atop + 2, Atop + 3);
        b->getTops(Btop, Btop + 1, Btop + 2, Btop + 3);
        int cpcount = 0;
        Point cp[12];
        bool hit = isRectCrossRect(Atop[0], Atop[1], Atop[2], Atop[3],
            Btop[0], Btop[1], Btop[2], Btop[3], &cpcount, cp);

        if(hit)
        {
            if(cpcount < 0)
                cpcount = 0;

            Point ca = a->getCenter();
            float wa = a->getWidth();
            float ha = ccpDistance(a->getBegin(), a->getEnd());
            Vector2 aa = ccpNormalize(a->getEnd() - a->getBegin());
            for(int i = 0; i < 4; i++)
            {
                if(isPointInRect(Btop[i], ca, wa, ha, aa))
                {
                    cp[cpcount++] = Btop[i];
                }
            }

            // use dot result for comparing directly
            // get true distance when receive a minimun dot result
            int iMin = 0;
            float dMin = 0;
            Vector2 direction = a->getEnd() - a->getBegin();
            for(int i = 0; i < cpcount; i++)
            {
                float d = ccpDot(cp[i], direction);
                if( i == 0 && d < dMin)
                {
                    dMin = d;
                    iMin = i;
                }
            }

            dMin /= ccpLength(direction);
            return HitTestResult::create(HTRT_CROSS, cp[iMin], dMin);
        }
        else
        {
            return HitTestResult::create(HTRT_NONE);
        }
    }
}