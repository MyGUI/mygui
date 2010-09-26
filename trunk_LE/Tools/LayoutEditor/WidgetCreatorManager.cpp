/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "WidgetCreatorManager.h"
#include "WidgetSelectorManager.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "SettingsManager.h"

template <> tools::WidgetCreatorManager* MyGUI::Singleton<tools::WidgetCreatorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetCreatorManager>::mClassTypeName("WidgetCreatorManager");

namespace tools
{
	WidgetCreatorManager::WidgetCreatorManager() :
		//mSelectDepth(0),
		//mMouseButtonPressed(false),
		mCreateMode(false),
		mStartNewWidget(false),
		mNewWidget(nullptr),
		mGridStep(0),
		mPositionSelectorControl(nullptr)
	{
	}

	WidgetCreatorManager::~WidgetCreatorManager()
	{
	}

	void WidgetCreatorManager::initialise()
	{
		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &WidgetCreatorManager::notifySettingsChanged);

		mPositionSelectorControl = new PositionSelectorControl();
		mPositionSelectorControl->setVisible(false);

		//WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);
	}

	void WidgetCreatorManager::shutdown()
	{
		//WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WidgetCreatorManager::notifyChangeSelectedWidget);
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &WidgetCreatorManager::notifySettingsChanged);

		resetWidget();

		delete mPositionSelectorControl;
		mPositionSelectorControl = nullptr;
	}

	/*void WidgetCreatorManager::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		if (_currentWidget == nullptr)
			mSelectDepth = 0;
	}*/

	/*void WidgetCreatorManager::notifyMouseMouseMove(const MyGUI::IntPoint& _point)
	{
		mMouseButtonPressed = false;

		const int distance = 2;
		if ((abs(mLastClick.left - _point.left) > distance) || (abs(mLastClick.top - _point.top) > distance))
		{
			mSelectDepth = 0;
		}
	}*/

	void WidgetCreatorManager::notifyMouseDrag(const MyGUI::IntPoint& _point)
	{
		//mMouseButtonPressed = false;

		if (getCreateMode())
			moveNewWidget(_point);
	}

	void WidgetCreatorManager::notifyMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
		//mLastClick = _point;
		//mMouseButtonPressed = true;

		if (getCreateMode())
			createNewWidget(_point);
	}

	void WidgetCreatorManager::notifyMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
		if (getCreateMode())
			finishNewWidget(_point);

		/*if (mMouseButtonPressed)
		{
			mMouseButtonPressed = false;

			WidgetSelectorManager::getInstance().selectWidget();
		}*/
	}

	/*MyGUI::Widget* WidgetCreatorManager::getTopWidget(const MyGUI::IntPoint& _point)
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
	}*/

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

	void WidgetCreatorManager::createNewWidget(const MyGUI::IntPoint& _point)
	{
		mStartNewWidget = true;
		mStartPoint = _point;//getMousePosition();
		mStartPoint.left += mGridStep / 2;
		mStartPoint.top += mGridStep / 2;

		mPositionSelectorControl->setVisible(false);
		resetWidget();
	}

	void WidgetCreatorManager::moveNewWidget(const MyGUI::IntPoint& _point)
	{
		if (mNewWidget == nullptr)
		{
			// тип виджета может отсутсвовать
			if (!MyGUI::WidgetManager::getInstance().isFactoryExist(mWidgetType))
				return;

			// выделяем верний виджет
			// FIXME
			WidgetSelectorManager::getInstance().selectWidget(mStartPoint);

			// если будет глючить то вернуть
			//std::string tmpname = MyGUI::utility::toString("LayoutEditorWidget_", mWidgetType, EditorWidgets::getInstance().getNextGlobalCounter());

			MyGUI::Widget* parent = WidgetSelectorManager::getInstance().getSelectedWidget();

			// пока не найдем ближайшего над нами способного быть родителем
			while (parent != nullptr && !WidgetTypes::getInstance().findWidgetStyle(parent->getTypeName())->parent)
				parent = parent->getParent();

			if (!WidgetTypes::getInstance().findWidgetStyle(mWidgetType)->child)
				parent = nullptr;

			if (parent != nullptr)
				mNewWidget = parent->createWidgetT(mWidgetType, EditorWidgets::getInstance().getSkinReplace(mWidgetSkin), MyGUI::IntCoord(), MyGUI::Align::Default);
			else
				mNewWidget = MyGUI::Gui::getInstance().createWidgetT(mWidgetType, EditorWidgets::getInstance().getSkinReplace(mWidgetSkin), MyGUI::IntCoord(), MyGUI::Align::Default, DEFAULT_EDITOR_LAYER);

			if (mNewWidget->isType<MyGUI::StaticText>())
				mNewWidget->castType<MyGUI::StaticText>()->setCaption(MyGUI::utility::toString("#888888", mWidgetSkin));

			// переводим старт поинт в координаты отца
			if (parent != nullptr)
				mStartPoint -= parent->getAbsolutePosition();

			if (!MyGUI::InputManager::getInstance().isShiftPressed())
			{
				mStartPoint.left = toGrid(mStartPoint.left);
				mStartPoint.top = toGrid(mStartPoint.top);
			}

			mPositionSelectorControl->setVisible(true);
		}

		MyGUI::IntCoord coord = getCoordNewWidget(_point);

		mNewWidget->setCoord(coord);
		mPositionSelectorControl->setCoord(mNewWidget->getAbsoluteCoord());
	}

	void WidgetCreatorManager::finishNewWidget(const MyGUI::IntPoint& _point)
	{
		if (mNewWidget != nullptr)
		{
			MyGUI::IntCoord coord = getCoordNewWidget(_point);

			if (coord.width != 0 && coord.height != 0)
			{
				mNewWidget->setCoord(coord);

				// создали виджет, все счастливы
				WidgetContainer * widgetContainer = new WidgetContainer(mWidgetType, mWidgetSkin, mNewWidget);
				mNewWidget = nullptr;

				EditorWidgets::getInstance().add(widgetContainer);
				UndoManager::getInstance().addValue();

				// чтобы выделился созданый виджет
				resetCreatorInfo();
				WidgetSelectorManager::getInstance().setSelectedWidget(widgetContainer->widget);
			}
			else
			{
				// не удалось создать, т.к. размер нулевой
				resetWidget();
			}
		}

		resetCreatorInfo();
		mPositionSelectorControl->setVisible(false);
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

	/*void WidgetCreatorManager::selectWidget()
	{
		// здесь кликать вглубь
		MyGUI::Widget* item = getTopWidget(getMousePosition());
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
			else
			{
				WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
				mSelectDepth = 0;
			}
		}
		else
		{
			WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
			mSelectDepth = 0;
		}
	}*/

	void WidgetCreatorManager::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	int WidgetCreatorManager::toGrid(int _value)
	{
		if (mGridStep < 1)
			return _value;
		return _value / mGridStep * mGridStep;
	}

	MyGUI::IntCoord WidgetCreatorManager::getCoordNewWidget(const MyGUI::IntPoint& _point)
	{
		MyGUI::IntPoint point = _point;//getMousePosition();
		point.left += mGridStep / 2;
		point.top += mGridStep / 2;

		MyGUI::Widget* parent = mNewWidget->getParent();
		if (parent != nullptr)
			point -= parent->getAbsolutePosition();

		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			point.left = toGrid(point.left);
			point.top = toGrid(point.top);
		}

		MyGUI::IntCoord coord = MyGUI::IntCoord(
			std::min(mStartPoint.left, point.left),
			std::min(mStartPoint.top, point.top),
			abs(point.left - mStartPoint.left),
			abs(point.top - mStartPoint.top));

		return coord;
	}

	/*MyGUI::IntPoint WidgetCreatorManager::getMousePosition()
	{
		MyGUI::IntPoint point = MyGUI::InputManager::getInstance().getMousePosition();
		return point;
	}*/

} // namespace tools
