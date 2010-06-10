
#include "Widget.h"

namespace demo
{
	Widget::Widget() :
		mVisualParent(nullptr),
		mParent(nullptr),
		mClient(nullptr)
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

	Widget* Widget::createWidget(const std::string& _skin)
	{
		Widget* widget = new Widget();
		widget->initialise();

		widget->createSkin(_skin);

		addChild(widget);
		addVisualChildToClient(widget);

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

	void Widget::addVisualChildToClient(Widget* _child)
	{
		if (mClient != nullptr)
			mClient->addVisualChild(_child);
		else
			addVisualChild(_child);
	}

	void Widget::removeVisualChildFromClient(Widget* _child)
	{
		if (mClient != nullptr)
			mClient->removeVisualChild(_child);
		else
			removeVisualChild(_child);
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

	void Widget::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Widget::destroyChild(Widget* _widget)
	{
		removeChild(_widget);
		removeVisualChildFromClient(_widget);

		_widget->shutdown();
		delete _widget;
	}

	void Widget::addVisualChild(Widget* _child)
	{
		MYGUI_ASSERT(_child->getVisualParent() == nullptr, "allready added");
		mVisualChilds.push_back(_child);
		_child->mVisualParent = this;

		onVisualChildAdded(_child);
	}

	void Widget::removeVisualChild(Widget* _child)
	{
		VectorWidgetPtr::iterator item = std::remove(mVisualChilds.begin(), mVisualChilds.end(), _child);
		if (item != mVisualChilds.end())
		{
			mVisualChilds.erase(item);
			_child->mVisualParent = nullptr;

			onVisualChildRemoved(_child);
		}
		else
		{
			MYGUI_EXCEPT("widget not found");
		}
	}

	void Widget::addChild(Widget* _child)
	{
		MYGUI_ASSERT(_child->getParent() == nullptr, "allready added");
		mChilds.push_back(_child);
		_child->mParent = this;

		onChildAdded(_child);
	}

	void Widget::removeChild(Widget* _child)
	{
		VectorWidgetPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _child);
		if (item != mChilds.end())
		{
			mChilds.erase(item);
			_child->mParent = nullptr;

			onChildRemoved(_child);
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

}