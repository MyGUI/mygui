/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_H__
#define __PANEL_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class Panel : public BaseLayout
{
public:
	typedef MyGUI::delegates::CDelegate1<Panel*> DelegateUpdate;

	Panel();

	virtual void initialise(MyGUI::WidgetPtr _parent);

	DelegateUpdate eventUpdatePanel;

private:
	void notfyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
	void notifyUpdateAction(MyGUI::WidgetPtr _widget);

private:

	MyGUI::StaticTextPtr mTextCaption;
	MyGUI::ButtonPtr mButtonMinimize;
	MyGUI::WidgetPtr mWidgetClient;

	int m_minHeight;
	int m_maxHeight;
};

#endif // __PANEL_H__
