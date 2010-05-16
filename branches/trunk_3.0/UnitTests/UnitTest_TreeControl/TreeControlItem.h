/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef __TREE_CONTROL_ITEM_H__
#define __TREE_CONTROL_ITEM_H__

#include "TreeControl.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

    class TreeControlItem : public Button
    {
        MYGUI_RTTI_DERIVED( TreeControlItem )

    public:
        TreeControlItem();

        virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, const std::string& _name);

        TreeControl::Node* getNode() const;
        Button* getButtonExpandCollapse() const { return mpButtonExpandCollapse; }
        StaticImage* getIcon() const  { return mpIcon; }

        void setLevel(size_t nLevel);

    protected:
        virtual ~TreeControlItem();

        virtual void baseChangeWidgetSkin(ResourceSkin* pSkinInformation);

        void notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget);
        void notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget);
        void notifyMouseWheel(Widget* pSender, int nValue);

    private:
        void initialiseWidgetSkin(ResourceSkin* pSkinInformation);
        void shutdownWidgetSkin();

        Button* mpButtonExpandCollapse;
        StaticImage* mpIcon;
        int mnLevelOffset;
    };

}

#endif // __TREE_CONTROL_ITEM_H__
