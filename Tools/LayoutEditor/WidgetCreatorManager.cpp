/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "WidgetCreatorManager.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"

template <> tools::WidgetCreatorManager* MyGUI::Singleton<tools::WidgetCreatorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetCreatorManager>::mClassTypeName("WidgetCreatorManager");

namespace tools
{
	WidgetCreatorManager::WidgetCreatorManager() :
		mMouseButtonPressed(false),
		mLastClickX(0),
		mLastClickY(0),
		mSelectDepth(0)
	{
	}

	WidgetCreatorManager::~WidgetCreatorManager()
	{
	}

	void WidgetCreatorManager::initialise()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);
	}

	void WidgetCreatorManager::shutdown()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);
	}

	void WidgetCreatorManager::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		if (_currentWidget == nullptr)
			mSelectDepth = 0;
	}

	void WidgetCreatorManager::notifyMouseMouseMove(const MyGUI::IntPoint& _point)
	{
		mMouseButtonPressed = false;

		const int distance = 2;
		if ((abs(mLastClickX - _point.left) > distance) || (abs(mLastClickY - _point.top) > distance))
		{
			mSelectDepth = 0;
		}
	}

	void WidgetCreatorManager::notifyMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
		mLastClickX = _point.left;
		mLastClickY = _point.top;
		mMouseButtonPressed = true;
	}

	void WidgetCreatorManager::notifyMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
		if (mMouseButtonPressed)
		{
			mMouseButtonPressed = false;

			// здесь кликать вглубь
			MyGUI::Widget* item = getTopWidget(MyGUI::InputManager::getInstance().getLastLeftPressed());
			if (nullptr != item)
			{
				// find widget registered as container
				while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
					item = item->getParent();
				MyGUI::Widget* oldItem = item;

				// try to selectin depth
				int depth = mSelectDepth;
				while (depth && (nullptr != item))
				{
					item = item->getParent();
					while ((nullptr == EditorWidgets::getInstance().find(item)) && (nullptr != item))
						item = item->getParent();
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
					WidgetSelectorManager::getInstance().setSelectedWidget(item);
					mSelectDepth++;
				}
			}
		}
	}

	MyGUI::Widget* WidgetCreatorManager::getTopWidget(const MyGUI::IntPoint& _point)
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

	bool WidgetCreatorManager::checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point)
	{
		if (_container->widget->getAbsoluteCoord().inside(_point))
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

} // namespace tools
