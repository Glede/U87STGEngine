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
    class ActionDynamics: public cocos2d::CCActionInterval
    {
    public: 
        virtual void step(cocos2d::ccTime dt);
    };

    /** AccelerationBy Action, only act on movable object */
    class AccelerateBy: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finished acceleration
         @param velocity delta velocity to accelerate 
         @param related (optional)is delta veloicty's direction related of object rotation */
	    bool init(cocos2d::ccTime duration, const Vector2& velocity, bool related = false);

        COPYWITHZONE_FUNC(AccelerateBy, ActionDynamics, init(m_fDuration, m_delta, m_related));    
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta velocity to accelerate */
        CREATE_FUNC2(AccelerateBy, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty);

	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta velocity to accelerate 
         @param related (optional)is delta veloicty's direction related of object rotation */
        CREATE_FUNC3(AccelerateBy, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty, 
            bool, related);

    protected:
        Vector2 m_delta;        
        bool m_related;
        cocos2d::ccTime m_lastUpdate;
    };


    /** AccelerationBy Action, only act on movable object */
    class AccelerateTo: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate 
         @param related is delta veloicty's direction related of object rotation */
	    bool init(cocos2d::ccTime duration, const Vector2& velocity, bool related = false);

	    /** initializes the action with 
         @param duration time to sustain acceleration
         @param velocity target velocity to accelerate 
         @param rate accelerate rate to accelerate 
         @param related is delta veloicty's direction related of object rotation */
	    bool init(cocos2d::ccTime duration, const Vector2& velocity, float rate, bool related = false);

        COPYWITHZONE_FUNC(AccelerateTo, ActionDynamics, init(m_fDuration, m_velocity, m_rate, m_related));    
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate */
        CREATE_FUNC2(AccelerateTo, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty);

	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate 
         @param related is target veloicty related to object rotation */
        CREATE_FUNC3(AccelerateTo, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty, 
            bool, related);

	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate 
         @param rate accelerate rate to accelerate */
        CREATE_FUNC3(AccelerateTo, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty, 
            float, rate);

	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity target velocity to accelerate 
         @param rate accelerate rate to accelerate 
         @param related(optional) is target veloicty related to object rotation */
        CREATE_FUNC4(AccelerateTo, 
            cocos2d::ccTime, duration, 
            const Vector2&, veloicty, 
            float, rate,
            bool, related);


    protected:
        Vector2 m_velocity;        
        float m_rate;
        cocos2d::ccTime m_lastUpdate;
        bool m_related; 
    };

    /** Spin By Action, only act on movable object */
    class SpinBy: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees*/
	    bool init(cocos2d::ccTime duration, float veloicty);

        COPYWITHZONE_FUNC(SpinBy, ActionDynamics, init(m_fDuration, m_delta));
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees*/
        CREATE_FUNC2(SpinBy, 
            cocos2d::ccTime, duration,
            float, veloicty);

    protected:
        float m_delta;
        cocos2d::ccTime m_lastUpdate;
    };

    /** Spin By Action, only act on movable object */
    class SpinTo: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees
         @param rate(optional) max rate to accelerate to target velocity, 0 for auto*/
	    bool init(cocos2d::ccTime duration, float veloicty, float rate = 0);

        COPYWITHZONE_FUNC(SpinTo, ActionDynamics, init(m_fDuration, m_velocity, m_rate));
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees*/
        CREATE_FUNC2(SpinTo, 
            cocos2d::ccTime, duration,
            float, veloicty);

	    /** create the action with 
         @param duration time to finished acceleration
         @param velocity delta rotational velocity to accelerate in degrees
         @param rate  max rate to accelerate to target velocity, 0 for auto*/
        CREATE_FUNC3(SpinTo, 
            cocos2d::ccTime, duration,
            float, veloicty,
            float, maxRate);

    protected:
        float m_velocity;
        float m_rate;
        cocos2d::ccTime m_lastUpdate;
    };

    /** Turn By Action, only act on movable object */
    class TurnBy: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finish turning
         @param angle delta angle to turn in degrees*/
	    bool init(cocos2d::ccTime duration, float angle);

        COPYWITHZONE_FUNC(TurnBy, ActionDynamics, init(m_fDuration, m_delta));
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finish turning
         @param angle delta angle to turn in degrees */
        CREATE_FUNC2(TurnBy, 
            cocos2d::ccTime, duration,
            float, angle);

    protected:
        float m_delta;
        cocos2d::ccTime m_lastUpdate;
        float m_timeScale;
    };

    /** Rotate By Action, only act on movable object */
    class RotateBy: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees*/
	    bool init(cocos2d::ccTime duration, float angle);

        COPYWITHZONE_FUNC(RotateBy, ActionDynamics, init(m_fDuration, m_delta));
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees */
        CREATE_FUNC2(RotateBy, 
            cocos2d::ccTime, duration,
            float, angle);

    protected:
        float m_delta;
        cocos2d::ccTime m_lastUpdate;
    };

    
    /** Roturn By Action, only act on movable object */
    class RoturnBy: public ActionDynamics
    {
    public:
	    /** initializes the action with 
         @param duration time to finish rotation
         @param angle delta angle to rotate in degrees*/
	    bool init(cocos2d::ccTime duration, float angle);

        COPYWITHZONE_FUNC(RoturnBy, ActionDynamics, init(m_fDuration, m_delta));
	    virtual void startWithTarget(cocos2d::CCNode *pTarget);
	    virtual void update(cocos2d::ccTime time);

    public:
	    /** create the action with 
         @param duration time to finish rotation
         @param delta angle to rotate in degrees */
        CREATE_FUNC2(RoturnBy, 
            cocos2d::ccTime, duration,
            float, angle);

    protected:
        float m_delta;
        cocos2d::ccTime m_lastUpdate;
    };

     
}