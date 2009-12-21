/*!
	@file
	@author     Pavel Turin
	@date       08/2009
	@module
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

    void TreeControlItem::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
    {
        Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

        initialiseWidgetSkin(_info);
    }

    TreeControlItem::~TreeControlItem()
    {
        shutdownWidgetSkin();
    }

    void TreeControlItem::baseChangeWidgetSkin(ResourceSkin* pSkinInformation)
    {
        shutdownWidgetSkin();
        Button::baseChangeWidgetSkin(pSkinInformation);
        initialiseWidgetSkin(pSkinInformation);
    }

    void TreeControlItem::initialiseWidgetSkin(ResourceSkin* pSkinInformation)
    {
        for (VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
            Widget* pWidget = *WidgetIterator;
            pWidget->setUserData(pWidget->getPosition().left);

			if (*(pWidget->_getInternalData<std::string>()) == "ButtonExpandCollapse")
            {
                MYGUI_DEBUG_ASSERT(!mpButtonExpandCollapse, "widget already assigned");
                mpButtonExpandCollapse = pWidget->castType<Button>();

                pWidget->eventMouseSetFocus = newDelegate(this, &TreeControlItem::notifyMouseSetFocus);
                pWidget->eventMouseLostFocus = newDelegate(this, &TreeControlItem::notifyMouseLostFocus);
                pWidget->eventMouseWheel = newDelegate(this, &TreeControlItem::notifyMouseWheel);
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

        const MapString& SkinProperties = pSkinInformation->getProperties();
        MapString::const_iterator PropertyIterator = SkinProperties.find("LevelOffset");
        if (PropertyIterator != SkinProperties.end())
            mnLevelOffset = utility::parseInt(PropertyIterator->second);
    }

    void TreeControlItem::shutdownWidgetSkin()
    {
        mpButtonExpandCollapse = nullptr;
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
