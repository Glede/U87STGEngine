#pragma once
 
#include "U87MovableObject.h"
#include "U87GameEvent.h"
#include "U87Collision.h"

namespace u87
{
    //////////////////////////////////////
    // abstract class for game unit
    //////////////////////////////////////

    typedef int UnitID;
    typedef unsigned int UnitType;
    const UnitID _UID_NONE = (int)-1;
#define UID_NONE _UID_NONE

    const UnitType _UT_NONE = (unsigned int)-1;
#define UT_NONE _UT_NONE
    
    class GameUnit;
    class Camera;
    class CameraNode;

    /** game unit is a ccnode based interface for game unit node to interact 
    with each other and the world */ 
    class GameUnit 
    {
    public: 
        GameUnit();
        virtual ~GameUnit();

        /** default z = 0 */
        virtual void addToWorld();
        
        /** override this add to world method */
        virtual void addToWorld(int z);

        virtual void removeFromWorld();

        /** collision with other object selector
         override this selector to respon collision events, 
         return true to intercept the collision event */
        virtual bool onCollision(const HitRecord& hr) { CC_UNUSED_PARAM(hr); return false; }

        inline UnitType getUnitType() const { return m_unitType; }
        inline void setUnitType(UnitType type) { m_unitType = type; }
        inline UnitID getUnitID() const { return m_unitID; }
        inline void setUnitID(UnitID id) 
        { 
            if(m_unitID == UID_NONE) 
                m_unitID = id; 
            else 
                CC_ASSERT(m_unitID == UID_NONE);
        }
        
        /** quick add body */
        virtual BodyNode* addBody(BodyNode* body);
        /** quick add body with tags, end with CLT_NONE */
        virtual BodyNode* addBody(BodyNode* body, CTag self, CTag target, ...);
        /** quick add body with designed position rotation and tags CLT_NONE */
        virtual BodyNode* addBody(BodyNode* body, 
            const Point& position, float rotation, CTag self, CTag target, ...);

    protected:
        UnitType m_unitType;
        UnitID m_unitID;
    }; 
}