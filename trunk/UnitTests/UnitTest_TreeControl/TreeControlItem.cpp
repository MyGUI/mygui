/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/

#include "precompiled.h"
#include "TreeControlItem.h"

namespace MyGUI
{
    TreeControlItem::TreeControlItem() :
        mpButtonExpandCollapse(nullptr),
        mpIcon(nullptr),
        mnLevelOffset(16)
    {
    }

    void TreeControlItem::initialiseOverride()
    {
		Base::initialiseOverride();

        for (VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
            Widget* pWidget = *WidgetIterator;
            pWidget->setUserData(pWidget->getPosition().left);

			if (*(pWidget->_getInternalData<std::string>()) == "ButtonExpandCollapse")
            {
                MYGUI_DEBUG_ASSERT(!mpButtonExpandCollapse, "widget already assigned");
                mpButtonExpandCollapse = pWidget->castType<Button>();

                pWidget->eventMouseSetFocus += newDelegate(this, &TreeControlItem::notifyMouseSetFocus);
                pWidget->eventMouseLostFocus += newDelegate(this, &TreeControlItem::notifyMouseLostFocus);
                pWidget->eventMouseWheel += newDelegate(this, &TreeControlItem::notifyMouseWheel);
            }
            else
                pWidget->setInheritsPick(true);

            if (*(pWidget->_getInternalData<std::string>()) == "Image")
            {
                MYGUI_DEBUG_ASSERT(!mpIcon, "widget already assigned");
                mpIcon = pWidget->castType<StaticImage>();
            }
        }

        MYGUI_ASSERT(nullptr != mpButtonExpandCollapse, "Child ButtonExpandCollapse not found in skin (TreeControlItem must have ButtonExpandCollapse)");

		if (isUserString("LevelOffset"))
			mnLevelOffset = utility::parseValue<int>(getUserString("LevelOffset"));
    }

    void TreeControlItem::shutdownOverride()
    {
        mpButtonExpandCollapse = nullptr;

		Base::shutdownOverride();
    }

    void TreeControlItem::notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget)
    {
        if (pSender && pSender->getParent() == this)
            onMouseSetFocus(pPreviousWidget);
    }

    void TreeControlItem::notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget)
    {
        if (pSender && pSender->getParent() == this)
            onMouseLostFocus(pNextWidget);
    }

    void TreeControlItem::notifyMouseWheel(Widget* pSender, int nValue)
    {
        if (pSender && pSender->getParent() == this)
            onMouseWheel(nValue);
    }

    TreeControl::Node* TreeControlItem::getNode() const
    {
        return *(const_cast<TreeControlItem*>(this)->getUserData<TreeControl::Node*>());
    }

    void TreeControlItem::setLevel(size_t nLevel)
    {
        int nOffset = (mnLevelOffset * nLevel);
        getSubWidgetText()->setViewOffset(IntPoint(-nOffset, 0));

        for (VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
            Widget* pWidget = *WidgetIterator;
            pWidget->setPosition(IntPoint(
                *(pWidget->getUserData<int>()) + nOffset,
                pWidget->getPosition().top));
        }
    }
}
