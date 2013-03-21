#include "U87Layout.h"

USING_NS_CC;
USING_NS_U;


///////////////////////////////////////////////////////////////////////////////
// Layout Manager
///////////////////////////////////////////////////////////////////////////////
LayoutManager* LayoutManager::s_sharedInstance = new LayoutManager();

void LayoutManager::refresh()
{
    for(LayoutList::iterator i = m_layoutList.begin(); i != m_layoutList.end(); i++)
    {
        (*i)->updatePosition();
    }
}

void LayoutManager::addLayoutElement(LayoutElement* layout)
{
    m_layoutList.push_back(layout);
}

void LayoutManager::removeLayoutElement(LayoutElement* layout)
{
    m_layoutList.remove(layout);
}

///////////////////////////////////////////////////////////////////////////////
// Layout element
///////////////////////////////////////////////////////////////////////////////

// update self position, and update child position
void LayoutElement::updatePosition()
{
    CCNode* node = dynamic_cast<CCNode*>(this);
    if(node)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint anchor = ccp(winSize.width* m_layoutAnchor.x, winSize.height * m_layoutAnchor.y);
        node->setPosition(anchor + m_layoutOffset);
    }
}



const CCPoint& LayoutElement::getLayoutAnchor()
{
    return m_layoutAnchor;
}



void LayoutElement::setLayoutAnchor(const CCPoint& value)
{
    m_layoutAnchor = value;
}



const CCPoint& LayoutElement::getLayoutOffset()
{
    return m_layoutOffset;
}



void LayoutElement::setLayoutOffset(const CCPoint& value)
{
    m_layoutOffset = value;   
}



LayoutElement::LayoutElement()
{
    LayoutManager::sharedInstance()->addLayoutElement(this);
}



LayoutElement::~LayoutElement()
{
    LayoutManager::sharedInstance()->removeLayoutElement(this);
}

///////////////////////////////////////////////////////////////////////////////
// Layout Node
///////////////////////////////////////////////////////////////////////////////

void LayoutNode::onEnter()
{
    updatePosition();
    CCNode::onEnter();
}

LayoutNode* LayoutNode::create()
{
    LayoutNode* pRet = new LayoutNode();
    pRet->autorelease();
    return pRet;
}

///////////////////////////////////////////////////////////////////////////////
// Layout Menu
///////////////////////////////////////////////////////////////////////////////

void LayoutMenu::onEnter()
{
    updatePosition();
    CCMenu::onEnter();
}

LayoutMenu* LayoutMenu::create(CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
    LayoutMenu *pRet = new LayoutMenu();
	if (pRet && pRet->initWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet);
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// Layout Scene
///////////////////////////////////////////////////////////////////////////////

void LayoutScene::onEnter()
{
    LayoutManager::sharedInstance()->refresh();

    CCScene::onEnter();
}

LayoutScene* LayoutScene::create()
{
    LayoutScene* pRet = new LayoutScene();
    if(pRet->init())
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
