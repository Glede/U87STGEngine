#include "U87GameEvent.h"
#include "U87World.h"

USING_NS_CC;
USING_NS_U;
 

///////////////////////////////////////////////////////////////////////////////
// Game Event Hanlder
///////////////////////////////////////////////////////////////////////////////

void GameEvent::dispatch()
{
    GameEventDispatcher::sharedInstance()->dispatchGameEvent(this);
}

///////////////////////////////////////////////////////////////////////////////
// Game Event Hanlder
///////////////////////////////////////////////////////////////////////////////

GameEventHandler::~GameEventHandler()
{
    GameEventDispatcher::sharedInstance()->removeGameEventHandler(this);
}

void GameEventHandler::listenGameEvent(EventType type, ...)
{
    GameEventDispatcher::sharedInstance()->removeGameEventHandler(this);

    va_list params;
    va_start(params, type);

    EventType et = type;

    while(et != EVT_NONE)
    {
        GameEventDispatcher::sharedInstance()->addGameEventHandler(this, et);
        et = va_arg(params, EventType);
    }

    va_end(params);
}

///////////////////////////////////////////////////////////////////////////////
// Game Event Dispatcher
///////////////////////////////////////////////////////////////////////////////

GameEventDispatcher::~GameEventDispatcher()
{
    if(s_sharedInstance == this)
        s_sharedInstance = NULL;
}

GameEventDispatcher* GameEventDispatcher::s_sharedInstance = NULL;

GameEventDispatcher* GameEventDispatcher::sharedInstance()
{
    if(s_sharedInstance == NULL)
        s_sharedInstance = new GameEventDispatcher();

    return s_sharedInstance;
}

void GameEventDispatcher::addGameEventHandler(GameEventHandler* handler, EventType type)
{
    GameEventHandlerList* index = getGameEventHandlerIndex(type);    
    if(index == NULL)
    {
        m_eventHandlerMap[type] = GameEventHandlerList();
        index = &m_eventHandlerMap[type];
    }
     
    index->push_back(handler);
}

void GameEventDispatcher::removeGameEventHandler(GameEventHandler* handler, EventType type)
{
    GameEventHandlerList* index = getGameEventHandlerIndex(type);    
    if(index != NULL)
    {
        index->remove(handler);
    }
}

void GameEventDispatcher::removeGameEventHandler(GameEventHandler* handler)
{
    ListMap::iterator i;
    for(i = m_eventHandlerMap.begin(); i != m_eventHandlerMap.end(); i++)
    {
        i->second.remove(handler);
    }
}


void GameEventDispatcher::dispatchGameEvent(GameEvent* gameEvent)
{
    GameEventHandlerList* index = getGameEventHandlerIndex(gameEvent->getEventType());

    if(index)
    {
        for(GameEventHandlerList::iterator i = index->begin(); i != index->end(); i++)
        {
            (*i)->onGameEvent(gameEvent->getEventType(), gameEvent);
        }
    }
}


void GameEventDispatcher::dispatchGameEvent(EventType type)
{
    GameEventHandlerList* index = getGameEventHandlerIndex(type);

    if(index)
    {
        for(GameEventHandlerList::iterator i = index->begin(); i != index->end(); i++)
        {
            (*i)->onGameEvent(type, NULL);
        }
    }
}
GameEventHandlerList* GameEventDispatcher::getGameEventHandlerIndex(EventType type)
{
    GameEventHandlerList* pRet;
    if(m_eventHandlerMap.find(type) == m_eventHandlerMap.end())
    {
        pRet = NULL;
    }
    else
    {
        pRet = &m_eventHandlerMap[type];
    }    

    return pRet;
}