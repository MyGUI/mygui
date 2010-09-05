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
		public wraps::BaseLayout,
		public MyGUI::Singleton<WidgetsWindow> //FIXME
	{
	public:
		WidgetsWindow(MyGUI::Widget* _parent = nullptr);
		virtual ~WidgetsWindow();

		void clearNewWidget();
		void startNewWidget(int _x1, int _y1, MyGUI::MouseButton _id);
		void createNewWidget(int _x2, int _y2);
		void finishNewWidget(int _x2, int _y2);

		int getCreateStatus();

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);
		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		void notifySelectWidgetType(MyGUI::Widget* _sender);
		void notifySelectWidgetTypeDoubleclick(MyGUI::Widget* _sender);

		void initialise();
		void clearAllSheets();

	private:
		MyGUI::Tab* mTabSkins;

		int mWidgetsButtonWidth;
		int mWidgetsButtonHeight;
		int mWidgetsButtonsInOneLine;
		std::string mSkinSheetName;

		MyGUI::Widget* mCurrentWidget;

		EditorToolTip* mToolTip;

		// info for new widget
		int mX1;
		int mY1;
		int mX2;
		int mY2;

		// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
		int mCreatingStatus;

		std::string mNewWidgetType;
		std::string mNewWidgetSkin;
		int mMaxLines;
	};

} // namespace tools

#endif // __WIDGETS_WINDOW_H__
