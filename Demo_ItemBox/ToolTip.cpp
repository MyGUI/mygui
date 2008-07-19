/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ToolTip.h"

const std::string MAIN_WINDOW = "_Main";

ToolTipWindow::ToolTipWindow() :
	mMainWidget(null)
{
	mPrefix = MyGUI::utility::toString(this, "_");
	mLayoutName = "ToolTip.layout";
}

void ToolTipWindow::loadLayout()
{
	mListWindowRoot = MyGUI::LayoutManager::getInstance().loadLayout(mLayoutName, mPrefix, 0);
	const std::string main_name = mPrefix + MAIN_WINDOW;
	for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
		if ((*iter)->getName() == main_name) {
			mMainWidget = (*iter);
			return;
		}
	}

	MYGUI_EXCEPT("root widget name '" << MAIN_WINDOW << "' in layout '" << mLayoutName << "' not found.");
}

void ToolTipWindow::initialise()
{
	loadLayout();

	assignWidget(mTextName, "text_Name");
	assignWidget(mTextCount, "text_Count");
	assignWidget(mTextDesc, "text_Desc");
}

void ToolTipWindow::shutdown()
{
	MyGUI::LayoutManager::getInstance().unloadLayout(mListWindowRoot);
}

template <typename T>
void ToolTipWindow::assignWidget(T * & _widget, const std::string & _name, bool _throw)
{
	_widget = null;
	for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
		MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
		if (null != find) {
			T * cast = dynamic_cast<T*>(find);
			if (null != cast) {
				_widget = cast;
			}
			else if (_throw) {
					MYGUI_EXCEPT("Error dynamic cast : dest type = '" << T::_getType() 
					<< "' source name = '" << find->getName() 
					<< "' source type = '" << find->getWidgetType() << "' in layout '" << mLayoutName << "'");
			}
			return;

		}
	}
	MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
}

void ToolTipWindow::update(size_t count)
{
	mTextCount->setCaption(MyGUI::utility::toString(count));
}
