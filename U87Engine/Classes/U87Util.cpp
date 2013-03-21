//
//  Util.cpp
//  U87Engine
//
//  Created by Ñî »Æ on 12-10-3.
//  Copyright (c) 2012Äê __MyCompanyName__. All rights reserved.
//

#include "U87Util.h"
#include "U87Math.h"

USING_NS_U;
USING_NS_CC;

#ifdef FIX_CCZONE

namespace   cocos2d 
{
    CCZone::CCZone(CCObject *pObject)
    {
	    m_pCopyObject = pObject;
    }
}

#endif