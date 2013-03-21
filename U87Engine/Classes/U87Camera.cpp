#include "U87Camera.h"
#include "U87World.h"

USING_NS_CC;
USING_NS_U;

///////////////////////////////////////////////////////////////////////////////
// Camera Node
///////////////////////////////////////////////////////////////////////////////

bool CameraNode::init()
{
    MovableObject::init();

    m_location = POINT_ZERO;
    
    m_shockAmplitude    = 0;
    m_shockYScale       = 0.3f;
    m_shockDirection    = 0;

    m_sightSize             = CCDirector::sharedDirector()->getWinSize();

    m_shockMaxAmplitude     = 100.0f;
    m_shockEpsilonAmplitude = 0.0001f;
    m_shockLargeReduceRate  = 0.85f;
    m_shockTinyReduceRate   = 0.91f;
    m_shockReduceRateBorder = 15.0f;

    return true;
}

void CameraNode::shock(float amplitude)
{
    if(amplitude > 0)
    {
        m_shockAmplitude += amplitude; 
        if(m_shockAmplitude > m_shockMaxAmplitude)
            m_shockAmplitude = m_shockMaxAmplitude;

        schedule(schedule_selector(CameraNode::updateShock));
    }
}

void CameraNode::updateShock(float dt)
{
    // update offset
    if(m_shockAmplitude != 0)
    {
        m_shockOffset = ccp(
            m_shockAmplitude * sinf(3.1415f/2 * m_shockDirection),
            m_shockAmplitude * sinf(3.1415f/2 *( 2 * m_shockDirection + 1 ) * m_shockYScale)            
            );
    }
    else 
    {
        m_shockOffset = VECTOR2_ZERO;
    }

    // decline
    if(m_shockAmplitude > m_shockReduceRateBorder)
    {
        m_shockAmplitude *= m_shockLargeReduceRate;
    } 
    else 
    {
        m_shockAmplitude *= m_shockTinyReduceRate;
    }

    if(m_shockAmplitude < m_shockEpsilonAmplitude)
    {
        m_shockAmplitude = 0;
        unschedule(schedule_selector(CameraNode::updateShock));
    }

    m_shockDirection++;    
}
 
void CameraNode::setAsWorldCamera()
{
    World::sharedInstance()->setWorldCamera(this);
}