//
//  Collision.h
//  ADGameScene
//
//  Created by Glede on Oct 1st 2012
//  Supplemented by CaiyZ(Mr.IroN)
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
 
#include "U87Math.h"
#include <cmath>
#include <iostream>


namespace u87
{
    // forward declaration of game unit
    class GameUnit;


    // collision tag, indicating the logical meaning of a body node
    typedef unsigned int CTag;
    const CTag _CLT_NONE = (unsigned int)-1;
#define CLT_NONE _CLT_NONE

    typedef std::vector<CTag> CTagList;


    class HitTestResult;
    class BodyNode;

    class RoundBodyNode;
    class StripBodyNode;
    class RectBodyNode;

    class CompositeBodyNode;


    ///////////////////////////////////////////////////////////////////////////
    // shapes geometries 
    ///////////////////////////////////////////////////////////////////////////

    enum BodyType
    {
        BODY_NONE,
        BODY_STRIP,
        BODY_ROUND,
        BODY_RECT,
        BODY_COMPOSITE
    };
     

    /** BodyNode super class for collision test unit
    every game unit can have one or more bodyNodes, whether as child node or not
    a body node should have a game unit as a responder to the collision.

    CTag, used to specify the logical meaning of the body, and a CTag of a body 
    is the index code, for other body to get for collision test
    targetCTag, used to specify the target bodies that will be the collision 
    target of this body.
    */
    class BodyNode: public cocos2d::CCNode
    {
    public:
        BodyNode() { m_type = BODY_NONE; m_vRadius = 0; m_active = false; m_gameUnit = NULL; }
        virtual ~BodyNode(void) {}

        bool init(BodyType type);
        virtual void update(float dt);
        virtual void updateState(float dt) { CC_UNUSED_PARAM(dt); }
        virtual void updateCollision(float dt);
         
    public: 
        virtual HitTestResult* hitTest(BodyNode* body);

        inline BodyType getType() const { return m_type; }
        virtual float getRadius() const { return m_vRadius; }
        inline const Point& getCenter() const { return m_vCenter; }
         
        inline const bool isActive() const { return m_active; }
        inline void setActive(bool val) { m_active = val; }

        // collision conduct related methods
    public:
        /** set sel tag. end with CLT_NONE. */
        virtual void setCTag(CTag tag);
        inline CTag getCTag() const { return m_CTag; }

        /** set target tag. end with CLT_NONE.
         this method works with the default getTargetCache() method.*/
        inline const CTagList& getTargetCTag() const { return m_targetCTag; }
        virtual void setTargetCTag(CTag tag, ...);
        virtual void addTargetCTag(CTag tag);
        virtual void removeTargetCTag(CTag tag);

        /** set a repsonder game unit, auto set active to true */
        inline void setGameUnit(GameUnit* unit, bool notactive = false) { m_gameUnit = unit; m_active = !notactive; }
        inline GameUnit* getGameUnit() const { return m_gameUnit; }

    protected:
        /** will be called by updateCollisionTest() method.
         * get collision targets, two ways to get it work.
         1.initialize @targetType via setTargetType() and use default getTargets() method.
         2.override getTargetCache() method yourself */
        virtual cocos2d::CCArray* getTargetCache();        


    public:
        inline static void setDrawBody(bool val) { s_visible = val; }
    protected:
        static bool s_visible;

    protected:
        // body geometry porperties
        BodyType m_type;
        float m_vRadius;
        Point m_vCenter;

        // control porperties
        bool m_active;
        GameUnit* m_gameUnit;
        CTag m_CTag;
        CTagList m_targetCTag;
    };

    BodyNode* const BODYNONE = new BodyNode();

    /** Round body */
	class RoundBodyNode : public BodyNode
	{    
    public:
        bool init(float radius);
        CREATE_FUNC1(RoundBodyNode, float, radius); 
        virtual void updateState(float dt);
        virtual void draw();
        
        virtual HitTestResult* hitTest(BodyNode* body);
         
    protected:
        float m_radius;
         
	};



    /* Strip body */
    class StripBodyNode: public BodyNode
    {
    public: 
        bool init(float width);
        CREATE_FUNC1(StripBodyNode, float, width);
        virtual void updateState(float dt);
        virtual void draw();
        
        virtual HitTestResult* hitTest(BodyNode* body);

