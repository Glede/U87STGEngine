#pragma once

#include "U87Util.h"
#include "U87Graphics.h"

namespace u87
{
    
	class ParticleModel: public cocos2d::CCObject
	{		
    public:
        bool init(const char* file);

	    static ParticleModel* create(const char* file);
        cocos2d::CCParticleSystemQuad* createParticleSystem();
        

	public:
		int maxParticles;

		float angle;
		float angleVar;

		float duration;

		UGenum blendFuncDest;
		UGenum blendFuncSrc;
		
		float startColorR;
		float startColorG;
		float startColorB;
		float startColorA;

		float startColorVarR;
		float startColorVarG;
		float startColorVarB;
		float startColorVarA;

		float endColorR;
		float endColorG;
		float endColorB;
		float endColorA;

		float endColorVarR;
		float endColorVarG;
		float endColorVarB;
		float endColorVarA;

		float startSize;
		float startSizeVar;
		float endSize;
		float endSizeVar;

		cocos2d::tCCPositionType positionType;

		float positionX;
		float positionY;
		float positionVarX;
		float positionVarY;

		float startSpin;
		float startSpinVar;
		float endSpin;
		float endSpinVar;

		float life;
		float lifeVar;

		float emissionRate;
		int	  emitterMode;

		// Gravity Mode Parameters {
		float gravityX;
		float gravityY;
		float speed;
		float speedVar;
		float radialAccel;
		float radialAccelVar;
		float tangentialAccel;
		float tangentialAccelVar;
		// Gravity Mode Parameters }

		// Radius Mode Parameters {
		float startRadius;
		float startRadiusVar;
		float endRadius;
		float endRadiusVar;
		float rotatePerSecond;
		float rotatePerSecondVar;
		// Radius Mode Parameters }

		cocos2d::CCTexture2D* tex;
	};
}