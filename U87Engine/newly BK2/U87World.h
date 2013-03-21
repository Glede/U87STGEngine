#pragma once

#include "U87Util.h"
#include "U87GameUnit.h" 
#include "U87Geometry.h"
#include "U87Terrain.h"
#include <hash_map>

namespace u87
{
    class CollisionHandler
    {
    public:
        virtual bool onCollision(BodyNode* A, BodyNode* B) = 0;
    };

    /** a collision rule */
    struct CollisionRule 
    {
        CollisionRule(CTag _tagA, CTag _tagB, CollisionHandler* _handler, int _priority = 0)
        {
            tagA = _tagA;
            tagB = _tagB;
            handler = _handler;
            priority = _priority;
        }

        CTag tagA; 
        CTag tagB; 
        int priority;
        CollisionHandler* handler;
    };

    inline bool operator== (const CollisionRule& a, const CollisionRule& b)
    {
        return a.tagA == b.tagA && a.tagB == b.tagB && a.priority == b.priority && a.handler == b.handler;
    }



    typedef std::hash_map<int, cocos2d::CCArray*> ArrayMap;
    typedef std::list<CollisionRule> RuleList;


    /** World Node */
    class World: public cocos2d::CCNode
    {
    protected:
        World();

    public:
        virtual ~World();

        bool init();
        CREATE_FUNC(World);
        virtual void update(float dt);

    public: 
        static World* sharedInstance() { return s_sharedInstance; }
    protected:
        static World* s_sharedInstance;

    public:
        /** schedule selector to follow cameranode */
        virtual void updateFollowCameraNode(float dt);

        template <typename T>
        T* getUnit(UnitID id) { return (T*)m_gameUnits->objectForKey(id); }

        /** get a index array for a specific ctag of body*/
        cocos2d::CCArray* getBodyIndex(CTag tag);
        
        void addGameUnit(GameUnit* unit, UnitID id = UID_NONE, int z = 0);

        void removeGameUnit(GameUnit* unit);
         
        void addCollisionRule(const CollisionRule& rule);

        void removeCollisionRule(const CollisionRule& rule);

        CC_PROPERTY(ICamera*, m_camera, WorldCamera);
        CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCRect, m_border, Border);
        CC_SYNTHESIZE(Terrain*, m_terran, Terran);

    protected: 
        UnitID m_nextAvailableID;
        cocos2d::CCDictionary* m_gameUnits;
        ArrayMap m_indexMap;
        RuleList m_ruleList;
    };
}