        inline const Point& getBegin() const { return m_vBegin; }
        inline const Point& getEnd() const { return m_vEnd; }
        inline float getWidth() const { return m_vWidth; }

        virtual void setPosition(const Point& position);


        void getTops(Point* topA, Point* topB, Point* topC, Point* topD);

    protected:
        float m_width; 

        bool m_updateBegin;

        // var
        Point m_vBegin;
        Point m_vEnd;
        float m_vWidth;
    };

    /** Rectanle body, angle in degrees */
    class RectBodyNode: public BodyNode
    {
    public: 
        bool init(float width, float height);
        CREATE_FUNC2(RectBodyNode, float, width, float, height);
        virtual void updateState(float dt); 
        virtual void draw();
        
        virtual HitTestResult* hitTest(BodyNode* body);

        inline float getWidth() const { return m_vWidth; }
        inline float getHeight() const { return m_vHeight; }
        inline float getAngle() const { return m_vAngle; } 

        void getTops(Point* topA, Point* topB, Point* topC, Point* topD);

    protected: 
        float m_width;
        float m_height; 

        // var
        float m_vWidth;
        float m_vHeight;  
        float m_vAngle;
    };
 
    class CompositeBodyNode: public BodyNode
    { 

    };

    ///////////////////////////////////////////////////////////////////////////
    // hit test result;
    ///////////////////////////////////////////////////////////////////////////


    /** Type of Hit Test Result 
     * HTRT_NONE    no intersection
     * HTRT_CONTACT intersect on border
     * HTRT_CROSS   intersect cross
     */
    typedef unsigned int HitTestResultType;
    const HitTestResultType HTRT_NONE     = 0;
    const HitTestResultType HTRT_CONTACT  = 1;
    const HitTestResultType HTRT_CROSS    = 3;

    /** Result of a Hit Test , pass back intersect points*/
    class HitTestResult: public cocos2d::CCObject
    {
    public:
        HitTestResult(HitTestResultType resultType, const Point& point = POINT_ZERO, const float dist = 0.0)
            :resultType(resultType)
            ,crossPoint(point)
			,distance(dist)
        {  }

        bool isHit(void)    { return resultType != HTRT_NONE;    }
        bool isCross(void)  { return resultType == HTRT_CROSS;   }

        inline static HitTestResult* create(HitTestResultType type, const Point& point = cocos2d::CCPointZero, const float dist = 0.0)
        {
            HitTestResult* pRet = new HitTestResult(type, point, dist);
            pRet->autorelease();
            return pRet;
        }

    public:
        const HitTestResultType resultType;
        const Point crossPoint;
		const float distance;
    }; 

    HitTestResult* hitTestRoundRound(RoundBodyNode* a, RoundBodyNode* b);
    HitTestResult* hitTestRoundRect(RoundBodyNode* a, RectBodyNode* b);
    HitTestResult* hitTestRoundStrip(RoundBodyNode* a, StripBodyNode* b);

    HitTestResult* hitTestRectRound(RectBodyNode* a, RoundBodyNode* b);
    HitTestResult* hitTestRectRect(RectBodyNode* a, RectBodyNode* b);
    HitTestResult* hitTestRectStrip(RectBodyNode* a, StripBodyNode* b);

    HitTestResult* hitTestStripRound(StripBodyNode* a, RoundBodyNode* b);
    HitTestResult* hitTestStripRect(StripBodyNode* a, RectBodyNode* b);
    HitTestResult* hitTestStripStrip(StripBodyNode* a, StripBodyNode* b);


    
    ///////////////////////////////////////////////////////////////////////////
    // hit record;
    ///////////////////////////////////////////////////////////////////////////

    // hitRecord records a hit with  Target, hitPoint, distance
    struct HitRecord
    {
        HitRecord(BodyNode* target, const Point& hitPoint, float distance)
            :target(target)
            ,hitPoint(hitPoint)
            ,distance(distance)
        {    }

        const HitRecord& operator = (const HitRecord& value)
        {
            target   = value.target;
            hitPoint = value.hitPoint;
            distance = value.distance;
            return value;
        }

        BodyNode* self;
        BodyNode* target;
        Point hitPoint;
        float distance;
    };

    inline bool operator < (const HitRecord& a, const HitRecord& b)
    {
        return a.distance < b.distance;
    }
     
}

