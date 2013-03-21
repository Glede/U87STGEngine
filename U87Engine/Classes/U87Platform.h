#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN8_METRO)

#define UGenum unsigned int
     
#define UGubyte unsigned char 

/* BlendingFactorDest */
#define UG_ZERO                 CC_ZERO 
#define UG_ONE                  CC_ONE 
#define UG_SRC_COLOR            CC_SRC_COLOR 
#define UG_ONE_MINUS_SRC_COLOR  CC_ONE_MINUS_SRC_COLOR 
#define UG_SRC_ALPHA            CC_SRC_ALPHA 
#define UG_ONE_MINUS_SRC_ALPHA  CC_ONE_MINUS_SRC_ALPHA 
#define UG_DST_ALPHA            CC_DST_ALPHA 
#define UG_ONE_MINUS_DST_ALPHA  CC_ONE_MINUS_DST_ALPHA 

/* BlendingFactorSrc */
/*      UG_ZERO                 CC_ZERO */
/*      UG_ONE                  CC_ONE */
#define UG_DST_COLOR            CC_DST_COLOR 
#define UG_ONE_MINUS_DST_COLOR  CC_ONE_MINUS_DST_COLOR 
#define UG_SRC_ALPHA_SATURATE   CC_SRC_ALPHA_SATURATE 
/*      UG_SRC_ALPHA            CC_SRC_ALPHA */
/*      UG_ONE_MINUS_SRC_ALPHA  CC_ONE_MINUS_SRC_ALPHA */
/*      UG_DST_ALPHA            CC_DST_ALPHA */
/*      UG_ONE_MINUS_DST_ALPHA  CC_ONE_MINUS_DST_ALPHA */


#else


#define UGenum unsigned int
     
#define UGubyte unsigned char 

/* BlendingFactorDest */
#define UG_ZERO                 GL_ZERO 
#define UG_ONE                  GL_ONE 
#define UG_SRC_COLOR            GL_SRC_COLOR 
#define UG_ONE_MINUS_SRC_COLOR  GL_ONE_MINUS_SRC_COLOR 
#define UG_SRC_ALPHA            GL_SRC_ALPHA 
#define UG_ONE_MINUS_SRC_ALPHA  GL_ONE_MINUS_SRC_ALPHA 
#define UG_DST_ALPHA            GL_DST_ALPHA 
#define UG_ONE_MINUS_DST_ALPHA  GL_ONE_MINUS_DST_ALPHA 

/* BlendingFactorSrc */
/*      UG_ZERO                 GL_ZERO */
/*      UG_ONE                  GL_ONE */
#define UG_DST_COLOR            GL_DST_COLOR 
#define UG_ONE_MINUS_DST_COLOR  GL_ONE_MINUS_DST_COLOR 
#define UG_SRC_ALPHA_SATURATE   GL_SRC_ALPHA_SATURATE 
/*      UG_SRC_ALPHA            GL_SRC_ALPHA */
/*      UG_ONE_MINUS_SRC_ALPHA  GL_ONE_MINUS_SRC_ALPHA */
/*      UG_DST_ALPHA            GL_DST_ALPHA */
/*      UG_ONE_MINUS_DST_ALPHA  GL_ONE_MINUS_DST_ALPHA */

#endif 