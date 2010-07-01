/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/

#include "precompiled.h"
#include "TreeControl.h"
#include "TreeControlItem.h"

namespace MyGUI
{
    TreeControl::Node::Node(TreeControl* pOwner) :
        GenericNode<Node, TreeControl>(pOwner),
        mbIsPrepared(false),
        mbIsExpanded(true),
        mstrImage("Folder")
    {
    }

    TreeControl::Node::Node(const UString& strText, Node* pParent) :
        GenericNode<Node, TreeControl>(strText, pParent),
        mbIsPrepared(false),
        mbIsExpanded(false),
        mstrImage("Folder")
    {
    }

    TreeControl::Node::Node(const UString& strText, const UString& strImage, Node* pParent) :
        GenericNode<Node, TreeControl>(strText, pParent),
        mbIsPrepared(false),
        mbIsExpanded(false),
        mstrImage(strImage)
    {
    }

    TreeControl::Node::~Node()
    {
    }

    void TreeControl::Node::prepare()
    {
        if (mbIsPrepared || !mpOwner)
            return;

        mpOwner->eventTreeNodePrepare(mpOwner, this);
        mbIsPrepared = true;
    }

    size_t TreeControl::Node::prepareChildren()
    {
        prepare();

        size_t nResult = 0;
        for (VectorNodePtr::iterator Iterator = getChildren().begin(); Iterator != getChildren().end(); ++Iterator)
        {
            TreeControl::Node* pChild = *Iterator;

            nResult++;

            pChild->prepare();
            if (pChild->isExpanded())
                nResult += pChild->prepareChildren();
        }

        return nResult;
    }

    void TreeControl::Node::setExpanded(bool bIsExpanded)
    {
        if (mbIsExpanded == bIsExpanded)
            return;

        mbIsExpanded = bIsExpanded;

        invalidate();
    }

    void TreeControl::Node::setImage(const UString& strImage)
    {
        mstrImage = strImage;

        invalidate();
    }

    TreeControl::TreeControl() :
        mpWidgetScroll(nullptr),
        mbScrollAlwaysVisible(true),
        mbInvalidated(false),
        mbRootVisible(false),
        mnItemHeight(1),
        mnScrollRange(-1),
        mnTopIndex(0),
        mnTopOffset(0),
        mnFocusIndex(ITEM_NONE),
        mpSelection(nullptr),
		mpRoot(nullptr),
        mnExpandedNodes(0)
    {
    }

    void TreeControl::initialiseWidgetSkin(ResourceSkin* _info)
    {
		Base::initialiseWidgetSkin(_info);

		// FIXME перенесенно из конструктора, проверить смену скина
        mpRoot = new Node(this);

		//FIXME
		setNeedKeyFocus(true);

        for (VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
			if (*(*WidgetIterator)->_getInternalData<std::string>() == "VScroll")
            {
                MYGUI_DEBUG_ASSERT(!mpWidgetScroll, "widget already assigned");
                mpWidgetScroll = (*WidgetIterator)->castType<VScroll>();
                mpWidgetScroll->eventScrollChangePosition = newDelegate(this, &TreeControl::notifyScrollChangePosition);
                mpWidgetScroll->eventMouseButtonPressed = newDelegate(this, &TreeControl::notifyMousePressed);
            }
            else
            if (*(*WidgetIterator)->_getInternalData<std::string>() == "Client")
            {
                MYGUI_DEBUG_ASSERT(!mWidgetClient, "widget already assigned");
                mWidgetClient = (*WidgetIterator);
                mWidgetClient->eventMouseButtonPressed = newDelegate(this, &TreeControl::notifyMousePressed);
            }
        }

        MYGUI_ASSERT(nullptr != mpWidgetScroll, "Child VScroll not found in skin (TreeControl must have VScroll)");
        MYGUI_ASSERT(nullptr != mWidgetClient, "Child Widget Client not found in skin (TreeControl must have Client)");

        const MapString& SkinProperties = _info->getProperties();
        MapString::const_iterator PropertyIterator = SkinProperties.find("SkinLine");
        if (PropertyIterator != SkinProperties.end())
            mstrSkinLine = PropertyIterator->second;
        MYGUI_ASSERT(!mstrSkinLine.empty(), "SkinLine property not found (TreeControl must have SkinLine property)");

        PropertyIterator = SkinProperties.find("HeightLine");
        if (PropertyIterator != SkinProperties.end())
            mnItemHeight = utility::parseInt(PropertyIterator->second);

        if (mnItemHeight < 1)
            mnItemHeight = 1;

        mpWidgetScroll->setScrollPage((size_t)mnItemHeight);
        mpWidgetScroll->setScrollViewPage((size_t)mnItemHeight);

        invalidate();
    }

