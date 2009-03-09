/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __WIDGETS_WINDOW_H__
#define __WIDGETS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"

class WidgetsWindow : public wraps::BaseLayout
{
public:
	WidgetsWindow();

	void initialise();

	void update(MyGUI::WidgetPtr _current_widget){current_widget = _current_widget;};
	void load(MyGUI::xml::ElementEnumerator field);
	void save(MyGUI::xml::ElementPtr root);

	void clearNewWidget();
	void startNewWidget(int _x1, int _y1, int _id);
	void createNewWidget(int _x2, int _y2);
	void finishNewWidget(int _x2, int _y2);

	int getCreatingStatus(){return creating_status;}

	MyGUI::EventHandle_WidgetVoid eventSelectWidget;
	MyGUI::EventHandle_WidgetToolTip eventToolTip;

	MyGUI::WidgetPtr getMainWidget() { return mMainWidget; }

private:
	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info){eventToolTip(_sender, _info);};
	void notifySelectWidgetType(MyGUI::WidgetPtr _sender);
	void notifySelectWidgetTypeDoubleclick(MyGUI::WidgetPtr _sender);

private:
	MyGUI::TabPtr mTabSkins;

	int widgetsButtonWidth;
	int widgetsButtonHeight;
	int widgetsButtonsInOneLine;
	std::string skinSheetName;

	MyGUI::WidgetPtr current_widget;

	// info for new widget
	int x1, y1, x2, y2;
	// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
	int creating_status;

	std::string new_widget_type;
	std::string new_widget_skin;
};

#endif // __WIDGETS_WINDOW_H__
