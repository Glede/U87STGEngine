#pragma once

#include "U87Util.h"

namespace u87
{
    inline cocos2d::ccColor4F ccc4F(float r, float g, float b, float o)
    { 
        cocos2d::ccColor4F c = {r, g, b, o};
	    return c;
    }

    inline cocos2d::ccBlendFunc CCBlendFuncMake(UGenum src, UGenum dst )
	{
		cocos2d::ccBlendFunc blendFunc = {src, dst}; 
		return blendFunc; 
	}

    const cocos2d::ccBlendFunc BLENDFUNC_NORMAL = CCBlendFuncMake(UG_ONE, UG_ONE_MINUS_SRC_ALPHA);

    const cocos2d::ccBlendFunc BLENDFUNC_ADD = CCBlendFuncMake(UG_SRC_ALPHA, UG_ONE);

    const cocos2d::ccBlendFunc BLENDFUNC_LIGHTER = CCBlendFuncMake(UG_DST_COLOR, UG_ONE);
}