    void TreeControl::shutdownWidgetSkin()
    {
        mpWidgetScroll = nullptr;
        mWidgetClient = nullptr;
		// FIXME перенесенно из деструктора, проверить смену скина
        delete mpRoot;

		Base::shutdownWidgetSkin();
    }

    void TreeControl::setRootVisible(bool bValue)
    {
        if (mbRootVisible == bValue)
            return;

        mbRootVisible = bValue;
        invalidate();
    }

    void TreeControl::setSelection(Node* pSelection)
    {
        if (mpSelection == pSelection)
            return;

        mpSelection = pSelection;
        while (pSelection)
        {
            pSelection->setExpanded(true);
            pSelection = pSelection->getParent();
        }

        invalidate();
        eventTreeNodeSelected(this, mpSelection);
    }

    void TreeControl::onMouseWheel(int nValue)
    {
        notifyMouseWheel(nullptr, nValue);

        Widget::onMouseWheel(nValue);
    }

    void TreeControl::onKeyButtonPressed(KeyCode Key, Char Character)
    {
        // TODO

        Widget::onKeyButtonPressed(Key, Character);
    }

    void TreeControl::setSize(const IntSize& Size)
    {
        Widget::setSize(Size);

        invalidate();
    }

    void TreeControl::setCoord(const IntCoord& Bounds)
    {
        Widget::setCoord(Bounds);

        invalidate();
    }

    void TreeControl::notifyFrameEntered(float nTime)
    {
        if (!mbInvalidated)
            return;

        mnExpandedNodes = mpRoot->prepareChildren();
        if (mbRootVisible)
            mnExpandedNodes++;

        updateScroll();
        updateItems();

        validate();

        mbInvalidated = false;
        Gui::getInstance().eventFrameStart -= newDelegate(this, &TreeControl::notifyFrameEntered);
    }

    void TreeControl::updateScroll()
    {
        mnScrollRange = (mnItemHeight * (int)mnExpandedNodes) - mWidgetClient->getHeight();

        if (!mbScrollAlwaysVisible || mnScrollRange <= 0 || mpWidgetScroll->getLeft() <= mWidgetClient->getLeft())
        {
            if (mpWidgetScroll->getVisible())
            {
                mpWidgetScroll->setVisible(false);
                mWidgetClient->setSize(mWidgetClient->getWidth() + mpWidgetScroll->getWidth(), mWidgetClient->getHeight());
            }
        }
        else
        if (!mpWidgetScroll->getVisible())
        {
            mWidgetClient->setSize(mWidgetClient->getWidth() - mpWidgetScroll->getWidth(), mWidgetClient->getHeight());
            mpWidgetScroll->setVisible(true);
        }

        mpWidgetScroll->setScrollRange(mnScrollRange + 1);

        if (mnExpandedNodes)
            mpWidgetScroll->setTrackSize(mpWidgetScroll->getLineSize() * mWidgetClient->getHeight() / mnItemHeight / mnExpandedNodes);
    }

