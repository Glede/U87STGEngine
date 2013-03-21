#pragma once
#include "U87Math.h" 
#include "U87Geometry.h"

namespace u87
{
    
    ///////////////////////////////////////////////////////////////////////////
    // functions
    ///////////////////////////////////////////////////////////////////////////
    
    /** will return collision point in world space coordinate */
    bool collisionTest(BodyNode* a, BodyNode* b);

    /** will return collision point in world space coordinate */
    Point collisionPoint(BodyNode* a, BodyNode* b);
}