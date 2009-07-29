
#pragma once

#include "TreeControl.h"

namespace MyGUI
{
    class TreeControlItem : public MyGUI::Button
    {
        MYGUI_RTTI_DERIVED(TreeControlItem);

    public:
        TreeControlItem();

        virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

        TreeControl::Node* getNode() const;
        MyGUI::ButtonPtr getButtonExpandCollapse() const { return mpButtonExpandCollapse; }
        MyGUI::StaticImagePtr getIcon() const  { return mpIcon; }

        void setLevel(size_t nLevel);

    protected:
        virtual ~TreeControlItem();

        virtual void baseChangeWidgetSkin(MyGUI::ResourceSkin* pSkinInformation);

        void notifyMouseSetFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pPreviousWidget);
        void notifyMouseLostFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pNextWidget);
        void notifyMouseWheel(MyGUI::WidgetPtr pSender, int nValue);

    private:
        void initialiseWidgetSkin(MyGUI::ResourceSkin* pSkinInformation);
        void shutdownWidgetSkin();

        MyGUI::ButtonPtr mpButtonExpandCollapse;
        MyGUI::StaticImagePtr mpIcon;
        int mnLevelOffset;
    };

}
