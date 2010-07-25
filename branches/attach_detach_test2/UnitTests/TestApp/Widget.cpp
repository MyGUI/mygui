
#include "Widget.h"

namespace MyGUI
{
	Control::Control() :
		mVisualParent(nullptr),
		mParent(nullptr),
		mClient(nullptr),
		mParentContainer(nullptr)
	{
	}

	Control::~Control()
	{
		MYGUI_ASSERT(mVisualParent == nullptr, "not clear");
		MYGUI_ASSERT(mVisualChilds.empty(), "not clear");
		MYGUI_ASSERT(mParent == nullptr, "not clear");
		MYGUI_ASSERT(mChilds.empty(), "not clear");
		MYGUI_ASSERT(mClient == nullptr, "not clear");
	}

	void Control::initialise()
	{
	}

	void Control::shutdown()
	{
		destroyAllChilds();
		destroySkin();
	}

	Control* Control::createChild()
	{
		Control* widget = new Control();
		widget->initialise();

		attachControl(widget);

		return widget;
	}

	void Control::addVisualChildToClient(Control* _widget)
	{
		if (mClient != nullptr)
			mClient->addVisualChild(_widget);
		else
			addVisualChild(_widget);
	}

	void Control::removeVisualChildFromClient(Control* _widget)
	{
		if (mClient != nullptr)
			mClient->removeVisualChild(_widget);
		else
			removeVisualChild(_widget);
	}

	void Control::changeSkin(const std::string& _skinName)
	{
		for (VectorControlPtr::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
			removeVisualChildFromClient(*item);

		destroySkin();
		ResourceSkin* info = SkinManager::getInstance().getByName(_skinName);
		createSkin(info);

		for (VectorControlPtr::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
			addVisualChildToClient(*item);
	}

	void Control::destroySkin()
	{
		onDestroySkin();

		VectorControlPtr skinControls;
		for (VectorControlPtr::iterator item=mVisualChilds.begin(); item!=mVisualChilds.end(); ++item)
		{
			// если виджет есть в логических детях, значит не скин
			if (std::find(mChilds.begin(), mChilds.end(), *item) == mChilds.end())
				skinControls.push_back(*item);
		}
		for (VectorControlPtr::iterator item=skinControls.begin(); item!=skinControls.end(); ++item)
		{
			Control* widget = *item;
			removeVisualChild(widget);

			widget->shutdown();
			delete widget;
		}
		mClient = nullptr;
	}

	void Control::createSkin(ResourceSkin* _info)
	{
		// создаем детей скина
		const VectorChildSkinInfo& child = _info->getChild();
		for (VectorChildSkinInfo::const_iterator iter=child.begin(); iter!=child.end(); ++iter)
		{
			Control* widget = new Control();
			widget->changeSkin(iter->skin);
			widget->setCoord(iter->coord);

			addVisualChild(widget);

			if (iter->name == "Client")
				mClient = widget;
		}

		onCreateSkin(_info);
	}

	void Control::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	void Control::destroyChild(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		detachControl(_widget);

		_widget->shutdown();
		delete _widget;
	}

	void Control::addVisualChild(Control* _widget)
	{
		//MYGUI_ASSERT(_widget->getVisualParent() == nullptr, "allready added");

		mVisualChilds.push_back(_widget);
		_widget->mVisualParent = this;

		onVisualChildAdded(_widget);
	}

	void Control::removeVisualChild(Control* _widget)
	{
		VectorControlPtr::iterator item = std::remove(mVisualChilds.begin(), mVisualChilds.end(), _widget);
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

	void Control::addChild(Control* _widget)
	{
		MYGUI_ASSERT(_widget->getParent() == nullptr, "allready added");
		MYGUI_ASSERT(_widget->getParentContainer() == nullptr, "allready added");

		mChilds.push_back(_widget);
		_widget->mParent = this;
		_widget->mParentContainer = this;
	}

	void Control::removeChild(Control* _widget)
	{
		VectorControlPtr::iterator item = std::remove(mChilds.begin(), mChilds.end(), _widget);
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

	size_t Control::getChildCount()
	{
		return mChilds.size();
	}

	Control* Control::getChild(size_t _index)
	{
		MYGUI_ASSERT(_index < mChilds.size(), "index out of range");
		return mChilds.at(_index);
	}

	void Control::detachControl(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		removeChild(_widget);
		removeVisualChildFromClient(_widget);
	}

	void Control::attachControl(Control* _widget)
	{
		MYGUI_ASSERT(_widget != nullptr, "null referense");

		addChild(_widget);
		addVisualChildToClient(_widget);
	}

	void Control::setCoord(const IntCoord& _value)
	{
		mCoord = _value;
	}
}