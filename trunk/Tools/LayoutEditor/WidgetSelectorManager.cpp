/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"

template <> tools::WidgetSelectorManager* MyGUI::Singleton<tools::WidgetSelectorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetSelectorManager>::mClassTypeName("WidgetSelectorManager");

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
		// ��������� ��������� ��� �����
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

		// ����� ������� ������
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
		if (!_container->widget->getVisible())
			return;

		if (_container->widget->getAbsoluteCoord().inside(_point))
			_result = _container->widget;

		for (std::vector<WidgetContainer*>::iterator item = _container->childContainers.begin(); item != _container->childContainers.end(); ++item)
		{
			if (_container->widget->isType<MyGUI::TabControl>() && (*item)->widget->isType<MyGUI::TabItem>())
			{
				if (_container->widget->castType<MyGUI::TabControl>()->getItemSelected() != (*item)->widget->castType<MyGUI::TabItem>())
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
				container->mUserString.push_back(MyGUI::PairString(mStoreWidgetTag, mStoreWidgetTag));
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
		for (MyGUI::VectorStringPairs::iterator item = _container->mUserString.begin(); item != _container->mUserString.end(); ++ item)
		{
			if ((*item).first == mStoreWidgetTag)
			{
				result = _container->widget;
				_container->mUserString.erase(item);
				break;
			}
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

} // namespace tools
