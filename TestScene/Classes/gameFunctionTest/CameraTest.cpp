#include "GameFunctionTestScene.h"

USING_NS_CC;
USING_NS_U;
 
const int TAG_MOVE_WHEEL = 1;
const int TAG_TURN_WHEEL = 2;

bool CameraTest::init()
{

    bool bRet = false;

    do
    {
        CC_BREAK_IF(!TestLayer::init());

        m_title = "Camera Test";

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // init world
        if(World::sharedInstance() != NULL)
        {
            if(World::sharedInstance()->getParent() != NULL)
                World::sharedInstance()->removeFromParentAndCleanup(true);
            
            delete World::sharedInstance();
        }

        addChild(World::create());
    
        {
            TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
                CCSprite::spriteWithFile("TouchPoint.png"));
            panel->setPosition(ccp(200, 200));
            panel->setInnerRadius(25);
            panel->setTag(TAG_MOVE_WHEEL);
            addChild(panel);

            panel->setHandler(this);   
        }

        {
            TouchWheel* panel = TouchWheel::create( CCSprite::spriteWithFile("TouchWheel.png"),
                CCSprite::spriteWithFile("TouchPoint.png"));
            panel->setPosition(ccp(winSize.width - 200, 200));
            panel->setInnerRadius(25);
            panel->setTag(TAG_TURN_WHEEL);
            addChild(panel);

            panel->setHandler(this);   
        }


        {
            TestTarget* tt = TestTarget::create();
            tt->addChild(tt->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
            tt->setPosition(ccp(winSize.width/3, winSize.height/2));
            tt->setUnitType(TEST_UNIT);
            tt->addToWorld();
        }

        {
            TestTarget* tt = TestTarget::create();
            tt->addChild(tt->addBody(RoundBodyNode::create(50), TEST_UNIT, TEST_UNIT, CLT_NONE));
            tt->addToWorld();
            tt->setPosition(ccp(winSize.width/3*2, winSize.height/2));
        }

        // add camera
        {
            CameraNode* cam = CameraNode::create();
            cam->setPosition(ccpFromSize(winSize)/2);
            World::sharedInstance()->addChild(cam);
            cam->setAsWorldCamera();
            m_cam = cam; 

            m_centerObject = CCSprite::spriteWithFile("TouchPoint.png");
            World::sharedInstance()->addChild(m_centerObject);
        }

        // give a shock button
        CCMenuItemLabel* labelShock = CCMenuItemLabel::itemWithLabel(
            CCLabelTTF::labelWithString("Shock", "Consolas", 24), 
            this, menu_selector(CameraTest::shockPressed));
        labelShock->setAnchorPoint(ccp(0.5f,0.5f));
        addChild(CCMenu::menuWithItem(labelShock));
         
        scheduleUpdate();

        bRet = true;
    } while(0);

    return bRet;
}
 
void CameraTest::update(float dt)
{
    m_centerObject->setPosition( m_cam->getPosition() );
}

void CameraTest::shockPressed(CCObject* sender)
{
    m_cam->shock(15);
}

void CameraTest::wheelTouchUpdated(TouchWheel* sender, const Point& point, float dt)
{
    switch(sender->getTag())
    {
    case TAG_MOVE_WHEEL:
        {
            float length = ccpLength(point);

            Vector2 accel = length > 0? (point/length * 240): VECTOR2_ZERO; 
            accel = ccpRotateByAngle(accel, POINT_ZERO, 
                CC_DEGREES_TO_RADIANS(-m_cam->getRotation()));
            m_cam->accelerateTo(accel, length*480*dt);
        }
        break;
    case TAG_TURN_WHEEL:
        {
            float deltaRot = normalizeDegree( angleForVector2(point) + 90 );

            m_cam->rotate(deltaRot * dt);
        }
    default:
        break;
    }
}