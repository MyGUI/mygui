/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "WidgetCreatorManager.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"

template <> tools::WidgetCreatorManager* MyGUI::Singleton<tools::WidgetCreatorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetCreatorManager>::mClassTypeName("WidgetCreatorManager");

namespace tools
{
	WidgetCreatorManager::WidgetCreatorManager() :
		mMouseButtonPressed(false),
		mSelectDepth(0),
		mCreateMode(false),
		mStartNewWidget(false),
		mNewWidget(nullptr)
		//mPositionSelectorControl(nullptr)
	{
	}

	WidgetCreatorManager::~WidgetCreatorManager()
	{
	}

	void WidgetCreatorManager::initialise()
	{
		//mPositionSelectorControl = new PositionSelectorControl();
		//mPositionSelectorControl->setVisible(false);

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);
	}

	void WidgetCreatorManager::shutdown()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);

		resetWidget();

		//delete mPositionSelectorControl;
		//mPositionSelectorControl = nullptr;
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
		if ((abs(mLastClick.left - _point.left) > distance) || (abs(mLastClick.top - _point.top) > distance))
		{
			mSelectDepth = 0;
		}
	}

	void WidgetCreatorManager::notifyMouseMouseDrag(const MyGUI::IntPoint& _point)
	{
		mMouseButtonPressed = false;

		if (getCreateMode())
			WidgetCreatorManager::getInstance().moveNewWidget();
	}

	void WidgetCreatorManager::notifyMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
		mLastClick = _point;
		mMouseButtonPressed = true;

		if (getCreateMode())
			createNewWidget();
	}

	void WidgetCreatorManager::notifyMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
		if (getCreateMode())
			finishNewWidget();

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

	void WidgetCreatorManager::setCreatorInfo(const std::string& _widgetType, const std::string& _widgetSkin)
	{
		mWidgetType = _widgetType;
		mWidgetSkin = _widgetSkin;
		mCreateMode = true;

		eventChangeCreatorMode(mCreateMode);
	}

	void WidgetCreatorManager::resetCreatorInfo()
	{
		mWidgetType = "";
		mWidgetSkin = "";
		mCreateMode = false;

		eventChangeCreatorMode(mCreateMode);
	}

	bool WidgetCreatorManager::getCreateMode()
	{
		return mCreateMode;
	}

	const std::string& WidgetCreatorManager::getWidgetType()
	{
		return mWidgetType;
	}

	const std::string& WidgetCreatorManager::getWidgetSkin()
	{
		return mWidgetSkin;
	}

	void WidgetCreatorManager::createNewWidget()
	{
		mStartNewWidget = true;
		mStartPoint = MyGUI::InputManager::getInstance().getMousePosition();

		//mPositionSelectorControl->setVisible(false);
		resetWidget();
	}

	void WidgetCreatorManager::moveNewWidget()
	{
		if (mNewWidget == nullptr)
		{
			// тип виджета может отсутсвовать
			if (!MyGUI::WidgetManager::getInstance().isFactoryExist(mWidgetType))
				return;

			std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", mWidgetType, EditorWidgets::getInstance().getNextGlobalCounter());

			MyGUI::Widget* current = WidgetSelectorManager::getInstance().getSelectedWidget();

			// пока не найдем ближайшего над нами способного быть родителем
			while (current && !WidgetTypes::getInstance().findWidgetStyle(current->getTypeName())->parent)
				current = current->getParent();

			if (current && WidgetTypes::getInstance().findWidgetStyle(mWidgetType)->child)
				mNewWidget = current->createWidgetT(mWidgetType, mWidgetSkin, MyGUI::IntCoord(), MyGUI::Align::Default, tmpname);
			else
				mNewWidget = MyGUI::Gui::getInstance().createWidgetT(mWidgetType, mWidgetSkin, MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER, tmpname);

			if (mNewWidget->isType<MyGUI::StaticText>())
				mNewWidget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", mWidgetSkin));

			//mPositionSelectorControl->setVisible(true);
		}

		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition();
		MyGUI::IntPoint offset;

		MyGUI::Widget* parent = mNewWidget->getParent();
		if (parent != nullptr)
			offset = parent->getAbsolutePosition();

		MyGUI::IntCoord coord = MyGUI::IntCoord(
			std::min(mStartPoint.left - offset.left, point.left - offset.left),
			std::min(mStartPoint.top - offset.top, point.top - offset.top),
			abs(point.left - mStartPoint.left),
			abs(point.top - mStartPoint.top));

		mNewWidget->setCoord(coord);
		//mPositionSelectorControl->setCoord(mNewWidget->getAbsoluteCoord());
	}

	void WidgetCreatorManager::finishNewWidget()
	{
		resetCreatorInfo();
		//mPositionSelectorControl->setVisible(false);

		if (mNewWidget != nullptr)
		{
			MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition();
			MyGUI::IntPoint offset;

			MyGUI::Widget* parent = mNewWidget->getParent();
			if (parent != nullptr)
				offset = parent->getAbsolutePosition();

			MyGUI::IntCoord coord = MyGUI::IntCoord(
				std::min(mStartPoint.left - offset.left, point.left - offset.left),
				std::min(mStartPoint.top - offset.top, point.top - offset.top),
				abs(point.left - mStartPoint.left),
				abs(point.top - mStartPoint.top));

			if ((coord.width * coord.height) != 0)
			{
				mNewWidget->setCoord(coord);

				// создали виджет, все счастливы
				WidgetContainer * widgetContainer = new WidgetContainer(mWidgetType, mWidgetSkin, mNewWidget);
				mNewWidget = nullptr;

				EditorWidgets::getInstance().add(widgetContainer);
				WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);
				UndoManager::getInstance().addValue();
			}
			else
			{
				// не удалось создать, т.к. размер нулевой
				resetWidget();
			}
		}
	}

	void WidgetCreatorManager::resetWidget()
	{
		// подстрахуемся
		if (mNewWidget != nullptr)
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(mNewWidget);
			mNewWidget = nullptr;
		}
	}

} // namespace tools
