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
#include "U87GameUnit.h"
#include <cmath>
#include <iostream>


namespace u87
{ 
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
    
    typedef unsigned int CTag;
    const CTag _CT_NONE = (unsigned int)-1;
#define CT_NONE _CT_NONE

    /** Body interface for collision test*/
    class BodyNode: public cocos2d::CCNode
    {
    public:
        BodyNode() { m_type = BODY_NONE; m_enabled = false; m_vCCRadius = 0; m_unit = NULL; }
        virtual ~BodyNode(void) {}

        bool init(BodyType type);
        virtual void update(float dt);

        // collision tags for rules to make a collision test
    public:
        /** set sel tag. end with CT_NONE. */
        virtual void setCTag(CTag tag, ...);
        virtual void addCTag(CTag tag);
        virtual void removeCTag(CTag tag);

        /** set target tag. end with CT_NONE. */
        virtual void setTargetCTag(CTag tag, ...);
        virtual void addTargetCTag(CTag tag);
        virtual void removeTargetCTag(CTag tag);


    public:  
        template <typename T>
        T* getUnit() { return (T*)m_pParent; }
        CC_SYNTHESIZE(GameUnit*, m_unit, Unit);

    public:
        // for interaction with another body
        virtual void addCollisionBody(BodyNode* body);

        inline BodyType getType() const { return m_type; }
        inline bool isEnabled() const { return m_enabled; }
        virtual float getCircumcircleRadius() const { return m_vCCRadius; }
        inline const Point& getCenter() const { return m_vCenter; }
        
    public:
        inline static void setDrawBody(bool val) { s_visible = val; }
    protected:
        static bool s_visible;


    protected:
        BodyType m_type;
        bool m_enabled;

        float m_vCCRadius;
        Point m_vCenter;

        std::vector<BodyNode*> m_hitRecord;
        std::vector<CTag> m_CTag;
        std::vector<CTag> m_targetCTag;
    };


    /** Round body */
	class RoundBodyNode : public BodyNode
	{    
    public:
        bool init(float radius);
        CREATE_FUNC1(RoundBodyNode, float, radius); 
        virtual void update(float dt);
        virtual void draw();


        inline float getRadius() const { return m_vRadius; }

    protected:
        float m_radius;

        // var
        float m_vRadius;
	};



    /* Strip body */
    class StripBodyNode: public BodyNode
    {
    public: 
        bool init(float width);
        CREATE_FUNC1(StripBodyNode, float, width);
        virtual void update(float dt);
        virtual void draw();


        inline const Point& getBegin() const { return m_begin; }
        inline const Point& getEnd() const { return m_end; }
        inline float getWidth() const { return m_vWidth; }

        virtual void setPosition(const Point& position);


        void getTops(Point* topA, Point* topB, Point* topC, Point* topD);

    protected:
        float m_width;

        // var
        Point m_last;
        Point m_begin;
        Point m_end;
        float m_vWidth;
    };

    /** Rectanle body, angle in degrees */
    class RectBodyNode: public BodyNode
    {
    public: 
        bool init(float width, float height);
        CREATE_FUNC2(RectBodyNode, float, width, float, height);
        virtual void update(float dt); 
        virtual void draw();


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



}

