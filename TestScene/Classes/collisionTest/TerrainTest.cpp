#include "CollisionTestScene.h"

USING_NS_CC;
USING_NS_U;

bool TerrainTest::init()
{
	if(!TestLayer::init())
		return false;

	//m_title = "Terrain Test";

	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//this->terrain.loadBordersFromFile("TestTerrain.txt");

	//MovableObject* object = MovableObject::create();
	//object->addChild(CCSprite::spriteWithFile("TestObject.png"));
	//object->setLocation(ccp(400, 650));
	//addChild(object);
	//Point revisor = this->terrain.movingRevision(ccp(0, 0), object->getLocation());

	//MovableObject* objectR = MovableObject::create();
 //   CCSprite* visual = CCSprite::spriteWithFile("TestObject.png");
 //   visual->setOpacity(155);
 //   objectR->addChild(visual);
	//objectR->setLocation(object->getLocation() + revisor);
 //   addChild(objectR);
	
	return true;
}

void TerrainTest::draw() {
	BorderNode* sgmt = this->terrain.getBorderEntry();
	float tileSize = this->terrain.getTileSize();
	do {
		ccDrawLine(ccp(tileSize * sgmt->segment.start.col, tileSize * sgmt->segment.start.row), ccp(tileSize * sgmt->segment.end.col, tileSize * sgmt->segment.end.row));
		sgmt = sgmt->next;
	} while (sgmt != this->terrain.getBorderEntry());
}