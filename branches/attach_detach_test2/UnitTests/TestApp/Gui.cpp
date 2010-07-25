
#include "Gui.h"

template <> const char* MyGUI::Singleton<MyGUI::Gui2>::mClassTypeName("Gui2");

namespace MyGUI
{
	Gui2::Gui2()
	{
	}

	Gui2::~Gui2()
	{
		MYGUI_ASSERT(mChilds.empty(), "not clear");
	}

	Control* Gui2::createChild()
	{
		Control* widget = new Control();
		widget->initialise();

		attachControl(widget);

		return widget;
	}

	void Gui2::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Gui2::destroyChild(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");
		detachControl(_widget);

		_widget->shutdown();
		delete _widget;
	}

	size_t Gui2::getChildCount()
	{
		return mChilds.size();
	}

	Control* Gui2::getChild(size_t _index)
	{
		MYGUI_ASSERT(_index < mChilds.size(), "index out of range");
		return mChilds.at(_index);
	}

	void Gui2::detachControl(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		removeChild(_widget);
	}

	void Gui2::attachControl(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		addChild(_widget);
	}

	void Gui2::addChild(Control* _widget)
	{
		MYGUI_ASSERT(_widget->getParent() == nullptr, "allready added");
		MYGUI_ASSERT(_widget->getParentContainer() == nullptr, "allready added");

		mChilds.push_back(_widget);
		//_child->mParent = this;
		_widget->mParentContainer = this;
	}

	void Gui2::removeChild(Control* _widget)
	{
		VectorControlPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _widget);
		if (item != mChilds.end())
		{
			mChilds.erase(item);
			//_child->mParent = nullptr;
			_widget->mParentContainer = nullptr;
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}
	}
}