#pragma once

#include "U87Util.h"
#include <list>
#include <hash_map>

namespace u87
{
    class GameEvent;
    class GameEventHandler;
    class GameEventDispatcher;
    typedef unsigned int EventType;    
    typedef std::list<GameEventHandler*> GameEventHandlerList;
    typedef std::hash_map<int, std::list<GameEventHandler*>> ListMap;

    const EventType _ET_NONE = (unsigned int)-1;
#define ET_NONE _ET_NONE

#define EVENTTYPE_FUNC(type)  virtual u87::EventType getEventType() const { return type; }
    


    /** GameEvent Interface
     is a super class of all game event, can show the type of itself.
     subclasses of a game event should carry event informations to info
     the handler. */
    class GameEvent: public cocos2d::CCObject
    {
    public:
        virtual EventType getEventType() const = 0;

        void dispatch();
    };

    /** GameEventHandler Interface
     is a super class of all game event, can receive game events from
     the world and handle them. 
     */
    class GameEventHandler
    { 
    public:
        virtual ~GameEventHandler();
 
        /** reg this as a listener of the spcific type of game event */
        void listenGameEvent(EventType type, ...);

        /** override this to respond game event  */
        virtual void onGameEvent(EventType type, GameEvent* gameEvent) = 0;
    };
     

    /** a game event dispate */
    class GameEventDispatcher
    {
    public: 
        ~GameEventDispatcher();
        static GameEventDispatcher* sharedInstance();
    protected:
        static GameEventDispatcher* s_sharedInstance;

    public:

        /** add a handler to receive specific type of event */
        virtual void addGameEventHandler(GameEventHandler* handler, EventType type);
        
        /** remove a handler to stop recieving specific type of evet */
        virtual void removeGameEventHandler(GameEventHandler* handler, EventType type);

        /** remove a handler from all event index */
        virtual void removeGameEventHandler(GameEventHandler* handler);

        /** something happened and a event is thrown out. the specific type of 
        handlers will receive the event. */
        virtual void dispatchGameEvent(GameEvent* gameEvent);

        /** something happened and a event is thrown out. the specific type of 
        handlers will receive the event. quick method if this event has no params */
        virtual void dispatchGameEvent(EventType type);
    protected:
        GameEventHandlerList* getGameEventHandlerIndex(EventType type);

        ListMap m_eventHandlerMap;
    };


}