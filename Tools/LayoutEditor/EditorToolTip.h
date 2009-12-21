/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*/
#ifndef __EDITOR_TOOL_TIP_H__
#define __EDITOR_TOOL_TIP_H__

#include "BaseLayout/BaseLayout.h"

class EditorToolTip : public wraps::BaseLayout
{
public:
	EditorToolTip();

	void show(const MyGUI::UString & _text, const MyGUI::IntPoint & _point);
	void show(MyGUI::Widget* _sender, const MyGUI::IntPoint & _point);
	void hide();

private:
	void setPosition(const MyGUI::IntPoint & _point);
	MyGUI::Edit* mText;

	int minWidth;
	int minHeight;
	MyGUI::Widget* lastWidget;
};

#endif // __EDITOR_TOOL_TIP_H__
