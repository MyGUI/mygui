
#include "Precompiled.h"
#include "TreeControlItem.h"

namespace MyGUI
{
    TreeControlItem::TreeControlItem() :
        mpButtonExpandCollapse(nullptr),
        mpIcon(nullptr),
        mnLevelOffset(16)
    {
    }

    void TreeControlItem::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
    {
        Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

        initialiseWidgetSkin(_info);
    }

    TreeControlItem::~TreeControlItem()
    {
        shutdownWidgetSkin();
    }

    void TreeControlItem::baseChangeWidgetSkin(MyGUI::ResourceSkin* pSkinInformation)
    {
        shutdownWidgetSkin();
        MyGUI::Button::baseChangeWidgetSkin(pSkinInformation);
        initialiseWidgetSkin(pSkinInformation);
    }

    void TreeControlItem::initialiseWidgetSkin(MyGUI::ResourceSkin* pSkinInformation)
    {
        for (MyGUI::VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
            MyGUI::WidgetPtr pWidget = *WidgetIterator;
            pWidget->setUserData(pWidget->getPosition().left);

			if (*(pWidget->_getInternalData<std::string>()) == "ButtonExpandCollapse")
            {
                MYGUI_DEBUG_ASSERT(!mpButtonExpandCollapse, "widget already assigned");
                mpButtonExpandCollapse = pWidget->castType<MyGUI::Button>();

                pWidget->eventMouseSetFocus = newDelegate(this, &TreeControlItem::notifyMouseSetFocus);
                pWidget->eventMouseLostFocus = newDelegate(this, &TreeControlItem::notifyMouseLostFocus);
                pWidget->eventMouseWheel = newDelegate(this, &TreeControlItem::notifyMouseWheel);
            }
            else
                pWidget->setInheritsPick(true);

            if (*(pWidget->_getInternalData<std::string>()) == "Image")
            {
                MYGUI_DEBUG_ASSERT(!mpIcon, "widget already assigned");
                mpIcon = pWidget->castType<MyGUI::StaticImage>();
            }
        }

        MYGUI_ASSERT(NULL != mpButtonExpandCollapse, "Child ButtonExpandCollapse not found in skin (TreeControlItem must have ButtonExpandCollapse)");

        const MyGUI::MapString& SkinProperties = pSkinInformation->getProperties();
        MyGUI::MapString::const_iterator PropertyIterator = SkinProperties.find("LevelOffset");
        if (PropertyIterator != SkinProperties.end())
            mnLevelOffset = MyGUI::utility::parseInt(PropertyIterator->second);
    }

    void TreeControlItem::shutdownWidgetSkin()
    {
        mpButtonExpandCollapse = NULL;
    }

    void TreeControlItem::notifyMouseSetFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pPreviousWidget)
    {
        if (pSender && pSender->getParent() == this)
            onMouseSetFocus(pPreviousWidget);
    }

    void TreeControlItem::notifyMouseLostFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pNextWidget)
    {
        if (pSender && pSender->getParent() == this)
            onMouseLostFocus(pNextWidget);
    }

    void TreeControlItem::notifyMouseWheel(MyGUI::WidgetPtr pSender, int nValue)
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
        getSubWidgetText()->setViewOffset(MyGUI::IntPoint(-nOffset, 0));

        for (MyGUI::VectorWidgetPtr::iterator WidgetIterator = mWidgetChildSkin.begin(); WidgetIterator != mWidgetChildSkin.end(); ++WidgetIterator)
        {
            MyGUI::WidgetPtr pWidget = *WidgetIterator;
            pWidget->setPosition(MyGUI::IntPoint(
                *(pWidget->getUserData<int>()) + nOffset,
                pWidget->getPosition().top));
        }
    }
}
