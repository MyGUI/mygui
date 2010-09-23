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
		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		void notifySelectWidgetType(MyGUI::Widget* _sender);
		//void notifyChangeCreatorMode(bool _modeCreate);

		void initialise();

	private:
		MyGUI::Tab* mTabSkins;

		int mWidgetsButtonWidth;
		int mWidgetsButtonHeight;
		int mWidgetsButtonsInOneLine;
		int mMaxLines;

		std::string mSkinSheetName;
		MyGUI::VectorWidgetPtr mWidgets;
		EditorToolTip* mToolTip;
	};

} // namespace tools

#endif // __WIDGETS_WINDOW_H__
