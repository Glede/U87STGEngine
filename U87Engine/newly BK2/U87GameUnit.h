#pragma once
 
#include "U87MovableObject.h"
#include "U87GameEvent.h"

namespace u87
{
    //////////////////////////////////////
    // abstract class for game unit
    //////////////////////////////////////

    typedef unsigned int UnitType; 
    const UnitType _UT_NONE = (unsigned int)-1;
#define UT_NONE _UT_NONE

    typedef unsigned int UnitID;
    const UnitID _UID_NONE = (unsigned int)-1;
#define UID_NONE _UID_NONE


    /** GameUnit must be a CCNode with a connection to the world*/ 
    class GameUnit
    {
    friend class World;
    public: 
        GameUnit();
        virtual ~GameUnit();

        /** default z = 0 */
        inline void addToWorld() { addToWorld(UID_NONE, 0); }
        inline void addToWorld(int z) { addToWorld(UID_NONE, z); }

        /** override this add to world method */
        virtual void addToWorld(int id, int z);

        virtual void removeFromWorld();
        
        inline UnitType getUnitType() const { return m_unitType; }
        inline UnitID getUnitID() const { return m_unitID; }

    protected:
        UnitType m_unitType;
        UnitID m_unitID;

        bool m_inWorld;
    };
      

    ///////////////////////////////////////////////////////////////////////////
    // ICamera interface can be tracked by world
    ///////////////////////////////////////////////////////////////////////////

    class ICamera
    {
    public:
        virtual cocos2d::CCSize getSightSize(void) const = 0;

        virtual Point getCameraPosition() const = 0;

        virtual float getCameraRotation() const = 0;

        virtual void shock(float amplitude) = 0;

        virtual void setSightSize(cocos2d::CCSize var) = 0;
        
    };

    /** camera tracking view 
     * how to use:
     * 1.create a camera node;
     * 2.call addToWorld() method to let camera move inside world;
     * 3.call setAsWorldCamera() method to make the world follow it;
     */
    class CameraNode: public MovableObject, public ICamera
    {
    public:
        bool init();
        CREATE_FUNC(CameraNode);
        

        // interface implementation
    public:

        /** give camera a shock */
        virtual void shock(float amplitude);

        /** get a actual position of the camera */
        virtual Point getCameraPosition() const { return m_tPosition + m_shockOffset; }

        virtual float getCameraRotation() const { return m_fRotation; }

        CC_SYNTHESIZE(cocos2d::CCSize, m_sightSize, SightSize);
         

        // helper method
    public:
        virtual void setAsWorldCamera();


        // internal method
    public:
        virtual void updateShock(float dt);

    protected:
        Point   m_location;
        float   m_shockAmplitude;
        float   m_shockYScale;
        int     m_shockDirection;

        Vector2 m_shockOffset;
        
        CC_SYNTHESIZE(float, m_shockMaxAmplitude, ShockMaxAmplitude);
        CC_SYNTHESIZE(float, m_shockLargeReduceRate, ShockLargeReduceRate);
        CC_SYNTHESIZE(float, m_shockTinyReduceRate, ShockTinyReduceRate);
        CC_SYNTHESIZE(float, m_shockReduceRateBorder, ShockReduceRateBorder);
        CC_SYNTHESIZE(float, m_shockEpsilonAmplitude, ShockEpsilonAmplitude); 
    };

}