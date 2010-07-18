
#include "Widget.h"

namespace demo
{
	Widget::Widget() :
		mVisualParent(nullptr),
		mParent(nullptr),
		mClient(nullptr),
		mParentContainer(nullptr)
	{
	}

	Widget::~Widget()
	{
		MYGUI_ASSERT(mVisualParent == nullptr, "not clear");
		MYGUI_ASSERT(mVisualChilds.empty(), "not clear");
		MYGUI_ASSERT(mParent == nullptr, "not clear");
		MYGUI_ASSERT(mChilds.empty(), "not clear");
		MYGUI_ASSERT(mClient == nullptr, "not clear");
	}

	void Widget::initialise()
	{
	}

	void Widget::shutdown()
	{
		destroyAllChilds();
		destroySkin();
	}

	Widget* Widget::createChild()
	{
		Widget* widget = new Widget();
		widget->initialise();

		attachWidget(widget);

		return widget;
	}

	void Widget::changeSkin(const std::string& _skin)
	{
		for (VectorWidgetPtr::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
			removeVisualChildFromClient(*item);

		destroySkin();
		createSkin(_skin);

		for (VectorWidgetPtr::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
			addVisualChildToClient(*item);
	}

	void Widget::addVisualChildToClient(Widget* _widget)
	{
		if (mClient != nullptr)
			mClient->addVisualChild(_widget);
		else
			addVisualChild(_widget);
	}

	void Widget::removeVisualChildFromClient(Widget* _widget)
	{
		if (mClient != nullptr)
			mClient->removeVisualChild(_widget);
		else
			removeVisualChild(_widget);
	}

	void Widget::destroySkin()
	{
		onDestroySkin();

		VectorWidgetPtr skinWidgets;
		for (VectorWidgetPtr::iterator item=mVisualChilds.begin(); item!=mVisualChilds.end(); ++item)
		{
			// если виджет есть в логических дет€х, значит не скин
			if (std::find(mChilds.begin(), mChilds.end(), *item) == mChilds.end())
				skinWidgets.push_back(*item);
		}
		for (VectorWidgetPtr::iterator item=skinWidgets.begin(); item!=skinWidgets.end(); ++item)
		{
			Widget* widget = *item;
			removeVisualChild(widget);

			widget->shutdown();
			delete widget;
		}
		mClient = nullptr;
	}

	void Widget::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Widget::destroyChild(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		detachWidget(_widget);

		_widget->shutdown();
		delete _widget;
	}

	void Widget::createSkin(const std::string& _skin)
	{
		if (_skin != "")
		{
			Widget* widget = new Widget();
			widget->createSkin("");

			addVisualChild(widget);

			mClient = widget;
		}

		onCreateSkin(_skin);
	}

	void Widget::addVisualChild(Widget* _widget)
	{
		MYGUI_ASSERT(_widget->getVisualParent() == nullptr, "allready added");

		mVisualChilds.push_back(_widget);
		_widget->mVisualParent = this;

		onVisualChildAdded(_widget);
	}

	void Widget::removeVisualChild(Widget* _widget)
	{
		VectorWidgetPtr::iterator item = std::remove(mVisualChilds.begin(), mVisualChilds.end(), _widget);
		if (item != mVisualChilds.end())
		{
			mVisualChilds.erase(item);
			_widget->mVisualParent = nullptr;

			onVisualChildRemoved(_widget);
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}
	}

	void Widget::addChild(Widget* _widget)
	{
		MYGUI_ASSERT(_widget->getParent() == nullptr, "allready added");
		MYGUI_ASSERT(_widget->getParentContainer() == nullptr, "allready added");

		mChilds.push_back(_widget);
		_widget->mParent = this;
		_widget->mParentContainer = this;
	}

	void Widget::removeChild(Widget* _widget)
	{
		VectorWidgetPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _widget);
		if (item != mChilds.end())
		{
			mChilds.erase(item);
			_widget->mParent = nullptr;
			_widget->mParentContainer = nullptr;
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}
	}

	size_t Widget::getChildCount()
	{
		return mChilds.size();
	}

	Widget* Widget::getChild(size_t _index)
	{
		MYGUI_ASSERT(_index < mChilds.size(), "index out of range");
		return mChilds.at(_index);
	}

	void Widget::detachWidget(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		removeChild(_widget);
		removeVisualChildFromClient(_widget);
	}

	void Widget::attachWidget(Widget* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		addChild(_widget);
		addVisualChildToClient(_widget);
	}

}