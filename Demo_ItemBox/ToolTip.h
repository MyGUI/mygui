/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __TOOL_TIP_H__
#define __TOOL_TIP_H__

#include <MyGUI.h>

class ToolTipWindow
{
public:
	ToolTipWindow();

	void initialise();
	void shutdown();

	inline MyGUI::WidgetPtr operator->() const
	{
		MYGUI_DEBUG_ASSERT( mMainWidget, "error assign main widget");
		return mMainWidget;
	}

	inline MyGUI::WidgetPtr mainWidget()
	{
		MYGUI_DEBUG_ASSERT( mMainWidget, "error assign main widget");
		return mMainWidget;
	}

	void update(size_t count);

private:
	void loadLayout();

	template <typename T>
	void assignWidget(T * & _widget, const std::string & _name, bool _throw = true);

private:
	std::string mPrefix;
	std::string mLayoutName;
	MyGUI::VectorWidgetPtr mListWindowRoot;
	MyGUI::WidgetPtr mMainWidget;

	MyGUI::StaticTextPtr mTextName;
	MyGUI::StaticTextPtr mTextCount;
	MyGUI::StaticTextPtr mTextDesc;
};

#endif // __TOOL_TIP_H__