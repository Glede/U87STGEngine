#pragma once

#include "U87Platform.h" 
 
/**
 * CCARRAY FOREACHE with type conversion
 */
#define CCARRAY_TFOREACH( __array__, __object__, __type__ )                                                 \
if ((__array__) && (__array__)->data->num > 0)                                                              \
for(CCObject** arr = (__array__)->data->arr, **end = (__array__)->data->arr + (__array__)->data->num-1;     \
arr <= end && (((__object__) = (__type__)*arr) != NULL/* || true*/);                                       \
arr++)

/**
 * CCARRAY FOREACHE REVERSE with type conversion
 */
#define CCARRAY_TFOREACH_REVERSE( __array__, __object__, __type__ )                                        \
if ((__array__) && (__array__)->data->num > 0)                                                             \
for(CCObject** arr = (__array__)->data->arr + (__array__)->data->num-1, **end = (__array__)->data->arr;    \
arr >= end && (((__object__) = (__type__)*arr) != NULL/* || true*/);                                      \
arr--)

#define USING_NS_U using namespace u87

#define ccbf( _src_, _dst_ ) blendFunc( _src_, _dst_ )



#if (COCOS2D_VERSION >= 0x00020000)
/** compensative code for cocos2d 2.0 changes */
#define setIsVisible setVisible
#endif




/**
 * FIX CODE for create func for old cocos2d-x api
 */
#define CREATE_FUNC(__TYPE__)\
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

/**
 * quick create func with parameters
 */
#define CREATE_FUNC1(__TYPE__, _para1type_, _para1_)\
static __TYPE__* create(_para1type_ _para1_) \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(_para1_)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#define CREATE_FUNC2(__TYPE__\
	, _para1type_, _para1_\
	, _para2type_, _para2_)\
static __TYPE__* create(_para1type_ _para1_\
	, _para2type_ _para2_)\
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(_para1_, _para2_)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#define CREATE_FUNC3(__TYPE__\
	, _para1type_, _para1_\
	, _para2type_, _para2_\
	, _para3type_, _para3_)\
static __TYPE__* create(_para1type_ _para1_\
	, _para2type_ _para2_\
	, _para3type_ _para3_)\
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(_para1_, _para2_, _para3_)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}
#define CREATE_FUNC4(__TYPE__\
	, _para1type_, _para1_\
	, _para2type_, _para2_\
	, _para3type_, _para3_\
	, _para4type_, _para4_)\
static __TYPE__* create(_para1type_ _para1_\
	, _para2type_ _para2_\
	, _para3type_ _para3_\
	, _para4type_ _para4_)\
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(_para1_, _para2_, _para3_, _para4_)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}


#define CREATE_FUNC5(__TYPE__\
	, _para1type_, _para1_\
	, _para2type_, _para2_\
	, _para3type_, _para3_\
	, _para4type_, _para4_\
	, _para5type_, _para5_)\
static __TYPE__* create(_para1type_ _para1_\
	, _para2type_ _para2_\
	, _para3type_ _para3_\
	, _para4type_ _para4_\
	, _para5type_ _para5_)\
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(_para1_, _para2_, _para3_, _para4_, _para5_)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}
 


#define COPYWITHZONE_FUNC(__class__, __super__, __init__)                          \
    virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone *pZone)     \
    {                                                                   \
	    cocos2d::CCZone* pNewZone = NULL;                               \
	    __class__* pCopy = NULL;                                        \
	    if(pZone && pZone->m_pCopyObject)                               \
	    {                                                               \
		    /*in case of being called at sub class*/                    \
		    pCopy = (__class__*)(pZone->m_pCopyObject);                 \
	    }                                                               \
	    else                                                            \
	    {                                                               \
		    pCopy = new __class__();                                    \
            pZone = pNewZone = new cocos2d::CCZone(pCopy);              \
        }                                                               \
                                                                        \
	    __super__::copyWithZone(pZone);                                 \
                                                                        \
        pCopy->__init__;                                                \
                                                                        \
	    CC_SAFE_DELETE(pNewZone);                                       \
	    return pCopy;                                                   \
    }
