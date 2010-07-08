
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

	Widget* Gui::createChild()
	{
		Widget* widget = new Widget();
		widget->initialise();

		attachWidget(widget);

		return widget;
	}

	void Gui::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Gui::destroyChild(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");
		detachWidget(_widget);

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

	void Gui::detachWidget(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		removeChild(_widget);
	}

	void Gui::attachWidget(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		addChild(_widget);
	}

	void Gui::addChild(Widget* _widget)
	{
		MYGUI_ASSERT(_widget->getParent() == nullptr, "allready added");
		mChilds.push_back(_widget);
		//_child->mParent = this;
	}

	void Gui::removeChild(Widget* _widget)
	{
		VectorWidgetPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _widget);
		if (item != mChilds.end())
		{
			mChilds.erase(item);
			//_child->mParent = nullptr;
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}
	}
}