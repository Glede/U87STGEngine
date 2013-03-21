#include "U87ParticleModel.h"

USING_NS_CC;
USING_NS_U;


CCParticleSystemQuad* ParticleModel::createParticleSystem()
{
	//if(!this)
	//	return NULL;

    CCParticleSystemQuad* pRet = new CCParticleSystemQuad();
    if(!pRet->initWithTotalParticles(this->maxParticles))
    {
        delete pRet;
        return NULL;
    }
    pRet->autorelease();

	CCAssert(pRet, "AD::createParticleSystem - not enough memory");

	// initialized particlesystem data
	pRet->setAngle(this->angle);
	pRet->setAngleVar(this->angleVar);
	pRet->setDuration(this->duration);
	pRet->setBlendFunc(CCBlendFuncMake(this->blendFuncSrc, this->blendFuncDest));
	
	pRet->setStartColor( ccc4F( 
		this->startColorR,
		this->startColorG,
		this->startColorB,
		this->startColorA));
	pRet->setStartColorVar( ccc4F( 
		this->startColorVarR,
		this->startColorVarG,
		this->startColorVarB,
		this->startColorVarA));
	pRet->setEndColor( ccc4F(
		this->endColorR,
		this->endColorG,
		this->endColorB,
		this->endColorA));
	pRet->setEndColorVar( ccc4F(
		this->endColorVarR,
		this->endColorVarG,
		this->endColorVarB,
		this->endColorVarA));

	pRet->setStartSize(this->startSize);
	pRet->setStartSizeVar(this->startSizeVar);
	pRet->setEndSize(this->endSize);
	pRet->setEndSizeVar(this->endSizeVar);

	pRet->setPositionType(this->positionType);
	pRet->setPosition(ccp(this->positionX, this->positionY));
	pRet->setPosVar(ccp(this->positionVarX, this->positionVarY));
	
	pRet->setStartSpin(this->startSpin);
	pRet->setStartSpinVar(this->startSpinVar);
	pRet->setEndSpin(this->endSpin);
	pRet->setEndSpinVar(this->endSpinVar);

	pRet->setLife(this->life);
	pRet->setLifeVar(this->lifeVar);

    pRet->setEmissionRate(this->emissionRate);
	pRet->setEmitterMode(this->emitterMode);

	// emitter mode parameters
	switch(this->emitterMode)
	{
	case kCCParticleModeGravity:
		{
			pRet->setGravity(ccp(this->gravityX, this->gravityY));

			pRet->setSpeed(this->speed);
			pRet->setSpeedVar(this->speedVar);

			pRet->setRadialAccel(this->radialAccel);
			pRet->setRadialAccelVar(this->radialAccelVar);

			pRet->setTangentialAccel(this->tangentialAccel);
			pRet->setTangentialAccelVar(this->tangentialAccelVar);
		}
		break;
	case kCCParticleModeRadius:
		{
			pRet->setStartRadius(this->startRadius);
			pRet->setStartRadiusVar(this->startRadiusVar);

			pRet->setEndRadius(this->endRadius);
			pRet->setEndRadiusVar(this->endRadiusVar);

			pRet->setRotatePerSecond(this->rotatePerSecond);
			pRet->setRotatePerSecondVar(this->rotatePerSecondVar);
		}
		break;
	default:
		CCAssert(false, "AD::createParticleSystem - unexpected emitter mode");
		break;
	}

	// set texture
	pRet->setTexture(this->tex);

	return pRet;
}


#if (COCOS2D_VERSION < 0x00020000)
inline const char * valueForKey(const char *key, CCDictionary<std::string, CCObject*> *dict)
#else
inline const char * valueForKey(const char *key, CCDictionary *dict)
#endif
{
	if (dict)
	{
		CCString *pString = (CCString*)dict->objectForKey(std::string(key));
		return pString ? pString->m_sString.c_str() : "";
	}
	return "";
}

bool ParticleModel::init(const char* file)
{
    bool bRet = false;

#if (COCOS2D_VERSION < 0x00020000)
	std::string plistfile = CCFileUtils::fullPathFromRelativePath(file);
    CCMutableDictionary<std::string>* dictionary = CCFileUtils::dictionaryWithContentsOfFileThreadSafe(plistfile.c_str());
#else
    std::string plistfile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file);
    CCDictionary* dictionary = CCDictionary::createWithContentsOfFileThreadSafe(plistfile.c_str());
