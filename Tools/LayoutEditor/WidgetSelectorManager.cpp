/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"

template <> tools::WidgetSelectorManager* MyGUI::Singleton<tools::WidgetSelectorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetSelectorManager>::mClassTypeName = "WidgetSelectorManager";

namespace tools
{

	WidgetSelectorManager::WidgetSelectorManager() :
		mCurrentWidget(nullptr),
		mSelectDepth(0),
		mStoreWidgetTag("LE_StoreWidgetTag")
	{
	}

	WidgetSelectorManager::~WidgetSelectorManager()
	{
	}

	void WidgetSelectorManager::initialise()
	{
	}

	void WidgetSelectorManager::shutdown()
	{
	}

	MyGUI::Widget* WidgetSelectorManager::getSelectedWidget()
	{
		return mCurrentWidget;
	}

	void WidgetSelectorManager::setSelectedWidget(MyGUI::Widget* _value)
	{
		// некоторые обновляют кей фокус
		//if (_value != mCurrentWidget)
		{
			mCurrentWidget = _value;
			mSelectDepth = 0;
			eventChangeSelectedWidget(mCurrentWidget);
		}
	}

	void WidgetSelectorManager::selectWidget(const MyGUI::IntPoint& _mousePosition)
	{
		if (mLastClickPoint != _mousePosition)
		{
			mSelectDepth = 0;
			mLastClickPoint = _mousePosition;
		}

		// здесь кликать вглубь
		MyGUI::Widget* item = getTopWidget(_mousePosition);
		if (nullptr != item)
		{
			// find widget registered as container
			while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
				item = item->getParent();
			MyGUI::Widget* oldItem = item;

			// try to selectin depth
			size_t depth = mSelectDepth;
			while (depth && (nullptr != item))
			{
				item = item->getParent();
				while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
					item = item->getParent();
				MYGUI_ASSERT(depth != 0, "depth != 0");
				depth--;
			}

			if (nullptr == item)
			{
				item = oldItem;
				mSelectDepth = 0;
			}

			// found widget
			if (nullptr != item)
			{
				depth =  mSelectDepth;
				WidgetSelectorManager::getInstance().setSelectedWidget(item);
				mSelectDepth = depth + 1;
			}
			else
			{
				WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
			}
		}
		else
		{
			WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
		}
	}

	MyGUI::Widget* WidgetSelectorManager::getTopWidget(const MyGUI::IntPoint& _point)
	{
		MyGUI::Widget* result = nullptr;

		EnumeratorWidgetContainer container = EditorWidgets::getInstance().getWidgets();
		while (container.next())
			checkContainer(container.current(), result, _point);

		return result;
	}

	void WidgetSelectorManager::checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point)
	{
		if (!_container->getWidget()->getVisible())
			return;

		if (_container->getWidget()->getAbsoluteCoord().inside(_point))
		{
			if (_result != nullptr)
			{
				if (_result->getParent() == _container->getWidget()->getParent())
				{
					if (_result->getDepth() < _container->getWidget()->getDepth())
						return;
				}
			}

			_result = _container->getWidget();
		}

		for (std::vector<WidgetContainer*>::iterator item = _container->childContainers.begin(); item != _container->childContainers.end(); ++item)
		{
			if (_container->getWidget()->isType<MyGUI::TabControl>() && (*item)->getWidget()->isType<MyGUI::TabItem>())
			{
				if (_container->getWidget()->castType<MyGUI::TabControl>()->getItemSelected() != (*item)->getWidget()->castType<MyGUI::TabItem>())
					continue;
			}

			checkContainer(*item, _result, _point);
		}
	}

	void WidgetSelectorManager::resetDepth()
	{
		mSelectDepth = 0;
	}

	void WidgetSelectorManager::saveSelectedWidget()
	{
		if (mCurrentWidget != nullptr)
		{
			WidgetContainer* container = EditorWidgets::getInstance().find(mCurrentWidget);
			if (container != nullptr)
				container->setUserData(mStoreWidgetTag, mStoreWidgetTag);
		}
	}

	void WidgetSelectorManager::restoreSelectedWidget()
	{
		MyGUI::Widget* widget = findWidgetSelected();
		setSelectedWidget(widget);
	}

	MyGUI::Widget* WidgetSelectorManager::findWidgetSelected()
	{
		MyGUI::Widget* result = nullptr;
		EnumeratorWidgetContainer container = EditorWidgets::getInstance().getWidgets();
		while (container.next())
		{
			MyGUI::Widget* widget = findWidgetSelected(container.current());
			if (widget != nullptr)
			{
				result = widget;
				break;
			}
		}
		return result;
	}

	MyGUI::Widget* WidgetSelectorManager::findWidgetSelected(WidgetContainer* _container)
	{
		MyGUI::Widget* result = nullptr;
		bool existStoreTag = _container->existUserData(mStoreWidgetTag);
		if (existStoreTag)
		{
			result = _container->getWidget();
			_container->clearUserData(mStoreWidgetTag);
		}

		if (result == nullptr)
		{
			for (std::vector<WidgetContainer*>::iterator item = _container->childContainers.begin(); item != _container->childContainers.end(); ++item)
			{
				MyGUI::Widget* widget = findWidgetSelected(*item);
				if (widget != nullptr)
				{
					result = widget;
					break;
				}
			}
		}

		return result;
	}

}
