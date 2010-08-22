/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __WIDGETS_WINDOW_H__
#define __WIDGETS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

class WidgetsWindow :
	public wraps::BaseLayout
{
public:
	WidgetsWindow();
	virtual ~WidgetsWindow();

	void initialise();

	void update(MyGUI::Widget* _current_widget) { current_widget = _current_widget; }

	void clearAllSheets();

	void clearNewWidget();
	void startNewWidget(int _x1, int _y1, MyGUI::MouseButton _id);
	void createNewWidget(int _x2, int _y2);
	void finishNewWidget(int _x2, int _y2);

	int getCreatingStatus() { return creating_status; }

	MyGUI::delegates::CDelegate1<MyGUI::Widget*> eventSelectWidget;
	MyGUI::delegates::CDelegate2<MyGUI::Widget*, const MyGUI::ToolTipInfo& > eventToolTip;

	MyGUI::Widget* getMainWidget() { return mMainWidget; }

private:
	void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo & _info) { eventToolTip(_sender, _info); }
	void notifySelectWidgetType(MyGUI::Widget* _sender);
	void notifySelectWidgetTypeDoubleclick(MyGUI::Widget* _sender);

	void updateSize();

private:
	MyGUI::Tab* mTabSkins;

	int widgetsButtonWidth;
	int widgetsButtonHeight;
	int widgetsButtonsInOneLine;
	std::string skinSheetName;

	MyGUI::Widget* current_widget;

	// info for new widget
	int x1, y1, x2, y2;
	// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
	int creating_status;

	std::string new_widget_type;
	std::string new_widget_skin;
	int mMaxLines;
};

#endif // __WIDGETS_WINDOW_H__
