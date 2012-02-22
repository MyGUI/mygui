#include "Precompiled.h"
#include "WorkspaceControl.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "WidgetCreatorManager.h"
#include "CommandManager.h"
#include "UndoManager.h"
#include "Localise.h"
#include "MyGUI_RTTLayer.h"
#include "Grid.h"

namespace tools
{

	WorkspaceControl::WorkspaceControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mAreaSelectorControl(nullptr),
		mCurrentWidget(nullptr),
		mMoveableWidget(false),
		mPositionSelectorCreatorControl(nullptr),
		mFreeChildMode(false)
	{
		SettingsSector* sector = SettingsManager::getInstance().getSector("Workspace");
		MyGUI::IntSize size = sector->getPropertyValue<MyGUI::IntSize>("TextureSize");

		setRttLayerSize(size);

		setTextureName(sector->getPropertyValue("TextureName"));
		setTextureRegion(MyGUI::IntCoord(0, 0, size.width, size.height));

		addSelectorControl(mAreaSelectorControl);
		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangePosition);

		addSelectorControl(mPositionSelectorCreatorControl);
		mPositionSelectorCreatorControl->setEnabled(false);
		mPositionSelectorCreatorControl->setVisible(false);

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectedWidget);
		notifyChangeSelectedWidget(nullptr);

		EditorWidgets::getInstance().eventChangeWidgetCoord += MyGUI::newDelegate(this, &WorkspaceControl::notifyPropertyChangeCoord);

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

		CommandManager::getInstance().registerCommand("Command_FreeChildMode", MyGUI::newDelegate(this, &WorkspaceControl::Command_FreeChildMode));

		WidgetCreatorManager::getInstance().eventChangeCreatorMode += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeCreatorMode);
		WidgetCreatorManager::getInstance().eventChangeSelector += MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectorCreator);

		updateCaption();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &WorkspaceControl::notifyFrameStart);
	}

	WorkspaceControl::~WorkspaceControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &WorkspaceControl::notifyFrameStart);

		WidgetCreatorManager::getInstance().eventChangeSelector -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectorCreator);
		WidgetCreatorManager::getInstance().eventChangeCreatorMode -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeCreatorMode);
		EditorWidgets::getInstance().eventChangeWidgetCoord -= MyGUI::newDelegate(this, &WorkspaceControl::notifyPropertyChangeCoord);
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangeSelectedWidget);

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &WorkspaceControl::notifyChangePosition);
	}

	void WorkspaceControl::notifyChangePosition()
	{
		if (!mMoveableWidget)
			return;

		mCoordValue = mAreaSelectorControl->getCoord();

		// конвертируем в локальные координаты
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr && !mCurrentWidget->isRootWidget())
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
				coord.left = Grid::getInstance().toGrid(coord.left);
				coord.width = right - coord.left;
			}
			else if (actionScale.width != 0)
			{
				int right = Grid::getInstance().toGrid(coord.right());
				coord.width = right - coord.left;
			}
			else if (actionScale.left != 0)
			{
				coord.left = Grid::getInstance().toGrid(coord.left);
			}

			if (actionScale.top != 0 && actionScale.height != 0)
			{
				int bottom = coord.bottom();
				coord.top = Grid::getInstance().toGrid(coord.top);
				coord.height = bottom - coord.top;
			}
			else if (actionScale.height != 0)
			{
				int bottom = Grid::getInstance().toGrid(coord.bottom());
				coord.height = bottom - coord.top;
			}
			else if (actionScale.top != 0)
			{
				coord.top = Grid::getInstance().toGrid(coord.top);
			}

			if (coord != mCoordValue)
			{
				mCoordValue = coord;
				updateSelectionFromValue();
			}
		}

		if (mCurrentWidget != nullptr)
			setWidgetCoord(mCurrentWidget, mCoordValue);

		UndoManager::getInstance().addValue(PR_POSITION);
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

			// FIXME возвращаем себе кей фокус
			if (!mMainWidget->getRootKeyFocus())
				MyGUI::InputManager::getInstance().setKeyFocusWidget(mMainWidget);
		}
		else
		{
			mAreaSelectorControl->setVisible(false);
		}
	}

	void WorkspaceControl::notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner)
	{
		if (_owner == "WorkspaceControl" || _widget != mCurrentWidget)
			return;

		// тут приходит в абсолютных, конвертим в локальные
		mCoordValue = _coordValue;
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr && !mCurrentWidget->isRootWidget())
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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left = Grid::getInstance().toGrid(mCoordValue.left, Grid::Previous);
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.left = Grid::getInstance().toGrid(mCoordValue.left, Grid::Next);
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top = Grid::getInstance().toGrid(mCoordValue.top, Grid::Previous);
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.top = Grid::getInstance().toGrid(mCoordValue.top, Grid::Next);
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width = Grid::getInstance().toGrid(mCoordValue.right(), Grid::Previous) - mCoordValue.left;
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.width = Grid::getInstance().toGrid(mCoordValue.right(), Grid::Next) - mCoordValue.left;
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.height = Grid::getInstance().toGrid(mCoordValue.bottom(), Grid::Previous) - mCoordValue.top;
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::Command_GridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!mMoveableWidget)
			return;

		mCoordValue.height = Grid::getInstance().toGrid(mCoordValue.bottom(), Grid::Next) - mCoordValue.top;
		updateFromCoordValue();

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

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

		UndoManager::getInstance().addValue(PR_KEY_POSITION);

		_result = true;
	}

	void WorkspaceControl::updateFromCoordValue()
	{
		// тут работаем с локальными координатами
		if (mCurrentWidget != nullptr)
			setWidgetCoord(mCurrentWidget, mCoordValue);

		updateSelectionFromValue();
	}

	void WorkspaceControl::updateSelectionFromValue()
	{
		// саму рамку отображаем в глобальных
		if (mCurrentWidget != nullptr)
		{
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (parent != nullptr && !mCurrentWidget->isRootWidget())
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

		MyGUI::TabControl* tab = nullptr;

		if (mCurrentWidget != nullptr)
		{
			if (mCurrentWidget->isType<MyGUI::TabControl>())
				tab = mCurrentWidget->castType<MyGUI::TabControl>();
			else if (mCurrentWidget->getParent() != nullptr && mCurrentWidget->getParent()->isType<MyGUI::TabControl>())
				tab = mCurrentWidget->getParent()->castType<MyGUI::TabControl>();
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

	void WorkspaceControl::Command_FreeChildMode(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mFreeChildMode = !mFreeChildMode;

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
		UndoManager::getInstance().dropLastProperty();

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

	void WorkspaceControl::notifyChangeSelectorCreator(bool _visible, const MyGUI::IntCoord& _coord)
	{
		if (_visible)
		{
			mPositionSelectorCreatorControl->setVisible(true);
			mPositionSelectorCreatorControl->setCoord(_coord);
		}
		else
		{
			mPositionSelectorCreatorControl->setVisible(false);
		}
	}

	void WorkspaceControl::onChangeScale()
	{
		updateCaption();
	}

	void WorkspaceControl::updateCaption()
	{
		if (getActivate())
		{
			int scale = (int)(getScale() * (double)100);
			addUserTag("CurrentScale", MyGUI::utility::toString(scale));

			CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
		}
	}

	void WorkspaceControl::notifyFrameStart(float _time)
	{
		if (getSelectorsCapture())
			return;
		if (mCurrentWidget == nullptr)
			return;

		if (mAreaSelectorControl->getCoord() != mCurrentWidget->getAbsoluteCoord())
		{
			mCoordValue = mCurrentWidget->getCoord();
			updateSelectionFromValue();
		}
	}

	void WorkspaceControl::setWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coord)
	{
		if (mFreeChildMode)
		{
			typedef std::pair<MyGUI::Widget*, MyGUI::IntCoord> PairWidgetCoord;
			typedef std::vector<PairWidgetCoord> VectorPairWidgetCoord;
			VectorPairWidgetCoord coords;

			// запоминаем позиции детей
			for (size_t index = 0; index < _widget->getChildCount(); ++ index)
			{
				MyGUI::Widget* child = _widget->getChildAt(index);
				if (!child->isRootWidget())
					coords.push_back(PairWidgetCoord(child, child->getCoord()));
			}

			// на сколько сдвинут виджет
			MyGUI::IntCoord coordDiff = _coord - _widget->getCoord();
			_widget->setCoord(_coord);

			// восттанавливаем обсолютное положение детей
			for (VectorPairWidgetCoord::iterator item = coords.begin(); item != coords.end(); ++ item)
			{
				WidgetContainer* widgetContainer = EditorWidgets::getInstance().find((*item).first);
				if (widgetContainer != nullptr)
				{
					MyGUI::IntCoord coord = (*item).second;
					(*item).first->setCoord(coord.left - coordDiff.left, coord.top - coordDiff.top, coord.width, coord.height);
				}
			}
		}
		else
		{
			_widget->setCoord(_coord);
		}

		EditorWidgets::getInstance().onSetWidgetCoord(_widget, _coord, "WorkspaceControl");
	}

	void WorkspaceControl::setRttLayerSize(const MyGUI::IntSize& _size)
	{
		MyGUI::ILayer* layer = MyGUI::LayerManager::getInstance().getByName("LayoutEditor_Widgets", false);
		if (layer != nullptr)
		{
			MyGUI::RTTLayer* rttLayer = layer->castType<MyGUI::RTTLayer>();
			rttLayer->setTextureSize(_size);
		}
	}

} // namespace tools
