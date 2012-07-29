/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _ae329786_acd2_4c09_ab3b_73baa2294a60_
#define _ae329786_acd2_4c09_ab3b_73baa2294a60_

#include "BaseLayout/BaseLayout.h"
#include "EditorToolTip.h"

namespace tools
{

	class WidgetsWindow :
		public wraps::BaseLayout
	{
	public:
		WidgetsWindow(MyGUI::Widget* _parent = nullptr);
		virtual ~WidgetsWindow();

	private:
		void requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item);
		void requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag);
		void requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info);

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		void notifySelectWidgetType(MyGUI::Widget* _sender);
		void notifyChangeCreatorMode(bool _modeCreate);
		void notifyMouseButtonClickPopupMode(MyGUI::Widget* _sender);

		void initialise();

		SkinInfo getCellData(MyGUI::Widget* _widget);
		bool getCellSelected(MyGUI::Widget* _widget);

	private:
		MyGUI::TabControl* mTabSkins;
		MyGUI::Button* mPopupMode;

		int mWidgetsButtonWidth;
		int mWidgetsButtonHeight;
		int mWidgetsButtonOffset;

		std::string mSkinSheetName;
		std::string mButtonSkinName;

		typedef std::vector<MyGUI::ItemBox*> VectorItemBox;
		VectorItemBox mItemBoxs;
	};

}

#endif
