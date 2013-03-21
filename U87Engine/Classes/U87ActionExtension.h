#pragma once

#include "U87Util.h"
#include "U87Math.h"
#include "U87Dynamics.h"

namespace u87
{
    
///////////////////////////////////////////////////////////////////////////
// common node actions 
///////////////////////////////////////////////////////////////////////////
    
    /** Node Follow Action, use this action to let a node follow another,
     stops when the followed node is removed from its parent */
    class NodeFollow: public cocos2d::CCAction
    {
    public: 
        bool init(cocos2d::CCNode* follow, const Vector2& offset = POINT_ZERO);
        CREATE_FUNC1(NodeFollow, cocos2d::CCNode*, follow);
        CREATE_FUNC2(NodeFollow, cocos2d::CCNode*, follow, const Vector2&, offset);
        COPYWITHZONE_FUNC(NodeFollow, cocos2d::CCAction, init(m_follow)); 

        virtual bool isDone();
        virtual void stop();    
        virtual void update(float dt);
        virtual void startWithTarget(cocos2d::CCNode* pTarget);
        virtual void step(float dt);
        
    protected:
        cocos2d::CCNode* m_follow;
        Vector2 m_offset;
    };
    
    /** Sequence Action, use this action to run actions in order.
     able to sequence any action, including dynamics actions */
    class Sequence: public cocos2d::CCAction
    {
    public: 
        Sequence();
        virtual ~Sequence();

        bool init(cocos2d::CCArray* actionArray);
        static Sequence* create(cocos2d::CCAction* action, ...);
        CREATE_FUNC1(Sequence, cocos2d::CCArray*, actionArray);
        COPYWITHZONE_FUNC(Sequence, cocos2d::CCAction, init(m_actions)); 

        virtual bool isDone();
        virtual void stop();     
        virtual void startWithTarget(cocos2d::CCNode* pTarget);
        virtual void step(float dt);

    protected:
        cocos2d::CCArray* m_runTimeActions; 
        cocos2d::CCArray* m_actions; 
    };

    /** repeat action, use this action to run action repeatly 
     compatible with all actions */
    class Repeat: public cocos2d::CCAction
    {
    public:
        Repeat();
        virtual ~Repeat();
        
        /** init with action and repeat times, 
        @param action, repeat the action,
        @param times(optional) if times == 0, repeat forever */
        bool init(cocos2d::CCAction* action, int times = 0);

        CREATE_FUNC1(Repeat, cocos2d::CCAction*, action);
        CREATE_FUNC2(Repeat, cocos2d::CCAction*, action, int, times);
        COPYWITHZONE_FUNC(Repeat, cocos2d::CCAction, init(m_action, m_times)); 

        virtual bool isDone(); 
        virtual void stop();
        virtual void startWithTarget(cocos2d::CCNode* pTarget);
        virtual void step(float dt);

    protected:
        cocos2d::CCAction* m_action; 
        int m_times;
        int m_current;
    };

    /** spawn actions, use this action to run actions at the same time
     compatible with all actions.
     will be done when all the sub aciton is done */
    class Spawn: public cocos2d::CCAction
    {
    public: 
        Spawn();
        virtual ~Spawn();

        bool init(cocos2d::CCArray* actionArray);
        static Spawn* create(cocos2d::CCAction* action, ...);
        CREATE_FUNC1(Spawn, cocos2d::CCArray*, actionArray);
        COPYWITHZONE_FUNC(Spawn, cocos2d::CCAction, init(m_actions)); 

        virtual bool isDone(); 
        virtual void stop();     
        virtual void startWithTarget(cocos2d::CCNode* pTarget);
        virtual void step(float dt);

    protected:   
        cocos2d::CCArray* m_actions; 
        cocos2d::CCArray* m_runTimeActions; 
    };
}
