#include "Precompiled.h"
#include "WorkspaceControl.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "WidgetCreatorManager.h"
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
		mMoveableWidget(false)
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

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &WorkspaceControl::Command_MoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &WorkspaceControl::Command_MoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &WorkspaceControl::Command_MoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &WorkspaceControl::Command_MoveBottom));
		CommandManager::getInstance().registerCommand("Command_SizeLeft", MyGUI::newDelegate(this, &WorkspaceControl::Command_SizeLeft));
		CommandManager::getInstance().registerCommand("Command_SizeRight", MyGUI::newDelegate(this, &WorkspaceControl::Command_SizeRight));
		CommandManager::getInstance().registerCommand("Command_SizeTop", MyGUI::newDelegate(this, &WorkspaceControl::Command_SizeTop));
		CommandManager::getInstance().registerCommand("Command_SizeBottom", MyGUI::newDelegate(this, &WorkspaceControl::Command_SizeBottom));

		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridSizeLeft", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridSizeLeft));
		CommandManager::getInstance().registerCommand("Command_GridSizeRight", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridSizeRight));
		CommandManager::getInstance().registerCommand("Command_GridSizeTop", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridSizeTop));
		CommandManager::getInstance().registerCommand("Command_GridSizeBottom", MyGUI::newDelegate(this, &WorkspaceControl::Command_GridSizeBottom));

		CommandManager::getInstance().registerCommand("Command_Delete", MyGUI::newDelegate(this, &WorkspaceControl::Command_Delete));
		CommandManager::getInstance().registerCommand("Command_NextItem", MyGUI::newDelegate(this, &WorkspaceControl::Command_NextItem));

		WidgetCreatorManager::getInstance().eventChangeCreatorMode += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeCreatorMode);
	}

	WorkspaceControl::~WorkspaceControl()
	{
		WidgetCreatorManager::getInstance().eventChangeCreatorMode -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeCreatorMode);
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
		MyGUI::IntCoord coord = mAreaSelectorControl->getCoord();

		if (!mMoveableWidget)
			return;

		mCoordValue = coord;

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

		if (mCurrentWidget != nullptr)
		{
			mAreaSelectorControl->setVisible(true);
			mCoordValue = mCurrentWidget->getCoord();

			WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
			mMoveableWidget = widgetType->resizeable;

			updateSelectorEnabled();
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

	void WorkspaceControl::Command_MoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_MoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_MoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_MoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left = toGrid(--mCoordValue.left);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left = toGrid(mCoordValue.left + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top = toGrid(--mCoordValue.top);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top = toGrid(mCoordValue.top + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width = toGrid(mCoordValue.right() - 1) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width = toGrid(mCoordValue.right() + mGridStep) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() - 1) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() + mGridStep) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_SizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_SizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_SizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void WorkspaceControl::Command_SizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
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

	void WorkspaceControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		WidgetSelectorManager::getInstance().selectWidget(_point);
	}

	void WorkspaceControl::onMouseMove()
	{
		WidgetSelectorManager::getInstance().resetDepth();
	}

	void WorkspaceControl::Command_Delete(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mCurrentWidget == nullptr)
			return;

		EditorWidgets::getInstance().remove(mCurrentWidget);
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
		UndoManager::getInstance().addValue();

		_result = true;
	}

	void WorkspaceControl::Command_NextItem(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		MyGUI::Tab* tab = nullptr;

		if (mCurrentWidget != nullptr)
		{
			if (mCurrentWidget->isType<MyGUI::Tab>())
				tab = mCurrentWidget->castType<MyGUI::Tab>();
			else if (mCurrentWidget->getParent() != nullptr && mCurrentWidget->getParent()->isType<MyGUI::Tab>())
				tab = mCurrentWidget->getParent()->castType<MyGUI::Tab>();
		}

		if (tab == nullptr)
			return;

		if (tab->getItemCount() == 0)
			return;

		size_t sheet = tab->getIndexSelected();
		sheet++;
		if (sheet >= tab->getItemCount())
			sheet = 0;
		if (tab->getItemCount())
			tab->setIndexSelected(sheet);

		WidgetSelectorManager::getInstance().setSelectedWidget(tab->getItemSelected());

		_result = true;
	}

	void WorkspaceControl::notifyChangeCreatorMode(bool _createMode)
	{
		updateSelectorEnabled();
	}

	void WorkspaceControl::onMouseButtonPressed(const MyGUI::IntPoint& _point)
	{
		if (WidgetCreatorManager::getInstance().getCreateMode())
			WidgetCreatorManager::getInstance().createNewWidget(_point);
	}

	void WorkspaceControl::onMouseButtonReleased(const MyGUI::IntPoint& _point)
	{
		if (WidgetCreatorManager::getInstance().getCreateMode())
			WidgetCreatorManager::getInstance().finishNewWidget(_point);
	}

	void WorkspaceControl::onMouseDrag(const MyGUI::IntPoint& _point)
	{
		if (WidgetCreatorManager::getInstance().getCreateMode())
			WidgetCreatorManager::getInstance().moveNewWidget(_point);
	}

	void WorkspaceControl::updateSelectorEnabled()
	{
		mAreaSelectorControl->setEnabled(!WidgetCreatorManager::getInstance().getCreateMode() && mMoveableWidget);
	}

} // namespace tools
