/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "BaseLayout.h"

const std::string MAIN_WINDOW = "_Main";

BaseLayout::BaseLayout() :
	mMainWidget(null),
	mParentWidget(null)
{
	mPrefix = MyGUI::utility::toString(this, "_");
}

BaseLayout::BaseLayout(const std::string & _layout) :
	mMainWidget(null),
	mParentWidget(null),
	mLayoutName(_layout)
{
	mPrefix = MyGUI::utility::toString(this, "_");
}

void BaseLayout::loadLayout(MyGUI::WidgetPtr _parent)
{
	mParentWidget = _parent;
	mListWindowRoot = MyGUI::LayoutManager::getInstance().loadLayout(mLayoutName, mPrefix, _parent);

	//if (mParentWidget == null) {
		const std::string main_name = mPrefix + MAIN_WINDOW;
		for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
			if ((*iter)->getName() == main_name) {
				mMainWidget = (*iter);
				return;
			}
		}
		MYGUI_EXCEPT("root widget name '" << MAIN_WINDOW << "' in layout '" << mLayoutName << "' not found.");
	//}
}

void BaseLayout::initialise()
{
	loadLayout();
}

void BaseLayout::initialise(MyGUI::WidgetPtr _parent)
{
	loadLayout(_parent);
}

void BaseLayout::shutdown()
{
	if (mParentWidget == null) {
		MyGUI::LayoutManager::getInstance().unloadLayout(mListWindowRoot);
	}
	mListWindowRoot.clear();
}

void BaseLayout::assignWidget(BaseLayout & _base, const std::string & _name, bool _throw)
{
	MyGUI::WidgetPtr widget = null;
	assignWidget(widget, _name, _throw);
	_base.initialise(widget);
}

