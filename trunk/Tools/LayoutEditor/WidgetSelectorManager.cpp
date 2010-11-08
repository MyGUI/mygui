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
		mSelectDepth(0)
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
		// некоторые обновл€ют кей фокус
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
		{
			if (checkContainer(container.current(), result, _point))
				break;
		}

		return result;
	}

	bool WidgetSelectorManager::checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point)
	{
		if (_container->widget->getAbsoluteCoord().inside(_point) && _container->widget->getVisible())
		{
			_result = _container->widget;

			for (std::vector<WidgetContainer*>::iterator item = _container->childContainers.begin(); item != _container->childContainers.end(); ++item)
			{
				if (_container->widget->isType<MyGUI::Tab>() && (*item)->widget->isType<MyGUI::TabItem>())
				{
					if (_container->widget->castType<MyGUI::Tab>()->getItemSelected() != (*item)->widget->castType<MyGUI::TabItem>())
						continue;
				}

				if (checkContainer(*item, _result, _point))
					break;
			}

			return true;
		}
		return false;
	}

	void WidgetSelectorManager::resetDepth()
	{
		mSelectDepth = 0;
	}

} // namespace tools
