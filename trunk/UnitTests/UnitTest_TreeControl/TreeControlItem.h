/*!
	@file
	@author     Pavel Turin
	@date       08/2009
	@module
*/
#ifndef __TREE_CONTROL_ITEM_H__
#define __TREE_CONTROL_ITEM_H__

#include "TreeControl.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

    class TreeControlItem : public Button
    {
        MYGUI_RTTI_DERIVED( TreeControlItem );

    public:
        TreeControlItem();

        virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

        TreeControl::Node* getNode() const;
        ButtonPtr getButtonExpandCollapse() const { return mpButtonExpandCollapse; }
        StaticImagePtr getIcon() const  { return mpIcon; }

        void setLevel(size_t nLevel);

    protected:
        virtual ~TreeControlItem();

        virtual void baseChangeWidgetSkin(ResourceSkin* pSkinInformation);

        void notifyMouseSetFocus(WidgetPtr pSender, WidgetPtr pPreviousWidget);
        void notifyMouseLostFocus(WidgetPtr pSender, WidgetPtr pNextWidget);
        void notifyMouseWheel(WidgetPtr pSender, int nValue);

    private:
        void initialiseWidgetSkin(ResourceSkin* pSkinInformation);
        void shutdownWidgetSkin();

        ButtonPtr mpButtonExpandCollapse;
        StaticImagePtr mpIcon;
        int mnLevelOffset;
    };

}

#endif // __TREE_CONTROL_ITEM_H__
