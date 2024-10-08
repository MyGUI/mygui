/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef TREE_CONTROL_ITEM_H_
#define TREE_CONTROL_ITEM_H_

#include "TreeControl.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	class TreeControlItem : public Button
	{
		MYGUI_RTTI_DERIVED(TreeControlItem)

	public:
		TreeControl::Node* getNode() const;
		Button* getButtonExpandCollapse() const
		{
			return mpButtonExpandCollapse;
		}
		ImageBox* getIcon() const
		{
			return mpIcon;
		}

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget);
		void notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget);
		void notifyMouseWheel(Widget* pSender, int nValue);

	private:
		Button* mpButtonExpandCollapse{nullptr};
		ImageBox* mpIcon{nullptr};
		//int mnLevelOffset;
	};

}

#endif // TREE_CONTROL_ITEM_H_
