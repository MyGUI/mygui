
#include "Gui.h"

template <> const char* MyGUI::Singleton<demo::Gui>::mClassTypeName("Gui");

namespace demo
{
	Gui::Gui()
	{
	}

	Gui::~Gui()
	{
		MYGUI_ASSERT(mChilds.empty(), "not clear");
	}

	Widget* Gui::createWidget(const std::string& _skin)
	{
		Widget* widget = new Widget();
		widget->initialise();

		widget->createSkin(_skin);

		mChilds.push_back(widget);

		return widget;
	}

	void Gui::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Gui::destroyChild(Widget* _widget)
	{
		VectorWidgetPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _widget);
		if (item != mChilds.end())
		{
			mChilds.erase(item);
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}

		_widget->shutdown();
		delete _widget;
	}

	size_t Gui::getChildCount()
	{
		return mChilds.size();
	}

	Widget* Gui::getChild(size_t _index)
	{
		MYGUI_ASSERT(_index < mChilds.size(), "index out of range");
		return mChilds.at(_index);
	}

}