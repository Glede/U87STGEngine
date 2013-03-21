#pragma once

#include "U87Util.h"
#include "U87GameUnit.h" 
#include "U87Terrain.h"
#include "U87Camera.h"
#include <hash_map>

namespace u87
{
    typedef std::hash_map<int, cocos2d::CCArray*> ArrayMap;

    /** World Node */
    class World: public cocos2d::CCNode
    {
    protected:
        World();

    public:
        virtual ~World();

        bool init();
        CREATE_FUNC(World);

    public: 
        static World* sharedInstance() { return s_sharedInstance; }
    protected:
        static World* s_sharedInstance;

    public:
        /** schedule selector to follow cameranode */
        virtual void updateFollowCameraNode(float dt);

        /** get a index array for a specific type of target unit*/
        cocos2d::CCArray* getTargetIndex(UnitType type);

        
        /** get a single unit for a specific type*/
        template <typename Type>
        Type* getGameUnit(UnitID id)
        {
            return (Type*)m_gameUnits->objectForKey(id);
        }

        void addGameUnit(GameUnit* unit, int z = 0);

        void removeGameUnit(GameUnit* unit);
         
        CC_PROPERTY(Camera*, m_camera, WorldCamera);
        CC_SYNTHESIZE_PASS_BY_REF(cocos2d::CCRect, m_border, Border);
        CC_SYNTHESIZE(Terrain*, m_terran, Terran);

    protected: 
        const static int MAX_ID = 2222222;
        int m_availableID;
#if (COCOS2D_VERSION > 0x00020000)
        cocos2d::CCDictionary* m_gameUnits;
#else
        cocos2d::CCDictionary<int>* m_gameUnits;
#endif
        ArrayMap m_indexMap;
    };
}