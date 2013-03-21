#pragma once

#include "U87Util.h"
#include "U87Math.h"

#include <list>

namespace u87
{
    class LayoutElement;
    class LayoutManager;
    class LayoutNode;
    class LayoutMenu;
    class LayoutScene;
    typedef std::list<LayoutElement*> LayoutList;


    /** a single instance class manage up all layout element */
    class LayoutManager
    {
    friend class LayoutElement;
    public:
        static LayoutManager* sharedInstance() { return s_sharedInstance; }
    private:
        static LayoutManager* s_sharedInstance;

    public:
        /** call this method when layout elements need to replace themselves to
        fit new windows size */
        void refresh();        

        LayoutList& getAllLayoutElement()   { return m_layoutList; };
        
    private:
        LayoutManager() {}
        ~LayoutManager() {}
        void addLayoutElement(LayoutElement* layout);
        void removeLayoutElement(LayoutElement* layout);

    private:
         LayoutList m_layoutList;
    };


    /** abstract class for nodes which are part of ui layout, auto reg to layout
    manager, can beinformed to refresh its update its position*/
    class LayoutElement
    {
    public:
        virtual void updatePosition();

        CC_PROPERTY_PASS_BY_REF(cocos2d::CCPoint, m_layoutAnchor, LayoutAnchor);
        CC_PROPERTY_PASS_BY_REF(cocos2d::CCPoint, m_layoutOffset, LayoutOffset);

    protected:
        LayoutElement();
        virtual ~LayoutElement();
    };

    /** a common layout node extends layout node, can auto update position */
    class LayoutNode: public cocos2d::CCNode, public LayoutElement
    { 
    public: 
        /** update position when node enter */
        virtual void onEnter();

        static LayoutNode* create();
    };

    /** a layout menu item, can auto update position */
    class LayoutMenu: public cocos2d::CCMenu, public LayoutElement
    { 
    public:
        /** update position when node enter */
        virtual void onEnter();

        static LayoutMenu* create(cocos2d::CCMenuItem* item, ...);
    };

    /** a layout scene  */
    class LayoutScene: public cocos2d::CCScene, public LayoutElement
    {
    public:
        /** call Layout Manager refresh when scene enter */
        virtual void onEnter();

        static LayoutScene* create();
    };
}
