//
//  Dynamics.h
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

///////////////////////////////////////////////////////////////////////////////
// componential actions for movable Objects
///////////////////////////////////////////////////////////////////////////////

    /** Action of Dynamics, auto speed control from target movable object */
    class ActionDynamics: public cocos2d::CCAction
    {
    public: 
        virtual void step(float dt);
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    };

    class ActionDynamicsTimeLimted: public ActionDynamics
    {
    public:
        bool init(float duration);
        COPYWITHZONE_FUNC(ActionDynamicsTimeLimted, ActionDynamics, init(m_duration));    

        virtual bool isDone();
        virtual void step(float dt);
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);


    protected:
        float m_duration;
        float m_remain;
    };

    /** AccelerationBy Action, only act on movable object */
    class AccelerateBy: public ActionDynamicsTimeLimted
    {
    public:
	    /** initializes the action with delta, duration, and related */
	    bool init(float duration, const Vector2& delta, bool related = false);
        COPYWITHZONE_FUNC(AccelerateBy, ActionDynamicsTimeLimted, init(m_duration, m_rate * m_duration, m_related));    
          
	    virtual void update(float rdt);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta velocity to accelerate */
        CREATE_FUNC3(AccelerateBy, 
            float, duration, 
            const Vector2&, velocity,
            bool, related);
         
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta velocity to accelerate */
        CREATE_FUNC2(AccelerateBy, 
            float, duration, 
            const Vector2&, velocity);

    protected:
        Vector2 m_rate; 
        bool m_related;
    };


    /** Spin By Action, only act on movable object */
    class SpinBy: public ActionDynamicsTimeLimted
    {
    public:
	    /** initializes the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees*/
	    bool init(float duration, float delta);

        COPYWITHZONE_FUNC(SpinBy, ActionDynamicsTimeLimted, init(m_duration, m_rate * m_duration));
	    virtual void update(float rdt);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees*/
        CREATE_FUNC2(SpinBy, 
            float, duration,
            float, velocity);

    protected:
        float m_rate; 
    };

    
    /** Turn By Action, only act on movable object */
    class TurnBy: public ActionDynamicsTimeLimted
    {
    public:
	    /** initializes the action with 
         @param duration time to finish turning
         @param angle delta angle to turn in degrees*/
	    bool init(float duration, float angle);

        COPYWITHZONE_FUNC(TurnBy, ActionDynamicsTimeLimted, init(m_duration, m_rate * m_duration));
	    virtual void update(float rdt);


    public:
	    /** create the action with 
         @param duration time to finish turning
         @param angle delta angle to turn in degrees */
        CREATE_FUNC2(TurnBy, 
            float, duration,
            float, angle);

    protected:
        float m_rate; 
    };

    /** Rotate By Action, only act on movable object */
    class RotateBy: public ActionDynamicsTimeLimted
    {
    public:
	    /** initializes the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees*/
	    bool init(float duration, float angle);

        COPYWITHZONE_FUNC(RotateBy, ActionDynamicsTimeLimted, init(m_duration, m_rate * m_duration));
	    virtual void update(float rdt);

    public:
	    /** create the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees */
        CREATE_FUNC2(RotateBy, 
            float, duration,
            float, delta);

    protected:
        float m_rate; 
    };

    
    /** Roturn By Action, only act on movable object */
    class RoturnBy: public ActionDynamicsTimeLimted
    {
    public:
	    /** initializes the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees*/
	    bool init(float duration, float delta);

        COPYWITHZONE_FUNC(RoturnBy, ActionDynamicsTimeLimted, init(m_duration, m_rate * m_duration));
	    virtual void update(float rdt);

    public:
	    /** create the action with 
         @param duration time to finish rotation
         @param delta angle to rotate in degrees */
        CREATE_FUNC2(RoturnBy, 
            float, duration,
            float, delta);

    protected:
        float m_rate; 
    };


    /** Boost to target speed Action, only act on movable object */
    class AccelerateTo: public ActionDynamics
    {
    public: 

	    /** initializes the action with 
         @param stop time to sustain acceleration
         @param velocity target velocity to accelerate 
         @param rate accelerate rate to accelerate */
	    bool init(bool stop, const Vector2& velocity, float rate = 0);
        COPYWITHZONE_FUNC(AccelerateTo, ActionDynamics, init(m_stop, m_velocity, m_rate));    

        virtual bool isDone();
	    virtual void update(float rdt);
        virtual void startWithTarget(cocos2d::CCNode* pTarget);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate */
        CREATE_FUNC2(AccelerateTo, 
            bool, stop, 
            const Vector2&, velocity);
         
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate 
         @param rate accelerate rate to accelerate */
        CREATE_FUNC3(AccelerateTo, 
            bool, stop, 
            const Vector2&, velocity, 
            float, rate);

    protected:
        bool m_stop;
        bool m_done;
        Vector2 m_velocity;        
        float m_rate;  
    };


    /** Spin By Action, only act on movable object */
    class SpinTo: public ActionDynamics
    {
    public:


	    /** initializes the action with 
         @param bool 
         @param velocity delta rotational velocity to accelerate in degrees
         @param rate(optional) max rate to accelerate to target velocity, 0 for auto*/
	    bool init(bool stop, float velocity, float rate = 0);
        COPYWITHZONE_FUNC(SpinTo, ActionDynamics, init(m_stop, m_velocity, m_rate));    

        virtual bool isDone();
	    virtual void update(float rdt);
        virtual void startWithTarget(cocos2d::CCNode* pTarget);

    public:
        /** create the action with 
         @param bool stop
         @param target rotational velocity */
        CREATE_FUNC2(SpinTo, 
            bool, stop,
            float, velocity);

        
        /** create the action with 
         @param bool stop
         @param target rotational velocity 
         @param spin up rate*/
        CREATE_FUNC3(SpinTo, 
            bool, stop,
            float, velocity,
            float, rate);

    protected:
        bool m_stop;
        bool m_done;
        float m_velocity;
        float m_rate; 
    };

    //// not implemented yet
    //class MoveTo: public ActionDynamics
    //{
    //public:

    //    /** initialize with destination, top speed, and acceleration 
    //    @param destination, move to this position
    //    @param speed, top speed of movement
    //    @param accel, rate of acceleration 
    //    @param breaking(optional) breaking deceleration, 0 for the same rate of accel*/
    //    bool init(const Point& destination, float speed, float accel, float breaking = 0);
    //    COPYWITHZONE_FUNC(MoveTo, ActionDynamics, 
    //        init(m_destination, m_speed, m_accel, m_breaking));    

    //    virtual bool isDone();
	   // virtual void update(float rdt);
    //    virtual void startWithTarget(cocos2d::CCNode* pTarget);
    //    virtual void stop();

    //public:
    //    /** create MoveTo action with destination, speed, and accel */
    //    CREATE_FUNC3(MoveTo, 
    //        const Point&, destination, 
    //        float, speed, 
    //        float, accel);

    //    /** create MoveTo action with destination, speed, accel and designed breaking*/
    //    CREATE_FUNC4(MoveTo, 
    //        const Point&, destination, 
    //        float, speed, 
    //        float, accel,
    //        float, breaking);


    //protected:
    //    Point m_destination;
    //    float m_speed;
    //    float m_accel;
    //    float m_breaking;

    //    Point m_delta;
    //    bool m_done;
    //};
}