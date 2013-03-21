//
//  MovableObject.h
//  U87Engine
//
//  Created by Ñî »Æ on 13-1-28.
//  Copyright (c) 2013Äê __MyCompanyName__. All rights reserved.
//
//  Declare Dynamics 
//
#pragma once

#include "U87Util.h"
#include "U87Math.h"

namespace u87
{ 
    const float INFINITE_MASS = -1;
    const float INFINITE_RATE = -1;

    /** basic motion object for all objects */
    class MovableObject : public cocos2d::CCNode
    {
    public:
        MovableObject();
        virtual ~MovableObject();

        static MovableObject* create();
        void init();
        virtual void updateMotion(float dt);

        inline const Point& getLocation()      const { return m_location; }
        inline const Vector2& getVelocity()    const { return m_velocity; }
        inline float getRotationalVelocity()   const { return m_rotationalVelocity; }
        inline float getTimeScale()            const { return m_timeScale; }
        inline float getMass()                 const { return m_mass; }
        inline float getResistanceFactor()     const { return m_resistanceFactor; }

        virtual void setLocation(const Point& var);
        virtual void setVelocity(const Vector2& var);
        virtual void setRotationalVelocity(float var);
        virtual void setTimeScale(float var);
        virtual void setMass(float var);
        virtual void setResistanceFactor(float var);
        
        /** override CCNode::setPosition to make it work as setLocation*/
        virtual void setPosition(const cocos2d::CCPoint& var);
        /** override CCNode::setRotation to make it work correctly change the nextRotation*/
        virtual void setRotation(float var);

        /** give an acceleration change velocity */
        inline void accelerate(const Vector2& var)
        {
            m_nextVelocity = m_nextVelocity + var;
        }
         
        /** give a target velocity to change velocity
         * @param target   target velocity to accelerate to
         * @param maxRate accelerate max rate, < 0 for no limits*/        
        void accelerateTo(const Vector2& target, float maxRate = INFINITE_RATE);

        /** give an forward acceleration change velocity */
        void accelerateForward(float var);

        /** give a target velocity speed to change velocity
         * @param target  target velocity speed to accelerate to
         * @param maxRate accelerate max rate, < 0 for no limits*/
        void accelerateForwardTo(float speed, float maxRate = INFINITE_RATE);
        
        /** give a force to change the velocity*/
        void force(const Vector2& var); 

        /** give a length of force forward to change the velocity*/
        void forceForward(float var); 

        /** give a rotational acceleration to change the angle of object */
        inline void spin(float var)
        {
            m_nextRotationalVelocity += var;
        }

        /** give a target rotational velocity to change velocity
         * @param target  target rotational veloicty to accelerate to
         * @param maxRate max rotating rate, < 0 for no limits*/
        void spinTo(float var, float maxRate = INFINITE_RATE);


        /** give a instant rotational velocity to change the angle of object */
        inline void rotate(float var)
        { 
            m_nextRotation += var;
        }

        /** give a target rotation to change the angle of a object 
         * @param target  target rotation to rotate to
         * @param maxRate max turning rate, < 0 for no limits*/
        void rotateTo(float target, float maxRate = INFINITE_RATE);

        /** give a directional angle to change the angle of velocity */
        inline void turn(float var)
        {
            Vector2 delta = ccpRotateByAngle(m_velocity, POINT_ZERO, CC_DEGREES_TO_RADIANS(-var)) - m_velocity;
            m_nextVelocity = m_nextVelocity + delta;
        }

        /** give a target direction to chagne the angle of velocity 
         * @param target  target direction to turn to
         * @param maxRate max turning rate, < 0 for no limits*/
        void turnTo(float target, float maxRate = INFINITE_RATE);

        /** rotate and turn at the same time */
        inline void roturn(float var)
        {
            rotate(var);
            turn(var);
        }

        /** rotate and roturn at the same time 
         * @param target  target direction to roturn to
         * @param maxRate max roturning rate, < 0 for no limits*/
        void roturnTo(float target, float maxRate = INFINITE_RATE);

        /** add a foot point */
        void setFeet(Point* point, ...);
    private:
        Point m_location;  
        Point m_nextLocation;
        float m_rotation; // differs from rotation
        float m_nextRotation;

        Vector2 m_velocity;
        Vector2 m_nextVelocity;
        float m_rotationalVelocity;
        float m_nextRotationalVelocity;

        //Vector2 m_instantVelocityDelta;
        //float m_instantRotationalVelocityDelta;
        //float m_instantDirectionalVelocity;
        //float m_instantRotationalVelocity;

        float m_resistanceFactor;
        float m_timeScale;
        float m_mass;

        bool m_isGroundUnit;

        // set feet for groundObjectTest
        int m_footCount;
        Point* m_feet;
    };

}