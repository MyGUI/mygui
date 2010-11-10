/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __WIDGETS_WINDOW_H__
#define __WIDGETS_WINDOW_H__

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
		MyGUI::Tab* mTabSkins;
		MyGUI::Button* mPopupMode;

		int mWidgetsButtonWidth;
		int mWidgetsButtonHeight;

		std::string mSkinSheetName;
		EditorToolTip* mToolTip;

		typedef std::vector<MyGUI::ItemBox*> VectorItemBox;
		VectorItemBox mItemBoxs;
	};

} // namespace tools

#endif // __WIDGETS_WINDOW_H__