#endif

    CCAssert(dictionary, "loadParticleModel - plist file not exsited or invalid");
	do
	{
		this->maxParticles = atoi(valueForKey("maxParticles", dictionary));

		// angle
		this->angle = (float)atof(valueForKey("angle", dictionary));
		this->angleVar = (float)atof(valueForKey("angleVariance", dictionary));

		// duration
		this->duration = (float)atof(valueForKey("duration", dictionary));

		// blend function 
		this->blendFuncSrc = atoi(valueForKey("blendFuncSource", dictionary));
		this->blendFuncDest = atoi(valueForKey("blendFuncDestination", dictionary));

		// color
		this->startColorR = (float)atof(valueForKey("startColorRed", dictionary));
		this->startColorG = (float)atof(valueForKey("startColorGreen", dictionary));
		this->startColorB = (float)atof(valueForKey("startColorBlue", dictionary));
		this->startColorA = (float)atof(valueForKey("startColorAlpha", dictionary));

		this->startColorVarR = (float)atof(valueForKey("startColorVarianceRed", dictionary));
		this->startColorVarG = (float)atof(valueForKey("startColorVarianceGreen", dictionary));
		this->startColorVarB = (float)atof(valueForKey("startColorVarianceBlue", dictionary));
		this->startColorVarA = (float)atof(valueForKey("startColorVarianceAlpha", dictionary));

		this->endColorR = (float)atof(valueForKey("finishColorRed", dictionary));
		this->endColorG = (float)atof(valueForKey("finishColorGreen", dictionary));
		this->endColorB = (float)atof(valueForKey("finishColorBlue", dictionary));
		this->endColorA = (float)atof(valueForKey("finishColorAlpha", dictionary));

		this->endColorVarR = (float)atof(valueForKey("finishColorVarianceRed", dictionary));
		this->endColorVarG = (float)atof(valueForKey("finishColorVarianceGreen", dictionary));
		this->endColorVarB = (float)atof(valueForKey("finishColorVarianceBlue", dictionary));
		this->endColorVarA = (float)atof(valueForKey("finishColorVarianceAlpha", dictionary));

		// particle size
		this->startSize = (float)atof(valueForKey("startParticleSize", dictionary));
		this->startSizeVar = (float)atof(valueForKey("startParticleSizeVariance", dictionary));
		this->endSize = (float)atof(valueForKey("finishParticleSize", dictionary));
		this->endSizeVar = (float)atof(valueForKey("finishParticleSizeVariance", dictionary));

		// position
        if(dictionary->objectForKey("positionType") != NULL)
			this->positionType = (tCCPositionType)atoi(valueForKey("positionType", dictionary));
		else
			this->positionType = kCCPositionTypeFree;
		this->positionX = (float)atof(valueForKey("sourcePositionx", dictionary));
		this->positionY = (float)atof(valueForKey("sourcePositiony", dictionary));
		this->positionVarX  = (float)atof(valueForKey("sourcePositionVariancex", dictionary));
		this->positionVarY  = (float)atof(valueForKey("sourcePositionVariancey", dictionary));

		// Spinning
		this->startSpin = (float)atof(valueForKey("rotationStart", dictionary));
		this->startSpinVar = (float)atof(valueForKey("rotationStartVariance", dictionary));
		this->endSpin= (float)atof(valueForKey("rotationEnd", dictionary));
		this->endSpinVar= (float)atof(valueForKey("rotationEndVariance", dictionary));

		this->emitterMode = atoi(valueForKey("emitterType", dictionary));

		// Mode A: Gravity + tangential accel + radial accel
		if( this->emitterMode  == kCCParticleModeGravity ) 
		{
			// gravity
			this->gravityX = (float)atof(valueForKey("gravityx", dictionary));
			this->gravityY = (float)atof(valueForKey("gravityy", dictionary));

			// speed
			this->speed = (float)atof(valueForKey("speed", dictionary));
			this->speedVar = (float)atof(valueForKey("speedVariance", dictionary));

			// radial acceleration
			this->radialAccel = (float)atof(valueForKey("radialAcceleration", dictionary));
			this->radialAccelVar = (float)atof(valueForKey("radialAccelVariance", dictionary));

			// tangential acceleration
			this->tangentialAccel = (float)atof(valueForKey("tangentialAcceleration", dictionary));
			this->tangentialAccelVar = (float)atof(valueForKey("tangentialAccelVariance", dictionary));
		}

		// or Mode B: radius movement
		else if( this->emitterMode == kCCParticleModeRadius ) 
		{
			this->startRadius = (float)atof(valueForKey("maxRadius", dictionary));
			this->startRadiusVar = (float)atof(valueForKey("maxRadiusVariance", dictionary));
			this->endRadius = (float)atof(valueForKey("minRadius", dictionary));
			this->endRadiusVar = (float)atof(valueForKey("minRadiusVariance", dictionary));
			this->rotatePerSecond = (float)atof(valueForKey("rotatePerSecond", dictionary));
			this->rotatePerSecondVar = (float)atof(valueForKey("rotatePerSecondVariance", dictionary));

		} else {
			CCAssert( false, "Invalid emitterType in config file");
			CC_BREAK_IF(true);
		}
		

		// life span
		this->life = (float)atof(valueForKey("particleLifespan", dictionary));
		this->lifeVar = (float)atof(valueForKey("particleLifespanVariance", dictionary));

		// emission Rate
		this->emissionRate = (float)atof(valueForKey("emissionRate", dictionary));

		// set texture
		const char* textureName = valueForKey("textureFileName", dictionary);
#if (COCOS2D_VERSION < 0x00020000)
        std::string fullpath = CCFileUtils::fullPathFromRelativeFile(textureName, plistfile.c_str());
#else
        std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(textureName, plistfile.c_str());
#endif
		this->tex = CCTextureCache::sharedTextureCache()->addImage(fullpath.c_str());
		CC_BREAK_IF(!this->tex);
        
        bRet = true;
	} while (0);

    return bRet;
}