    void TreeControl::updateItems()
    {
        int nPosition = mnTopIndex * mnItemHeight + mnTopOffset;

        int nHeight = (int)mItemWidgets.size() * mnItemHeight - mnTopOffset;
        while ((nHeight <= (mWidgetClient->getHeight() + mnItemHeight)) && mItemWidgets.size() < mnExpandedNodes)
        {
            TreeControlItem* pItem = mWidgetClient->createWidget<TreeControlItem>(
                mstrSkinLine,
                0,
                nHeight,
                mWidgetClient->getWidth(),
                mnItemHeight,
                Align::Top | Align::HStretch);

            pItem->eventMouseButtonPressed = newDelegate(this, &TreeControl::notifyMousePressed);
            pItem->eventMouseButtonDoubleClick = newDelegate(this, &TreeControl::notifyMouseDoubleClick);
            pItem->eventMouseWheel = newDelegate(this, &TreeControl::notifyMouseWheel);
            pItem->eventMouseSetFocus = newDelegate(this, &TreeControl::notifyMouseSetFocus);
            pItem->eventMouseLostFocus = newDelegate(this, &TreeControl::notifyMouseLostFocus);
            pItem->_setInternalData((size_t)mItemWidgets.size());
            pItem->getButtonExpandCollapse()->eventMouseButtonClick = newDelegate(this, &TreeControl::notifyExpandCollapse);

            mItemWidgets.push_back(pItem);

            nHeight += mnItemHeight;
        };

        if (nPosition >= mnScrollRange)
        {
            if (mnScrollRange <= 0)
            {
                if (nPosition || mnTopOffset || mnTopIndex)
                {
                    nPosition = 0;
                    mnTopIndex = 0;
                    mnTopOffset = 0;
                }
            }
            else
            {
                int nCount = mWidgetClient->getHeight() / mnItemHeight;
                mnTopOffset = mnItemHeight - (mWidgetClient->getHeight() % mnItemHeight);

                if (mnTopOffset == mnItemHeight)
                {
                    mnTopOffset = 0;
                    nCount--;
                }

                mnTopIndex = ((int)mnExpandedNodes) - nCount - 1;
                nPosition = mnTopIndex * mnItemHeight + mnTopOffset;
            }
        }

        mpWidgetScroll->setScrollPosition(nPosition);
    }

    void TreeControl::validate()
    {
        typedef std::pair<VectorNodePtr::iterator, VectorNodePtr::iterator> PairNodeEnumeration;
        typedef std::list<PairNodeEnumeration> ListNodeEnumeration;
        ListNodeEnumeration EnumerationStack;
        PairNodeEnumeration Enumeration;
        VectorNodePtr vectorNodePtr;
        if (mbRootVisible)
        {
            vectorNodePtr.push_back(mpRoot);
            Enumeration = PairNodeEnumeration(vectorNodePtr.begin(), vectorNodePtr.end());
        }
        else
            Enumeration = PairNodeEnumeration(mpRoot->getChildren().begin(), mpRoot->getChildren().end());

        size_t nLevel = 0;
        size_t nIndex = 0;
        size_t nItem = 0;
        int nOffset = 0 - mnTopOffset;

        while (true)
        {
            if (Enumeration.first == Enumeration.second)
            {
                if (EnumerationStack.empty())
                    break;

                Enumeration = EnumerationStack.back();
                EnumerationStack.pop_back();
                nLevel--;
                continue;
            }

            Node* pNode = *Enumeration.first;
            Enumeration.first++;
            
            if (nIndex >= (size_t)mnTopIndex)
            {
                if (nIndex >= mnExpandedNodes || mItemWidgets[nItem]->getTop() > mWidgetClient->getHeight())
                    break;

                TreeControlItem* pItem = mItemWidgets[nItem];
                pItem->setVisible(true);
                pItem->setCaption(pNode->getText());
                pItem->setLevel(nLevel);
                pItem->setPosition(IntPoint(0, nOffset));
                pItem->setButtonPressed(pNode == mpSelection);
                pItem->setUserData(pNode);

                Button* pButtonExpandCollapse = pItem->getButtonExpandCollapse();
                pButtonExpandCollapse->setVisible(pNode->hasChildren());
                pButtonExpandCollapse->setButtonPressed(!pNode->isExpanded());

                StaticImage* pIcon = pItem->getIcon();
                if (pIcon)
                {
                    ResourceImageSetPtr pIconResource = pIcon->getItemResource();
                    if (pIconResource)
                    {
                        UString strIconType(pNode->isExpanded() ? "Expanded" : "Collapsed");
                        ImageIndexInfo IconInfo = pIconResource->getIndexInfo(pNode->getImage(), strIconType);
                        if (IconInfo.size.empty())
                            pIcon->setItemResourceInfo(pIconResource->getIndexInfo(pNode->getImage(), "Common"));
                        else
                            pIcon->setItemResourceInfo(IconInfo);
                    }
                }

                nOffset += mnItemHeight;
                nItem++;
            }

            nIndex++;

            if (pNode->hasChildren() && pNode->isExpanded())
            {
                EnumerationStack.push_back(Enumeration);
                Enumeration.first = pNode->getChildren().begin();
                Enumeration.second = pNode->getChildren().end();
                nLevel++;
            }
        }

        if (nItem < mItemWidgets.size())
        {
            for (; nItem < mItemWidgets.size(); ++nItem)
            {
                mItemWidgets[nItem]->setButtonPressed(false);
                mItemWidgets[nItem]->setVisible(false);
            }
        }
    }

