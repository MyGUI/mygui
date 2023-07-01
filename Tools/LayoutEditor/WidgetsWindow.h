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

	class WidgetsWindow : public wraps::BaseLayout
	{
	public:
		WidgetsWindow(MyGUI::Widget* _parent = nullptr);
		~WidgetsWindow() override;

	private:
		void requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item);
		void requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag) const;
		void requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info);

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		void notifySelectWidgetType(MyGUI::Widget* _sender);
		void notifyChangeCreatorMode(bool _modeCreate);
		void notifyMouseButtonClickPopupMode(MyGUI::Widget* _sender);

		void initialise();

		SkinInfo getCellData(MyGUI::Widget* _widget);
		bool getCellSelected(MyGUI::Widget* _widget);

	private:
		MyGUI::TabControl* mTabSkins{nullptr};
		MyGUI::Button* mPopupMode{nullptr};

		int mWidgetsButtonWidth{0};
		int mWidgetsButtonHeight{0};
		int mWidgetsButtonOffset{0};

		std::string mSkinSheetName;
		std::string mButtonSkinName;

		using VectorItemBox = std::vector<MyGUI::ItemBox*>;
		VectorItemBox mItemBoxs;
	};

}

#endif
