#include "Precompiled.h"
#include "WorkspaceControl.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "PropertiesPanelView.h"
#include "CommandManager.h"
#include "UndoManager.h"

namespace tools
{

	WorkspaceControl::WorkspaceControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mAreaSelectorControl(nullptr),
		mGridStep(0),
		mCurrentWidget(nullptr),
		mSelectDepth(0)
	{
		SettingsSector* sector = SettingsManager::getInstance().getSector("Workspace");
		MyGUI::IntSize size = sector->getPropertyValue<MyGUI::IntSize>("TextureSize");
		setTextureName(sector->getPropertyValue("TextureName"));
		setTextureRegion(MyGUI::IntCoord(0, 0, size.width, size.height));

		addSelectorControl(mAreaSelectorControl);
		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangePosition);

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &WorkspaceControl::notifySettingsChanged);

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectedWidget);
		notifyChangeSelectedWidget(nullptr);

		PropertiesPanelView::getInstance().eventChangeCoord += MyGUI::newDelegate(this, &WorkspaceControl::notifyPropertyChangeCoord);

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &WorkspaceControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &WorkspaceControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &WorkspaceControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &WorkspaceControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_SizeLeft", MyGUI::newDelegate(this, &WorkspaceControl::CommandSizeLeft));
		CommandManager::getInstance().registerCommand("Command_SizeRight", MyGUI::newDelegate(this, &WorkspaceControl::CommandSizeRight));
		CommandManager::getInstance().registerCommand("Command_SizeTop", MyGUI::newDelegate(this, &WorkspaceControl::CommandSizeTop));
		CommandManager::getInstance().registerCommand("Command_SizeBottom", MyGUI::newDelegate(this, &WorkspaceControl::CommandSizeBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridSizeLeft", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridSizeLeft));
		CommandManager::getInstance().registerCommand("Command_GridSizeRight", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridSizeRight));
		CommandManager::getInstance().registerCommand("Command_GridSizeTop", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridSizeTop));
		CommandManager::getInstance().registerCommand("Command_GridSizeBottom", MyGUI::newDelegate(this, &WorkspaceControl::CommandGridSizeBottom));
	}

	WorkspaceControl::~WorkspaceControl()
	{
		PropertiesPanelView::getInstance().eventChangeCoord -= MyGUI::newDelegate(this, &WorkspaceControl::notifyPropertyChangeCoord);

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectedWidget);

		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &WorkspaceControl::notifySettingsChanged);

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangePosition);
	}

	int WorkspaceControl::toGrid(int _value)
	{
		if (mGridStep < 1)
			return _value;
		return _value / mGridStep * mGridStep;
	}

	void WorkspaceControl::notifyChangePosition()
	{
		mCoordValue = mAreaSelectorControl->getCoord();

		// конвертируем в локальные координаты
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr)
				mCoordValue = mCoordValue - parent->getAbsolutePosition();
		}

		// снапим к гриду
		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			MyGUI::IntCoord coord = mCoordValue;
			MyGUI::IntCoord actionScale = mAreaSelectorControl->getActionScale();

			if (actionScale.left != 0 && actionScale.width != 0)
			{
				int right = coord.right();
				coord.width = toGrid(coord.width + (mGridStep / 2));
				coord.left = right - coord.width;
			}
			else if (actionScale.width != 0)
			{
				int right = toGrid(coord.right() + (mGridStep / 2));
				coord.width = right - coord.left;
			}
			else if (actionScale.left != 0)
			{
				coord.left = toGrid(coord.left + (mGridStep / 2));
			}

			if (actionScale.top != 0 && actionScale.height != 0)
			{
				int bottom = coord.bottom();
				coord.height = toGrid(coord.height + (mGridStep / 2));
				coord.top = bottom - coord.height;
			}
			else if (actionScale.height != 0)
			{
				int bottom = toGrid(coord.bottom() + (mGridStep / 2));
				coord.height = bottom - coord.top;
			}
			else if (actionScale.top != 0)
			{
				coord.top = toGrid(coord.top + (mGridStep / 2));
			}

			if (coord != mCoordValue)
			{
				mCoordValue = coord;
				updateSelectionFromValue();
			}
		}

		PropertiesPanelView::getInstance().setCoord(mCoordValue);
		if (mCurrentWidget != nullptr)
			mCurrentWidget->setCoord(mCoordValue);

		UndoManager::getInstance().addValue(PR_POSITION);
	}

	void WorkspaceControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	void WorkspaceControl::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;
		mSelectDepth = 0;

		if (mCurrentWidget != nullptr)
		{
			mAreaSelectorControl->setVisible(true);
			mCoordValue = mCurrentWidget->getCoord();

			updateSelectionFromValue();
		}
		else
		{
			mAreaSelectorControl->setVisible(false);
		}
	}

	void WorkspaceControl::notifyPropertyChangeCoord(const MyGUI::IntCoord& _coordValue)
	{
		// тут приходит в абсолютных, конвертим в локальные
		mCoordValue = _coordValue;
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr)
				mCoordValue = mCoordValue - parent->getAbsolutePosition();
		}

		updateSelectionFromValue();
	}

	void WorkspaceControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(--mCoordValue.left);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(mCoordValue.left + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(--mCoordValue.top);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(mCoordValue.top + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() - 1) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() + mGridStep) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() - 1) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() + mGridStep) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::updateFromCoordValue()
	{
		// тут работаем с локальными координатами
		PropertiesPanelView::getInstance().setCoord(mCoordValue);
		if (mCurrentWidget != nullptr)
			mCurrentWidget->setCoord(mCoordValue);

		updateSelectionFromValue();

		UndoManager::getInstance().addValue(PR_POSITION);
	}

	bool WorkspaceControl::checkCommand()
	{
		return mMainWidget->getRootKeyFocus() && !mAreaSelectorControl->getCapture();
	}

	void WorkspaceControl::updateSelectionFromValue()
	{
		// саму рамку отображаем в глобальных
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr)
				mAreaSelectorControl->setCoord(mCoordValue + parent->getAbsolutePosition());
			else
				mAreaSelectorControl->setCoord(mCoordValue);
		}
	}

	void WorkspaceControl::selectWidget(const MyGUI::IntPoint& _mousePosition)
	{
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
				//mSelectDepth = 0;
			}
		}
		else
		{
			WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
			//mSelectDepth = 0;
		}
	}

	MyGUI::Widget* WorkspaceControl::getTopWidget(const MyGUI::IntPoint& _point)
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

	bool WorkspaceControl::checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point)
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