    void TreeControl::invalidate()
    {
        if (mbInvalidated)
            return;

        Gui::getInstance().eventFrameStart += newDelegate(this, &TreeControl::notifyFrameEntered);
        mbInvalidated = true;
    }

    void TreeControl::scrollTo(size_t nPosition)
    {
        mnTopOffset = ((int)nPosition % mnItemHeight);
        mnTopIndex = ((int)nPosition / mnItemHeight);

        invalidate();
    }

    void TreeControl::sendScrollingEvents(size_t nPosition)
    {
        eventTreeScrolled(this, nPosition);
        if (mnFocusIndex != ITEM_NONE)
            eventTreeNodeMouseSetFocus(this, mItemWidgets[mnFocusIndex]->getNode());
    }

    void TreeControl::notifyMousePressed(Widget* pSender, int nLeft, int nTop, MouseButton nID)
    {
        if ((nID == MouseButton::Left || nID == MouseButton::Right) && pSender != mpWidgetScroll)
        {
            Node* pSelection = mpSelection;
            if (pSender == mWidgetClient)
                pSelection = nullptr;
            else
            if (pSender->getVisible())
                pSelection = *pSender->getUserData<Node*>();

            setSelection(pSelection);

            if (nID == MouseButton::Right)
                eventTreeNodeContextMenu(this, mpSelection);
        }
    }

    void TreeControl::notifyMouseWheel(Widget* pSender, int nValue)
    {
        if (mnScrollRange <= 0)
            return;

        int nPosition = (int)mpWidgetScroll->getScrollPosition();
        if (nValue < 0)
            nPosition += mnItemHeight;
        else
            nPosition -= mnItemHeight;

        if (nPosition >= mnScrollRange)
            nPosition = mnScrollRange;
        else
        if (nPosition < 0)
            nPosition = 0;

        if ((int)mpWidgetScroll->getScrollPosition() == nPosition)
            return;

        mpWidgetScroll->setScrollPosition(nPosition);

        scrollTo(nPosition);
        sendScrollingEvents(nPosition);
    }

    void TreeControl::notifyMouseDoubleClick(Widget* pSender)
    {
        if (mpSelection)
            eventTreeNodeActivated(this, mpSelection);
    }

    void TreeControl::notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget)
    {
        mnFocusIndex = *pSender->_getInternalData<size_t>();
        eventTreeNodeMouseSetFocus(this, mItemWidgets[mnFocusIndex]->getNode());
    }

    void TreeControl::notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget)
    {
        if (!pNextWidget || (pNextWidget->getParent() != mWidgetClient))
        {
            mnFocusIndex = ITEM_NONE;
            eventTreeNodeMouseLostFocus(this, nullptr);
        }
    }

    void TreeControl::notifyScrollChangePosition(VScroll* pSender, size_t nPosition)
    {
        scrollTo(nPosition);
        sendScrollingEvents(nPosition);
    }

    void TreeControl::notifyExpandCollapse(Widget* pSender)
    {
        TreeControlItem* pItem = pSender->getParent()->castType<TreeControlItem>(false);
        if (!pItem)
            return;

        Node* pNode = pItem->getNode();
        pNode->setExpanded(!pNode->isExpanded());

        if (!pNode->isExpanded() && mpSelection && mpSelection->hasAncestor(pNode))
        {
            mpSelection = pNode;
            eventTreeNodeSelected(this, mpSelection);
        }

        invalidate();
    }
}