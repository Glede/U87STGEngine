#include "U87ActionExtension.h"

using namespace std;
USING_NS_CC;
USING_NS_U;

///////////////////////////////////////////////////////////////////////////
// node Follow
///////////////////////////////////////////////////////////////////////////
    
bool NodeFollow::init(CCNode* follow, const Vector2& offset)
{
    bool bRet = false;

    do
    {
        CC_ASSERT(follow != NULL);
        m_follow = follow;
        m_follow->retain();
        m_offset = offset;

        bRet = true;
    } while(0);

    return bRet;
}

bool NodeFollow::isDone()
{
    CC_ASSERT(m_follow);
    return m_follow->getParent() == NULL;
}

void NodeFollow::stop()
{
    CC_ASSERT(m_follow);    
    m_follow->release();
    m_follow = NULL;
    m_pTarget = NULL;
}
 
void NodeFollow::startWithTarget(CCNode* pTarget)
{
    CC_ASSERT(pTarget);
    m_pTarget = pTarget;
}

void NodeFollow::update(float dt)
{
    CC_UNUSED_PARAM(dt);
    CC_ASSERT(m_pTarget != NULL && m_follow != NULL);
    Point folPos = m_follow->getPosition();
    if(m_offset != VECTOR2_ZERO)
        folPos = folPos + ccpRotateByAngle(m_offset, VECTOR2_ZERO, 
        CC_DEGREES_TO_RADIANS(m_follow->getRotation()));
    m_pTarget->setPosition(m_follow->getPosition());
}

void NodeFollow::step(float dt)
{
    update(dt);
}


///////////////////////////////////////////////////////////////////////////
// Sequence Action
///////////////////////////////////////////////////////////////////////////


Sequence* Sequence::create(CCAction* action, ...)
{
    CC_ASSERT(action != NULL);
    Sequence* pRet = new Sequence();

    va_list(params);
    va_start(params, action);

    CCArray* ary = CCArray::array();
    for(CCAction* act = action; act != NULL; act = va_arg(params, CCAction*))
    {
        ary->addObject(act);
    }

    va_end(params);

    if(pRet->init(ary))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return NULL;
    }
}

Sequence::Sequence()
{
    m_actions = NULL;
    m_runTimeActions = CCArray::array();
    m_runTimeActions->retain();
}

Sequence::~Sequence()
{
    CC_SAFE_RELEASE(m_actions);
    CC_SAFE_RELEASE(m_runTimeActions);
}

bool Sequence::init(CCArray* actionArray)
{
    CC_ASSERT(actionArray != NULL && actionArray->count() > 0U);
    m_actions = actionArray;
    m_actions->retain();

    return true;
}


void Sequence::stop()
{
    CCAction::stop();
    m_runTimeActions->removeAllObjects(); 
}
 
void Sequence::startWithTarget(CCNode* pTarget)
{
    CC_ASSERT(pTarget != NULL);
    m_pTarget = pTarget;

    m_runTimeActions->removeAllObjects();
    m_runTimeActions->addObjectsFromArray(m_actions);
    m_runTimeActions->reverseObjects(); 
    CC_ASSERT(m_runTimeActions->count() > 0U);


    CCAction* act = (CCAction*)m_runTimeActions->lastObject();
    act->startWithTarget(m_pTarget);
}

void Sequence::step(float dt)
{
    if(m_runTimeActions->count() > 0U)
    {
        CCAction* act = (CCAction*)m_runTimeActions->lastObject();
        while(act->isDone())
        {
            act->stop();
            m_runTimeActions->removeLastObject();

            if(m_runTimeActions->count() > 0U)
            {
                act = (CCAction*)m_runTimeActions->lastObject();
                act->startWithTarget(m_pTarget);
            }
            else
                return;
        }

        act->step(dt);
    }
}


bool Sequence::isDone()
{
    return m_runTimeActions->count() == 0U;
}


///////////////////////////////////////////////////////////////////////////
// Repeat Action
///////////////////////////////////////////////////////////////////////////

Repeat::Repeat()
{
    m_action = NULL;
    m_times = 0;
    m_current= 0;
}

Repeat::~Repeat()
{
    CC_SAFE_RELEASE(m_action);
}

bool Repeat::init(cocos2d::CCAction* action, int times)
{
    m_action = action;
    CC_ASSERT(m_action != NULL);
    m_action->retain();

    if(times == 0)
        m_times = -1;
    else
        m_times = times;
    m_current = 0;
    return true;
}

bool Repeat::isDone()
{
    return m_times == m_current;
}
 
void Repeat::startWithTarget(CCNode* pTarget)
{
    CCAction::startWithTarget(pTarget);

    CC_ASSERT(m_action != NULL);
    m_action->startWithTarget(pTarget);

    m_current = 0;
}


void Repeat::step(float dt)
{ 
    CC_ASSERT(m_action != NULL);
    if(m_action->isDone())
    {
        m_action->stop();
        m_action->startWithTarget(m_pTarget);
        m_action->step(dt);
        m_current++;
    }
    else
    {
        m_action->step(dt);
    }
}

void Repeat::stop()
{
    m_current = 0;
}

///////////////////////////////////////////////////////////////////////////
// Spawn Action
///////////////////////////////////////////////////////////////////////////

Spawn::Spawn()
{
    m_actions = NULL;
    m_runTimeActions = CCArray::array();
    m_runTimeActions->retain();
}

Spawn::~Spawn()
{
    CC_SAFE_RELEASE(m_actions);
    CC_SAFE_RELEASE(m_runTimeActions);
}

bool Spawn::init(cocos2d::CCArray* actionArray)
{
    m_actions = actionArray;
    CC_ASSERT(m_actions != NULL);
    m_actions->retain();

    return true;
}

Spawn* Spawn::create(CCAction* action, ...)
{
    CC_ASSERT(action != NULL);
    Spawn* pRet = new Spawn();

    va_list(params);
    va_start(params, action);

    CCArray* ary = CCArray::array();
    for(CCAction* act = action; act != NULL; act = va_arg(params, CCAction*))
    {
        ary->addObject(act);
    }

    va_end(params);

    if(pRet->init(ary))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return NULL;
    }
}

bool Spawn::isDone()
{
    return m_runTimeActions->count() == 0;
}


void Spawn::startWithTarget(CCNode* pTarget)
{
    CCAction::startWithTarget(pTarget);

    CCAction* ary;
    CCARRAY_TFOREACH(m_actions, ary, CCAction*)
    {
        ary->startWithTarget(pTarget);
        m_runTimeActions->addObject(ary);
    }     
}

void Spawn::step(float dt)
{
    vector<UINT> indexToRemove;

    CCAction* ary;
    for(UINT i = 0; i < m_runTimeActions->count(); i++)
    {
        ary = (CCAction*) m_runTimeActions->objectAtIndex(i);
        if(ary->isDone())
        {
            ary->stop();
            indexToRemove.push_back(i); 
        }
        else
        {
            ary->step(dt);
        }
    }

    for(UINT i = indexToRemove.size(); i > 0; i--)
    {
        m_runTimeActions->removeObjectAtIndex(indexToRemove[i-1]);
    }
}

void Spawn::stop()
{
    CCAction::stop();
    m_runTimeActions->removeAllObjects();  
}
