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
    bool roundCollideRect(BodyNode* round, BodyNode* rect);
    bool roundCollideStrip(BodyNode* round, BodyNode* strip);
    bool rectCollideRect(BodyNode* rectA, BodyNode* rectB);
    bool rectCollideStrip(BodyNode* rect, BodyNode* strip);
    bool stripCollideStrip(BodyNode* stripA, BodyNode* stripB);


    bool collisionTest(BodyNode* a, BodyNode* b)
    {
        if(a->isEnabled() && b->isEnabled())
        {
            if(a->getType() == BODY_NONE || b->getType() == BODY_NONE)
                return false;

            float distanceSQ = ccpDistanceSQ(a->getCenter(), b->getCenter());
            float radiusSumSQ = sq(a->getCircumcircleRadius() + b->getCircumcircleRadius());
            if(radiusSumSQ < distanceSQ)
                return false;

            switch(a->getType())
            {
            case BODY_ROUND:
                {
                    switch(b->getType())
                    {
//                    case BODY_ROUND: return hitTestRoundRound((RoundBodyNode*)a, (RoundBodyNode*)b);
                    case BODY_ROUND: return true; // prev test has show
                    case BODY_RECT: return roundCollideRect(a, b);
                    case BODY_STRIP: return roundCollideStrip(a, b);
                    default: return false;
                    }
                }
            case BODY_RECT:
                {
                    switch(b->getType())
                    {
                    case BODY_ROUND: return roundCollideRect(b, a);
                    case BODY_RECT: return rectCollideRect(a, b);
                    case BODY_STRIP: return rectCollideStrip(a, b);
                    default: return false;
                    }
                }
            case BODY_STRIP:
                {
                    switch(b->getType())
                    {
                    case BODY_ROUND: return roundCollideStrip(b, a);
                    case BODY_RECT: return rectCollideStrip(b, a);
                    case BODY_STRIP: return stripCollideStrip(b, a);
                    default: return false;
                    }
                }
            default: return false;
            }
        }
        else
            return false;
    }

    bool roundCollideRect(BodyNode* round, BodyNode* rect)
    {
        RoundBodyNode* rround = (RoundBodyNode*)round;
        RectBodyNode* rrect = (RectBodyNode*)rect;

        return isRoundCrossRect(rround->getCenter(), rround->getRadius(), 
            rrect->getCenter(), 
            rrect->getWidth(), 
            rrect->getHeight(), 
            ccpForAngle( CC_DEGREES_TO_RADIANS( -rrect->getAngle() )));
    }
    
    bool roundCollideStrip(BodyNode* round, BodyNode* strip)
    {
        RoundBodyNode* rround = (RoundBodyNode*)round;
        StripBodyNode* sstrip = (StripBodyNode*)strip;

        return isRoundCrossRect(rround->getCenter(), rround->getRadius(), 
            sstrip->getCenter(), 
            sstrip->getCircumcircleRadius() * 2, 
            sstrip->getWidth() * 2, 
            ccpNormalize(sstrip->getEnd() - sstrip->getBegin()));
    }


    bool rectCollideRect(BodyNode* rectA, BodyNode* rectB)
    {
        Point Atop0, Atop1, Atop2, Atop3;
        Point Btop0, Btop1, Btop2, Btop3;

        ((RectBodyNode*)rectA)->getTops(&Atop0, &Atop1, &Atop2, &Atop3);
        ((RectBodyNode*)rectB)->getTops(&Btop0, &Btop1, &Btop2, &Btop3);
        
        return isRectCrossRect(Atop0, Atop1, Atop2, Atop3,
            Btop0, Btop1, Btop2, Btop3);
    }


    bool rectCollideStrip(BodyNode* rect, BodyNode* strip)
    {
        Point Atop0, Atop1, Atop2, Atop3;
        Point Btop0, Btop1, Btop2, Btop3;

        ((RectBodyNode*)rect)->getTops(&Atop0, &Atop1, &Atop2, &Atop3);
        ((StripBodyNode*)strip)->getTops(&Btop0, &Btop1, &Btop2, &Btop3);
        
        return isRectCrossRect(Atop0, Atop1, Atop2, Atop3,
            Btop0, Btop1, Btop2, Btop3);
    }


    bool stripCollideStrip(BodyNode* stripA, BodyNode* stripB)
    {
        Point Atop0, Atop1, Atop2, Atop3;
        Point Btop0, Btop1, Btop2, Btop3;

        ((StripBodyNode*)stripA)->getTops(&Atop0, &Atop1, &Atop2, &Atop3);
        ((StripBodyNode*)stripB)->getTops(&Btop0, &Btop1, &Btop2, &Btop3);
        
        return isRectCrossRect(Atop0, Atop1, Atop2, Atop3,
            Btop0, Btop1, Btop2, Btop3);
    }

}
