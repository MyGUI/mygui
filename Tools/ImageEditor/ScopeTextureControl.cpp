/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "ScopeTextureControl.h"
#include "CommandManager.h"
#include "Localise.h"
#include "GridManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "FactoryManager.h"
#include "PositionSelectorBlackControl.h"
#include "HorizontalSelectorBlackControl.h"
#include "VerticalSelectorBlackControl.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ScopeTextureControl)

	ScopeTextureControl::ScopeTextureControl() :
		mAreaSelectorControl(nullptr),
		mPositionSelectorControl(nullptr),
		mCurrentSelectorControl(nullptr),
		mActivePositionOnly(false),
		mCoordValueReadOnly(false)
	{
	}

	ScopeTextureControl::~ScopeTextureControl()
	{
		mAreaSelectorControl->eventChangePosition.disconnect(this);
		mPositionSelectorControl->eventChangePosition.disconnect(this);
	}

	void ScopeTextureControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		TextureToolControl::OnInitialise(_parent, _place, _layoutName);

		InitialiseSelectors();

		CommandManager::getInstance().getEvent("Command_MoveLeft")->connect(this, &ScopeTextureControl::CommandMoveLeft);
		CommandManager::getInstance().getEvent("Command_MoveRight")->connect(this, &ScopeTextureControl::CommandMoveRight);
		CommandManager::getInstance().getEvent("Command_MoveTop")->connect(this, &ScopeTextureControl::CommandMoveTop);
		CommandManager::getInstance().getEvent("Command_MoveBottom")->connect(this, &ScopeTextureControl::CommandMoveBottom);
		CommandManager::getInstance().getEvent("Command_SizeLeft")->connect(this, &ScopeTextureControl::CommandSizeLeft);
		CommandManager::getInstance().getEvent("Command_SizeRight")->connect(this, &ScopeTextureControl::CommandSizeRight);
		CommandManager::getInstance().getEvent("Command_SizeTop")->connect(this, &ScopeTextureControl::CommandSizeTop);
		CommandManager::getInstance().getEvent("Command_SizeBottom")->connect(this, &ScopeTextureControl::CommandSizeBottom);
		CommandManager::getInstance().getEvent("Command_GridMoveLeft")->connect(this, &ScopeTextureControl::CommandGridMoveLeft);
		CommandManager::getInstance().getEvent("Command_GridMoveRight")->connect(this, &ScopeTextureControl::CommandGridMoveRight);
		CommandManager::getInstance().getEvent("Command_GridMoveTop")->connect(this, &ScopeTextureControl::CommandGridMoveTop);
		CommandManager::getInstance().getEvent("Command_GridMoveBottom")->connect(this, &ScopeTextureControl::CommandGridMoveBottom);
		CommandManager::getInstance().getEvent("Command_GridSizeLeft")->connect(this, &ScopeTextureControl::CommandGridSizeLeft);
		CommandManager::getInstance().getEvent("Command_GridSizeRight")->connect(this, &ScopeTextureControl::CommandGridSizeRight);
		CommandManager::getInstance().getEvent("Command_GridSizeTop")->connect(this, &ScopeTextureControl::CommandGridSizeTop);
		CommandManager::getInstance().getEvent("Command_GridSizeBottom")->connect(this, &ScopeTextureControl::CommandGridSizeBottom);

		updateCaption();

		setTextureValue("");
		clearCoordValue();
	}

	void ScopeTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		mCoordValue.left = _point.left - (mCoordValue.width / 2);
		mCoordValue.top = _point.top - (mCoordValue.height / 2);

		updateFromCoordValue();
	}

	void ScopeTextureControl::updateFromCoordValue()
	{
		mCurrentSelectorControl->setCoord(mCoordValue);

		setValue(mCoordValue.print());
	}

	void ScopeTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left ++;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top ++;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = GridManager::getInstance().toGrid(mCoordValue.left, GridManager::Previous);
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = GridManager::getInstance().toGrid(mCoordValue.left, GridManager::Next);
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = GridManager::getInstance().toGrid(mCoordValue.top, GridManager::Previous);
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = GridManager::getInstance().toGrid(mCoordValue.top, GridManager::Next);
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Previous) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Next) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Previous) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Next) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (mActivePositionOnly)
			return;

		mCoordValue.height ++;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::notifyChangePosition(SelectorControl* _sender)
	{
		mCoordValue = mCurrentSelectorControl->getCoord();

		// снапим к гриду
		if (!MyGUI::InputManager::getInstance().isShiftPressed())
		{
			MyGUI::IntCoord coord = mCoordValue;
			MyGUI::IntCoord actionScale = mCurrentSelectorControl->getActionScale();

			if (actionScale.left != 0 && actionScale.width != 0)
			{
				int right = coord.right();
				coord.width = GridManager::getInstance().toGrid(coord.width);
				coord.left = right - coord.width;
			}
			else if (actionScale.width != 0)
			{
				int right = GridManager::getInstance().toGrid(coord.right());
				coord.width = right - coord.left;
			}
			else if (actionScale.left != 0)
			{
				coord.left = GridManager::getInstance().toGrid(coord.left);
			}

			if (actionScale.top != 0 && actionScale.height != 0)
			{
				int bottom = coord.bottom();
				coord.height = GridManager::getInstance().toGrid(coord.height);
				coord.top = bottom - coord.height;
			}
			else if (actionScale.height != 0)
			{
				int bottom = GridManager::getInstance().toGrid(coord.bottom());
				coord.height = bottom - coord.top;
			}
			else if (actionScale.top != 0)
			{
				coord.top = GridManager::getInstance().toGrid(coord.top);
			}

			if (coord != mCoordValue)
			{
				mCoordValue = coord;
				mCurrentSelectorControl->setCoord(mCoordValue);
			}
		}

		setValue(mCoordValue.print());
	}

	void ScopeTextureControl::onChangeScale()
	{
		updateCaption();
	}

	void ScopeTextureControl::updateCaption()
	{
		int scale = (int)(getScale() * (double)100);
		addUserTag("CurrentScale", MyGUI::utility::toString(scale));

		CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
	}

	void ScopeTextureControl::setValue(const std::string& _value)
	{
		eventChangeValue(_value);
	}

	void ScopeTextureControl::setCoordValue(const MyGUI::IntCoord& _value)
	{
		mCurrentSelectorControl->setVisible(true);

		if (mCoordValue != _value)
		{
			mCoordValue = _value;
			mCurrentSelectorControl->setCoord(mCoordValue);
		}
	}

	void ScopeTextureControl::clearCoordValue()
	{
		mCurrentSelectorControl->setVisible(false);
	}

	void ScopeTextureControl::clearAll()
	{
		setTextureValue("");
		clearCoordValue();
		clearViewSelectors();
	}

	void ScopeTextureControl::setViewSelectors(const VectorCoord& _selectors)
	{
		clearViewSelectors();

		bool changes = false;
		for (size_t index = 0; index < _selectors.size(); index ++)
		{
			SelectorControl* selector = getFreeSelector(_selectors[index].second, changes);
			selector->setCoord(_selectors[index].first);
		}

		if (changes)
		{
			bool visible = mCurrentSelectorControl->getVisible();
			MyGUI::IntCoord coord = mCoordValue;

			InitialiseSelectors();

			if (visible)
				setCoordValue(coord);
		}
	}

	void ScopeTextureControl::clearViewSelectors()
	{
		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
			mBlackSelectors[index].first->setVisible(false);
	}

	void ScopeTextureControl::setActiveSelector(bool _positionOnly)
	{
		mActivePositionOnly = _positionOnly;

		mAreaSelectorControl->setVisible(false);
		mPositionSelectorControl->setVisible(false);

		if (mActivePositionOnly)
			mCurrentSelectorControl = mPositionSelectorControl;
		else
			mCurrentSelectorControl = mAreaSelectorControl;

		mCoordValue.clear();
	}

	void ScopeTextureControl::InitialiseSelectors()
	{
		ShutdownSelectors();

		addSelectorControl(mAreaSelectorControl);
		addSelectorControl(mPositionSelectorControl);

		mAreaSelectorControl->eventChangePosition.connect(this, &ScopeTextureControl::notifyChangePosition);
		mPositionSelectorControl->eventChangePosition.connect(this, &ScopeTextureControl::notifyChangePosition);

		setActiveSelector(mActivePositionOnly);
	}

	void ScopeTextureControl::ShutdownSelectors()
	{
		if (mAreaSelectorControl != nullptr)
		{
			removeSelectorControl(mAreaSelectorControl);
			mAreaSelectorControl = nullptr;
		}

		if (mPositionSelectorControl != nullptr)
		{
			removeSelectorControl(mPositionSelectorControl);
			mPositionSelectorControl = nullptr;
		}
	}

	void ScopeTextureControl::setCoordValueReadOnly(bool _value)
	{
		mCoordValueReadOnly = _value;
		mAreaSelectorControl->setEnabled(!mCoordValueReadOnly);
		mPositionSelectorControl->setEnabled(!mCoordValueReadOnly);
	}

	SelectorControl* ScopeTextureControl::getFreeSelector(SelectorType _type, bool& _changes)
	{
		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
		{
			if (!mBlackSelectors[index].first->getVisible())
			{
				if (mBlackSelectors[index].second == _type)
				{
					mBlackSelectors[index].first->setVisible(true);
					return mBlackSelectors[index].first;
				}
			}
		}

		_changes = true;

		SelectorControl* control = nullptr;

		if (_type == SelectorPosition)
			control = new PositionSelectorBlackControl();
		else if (_type == SelectorOffsetH)
			control = new HorizontalSelectorBlackControl();
		else if (_type == SelectorOffsetV)
			control = new VerticalSelectorBlackControl();

		addSelectorControl(control);

		mBlackSelectors.push_back(std::make_pair(control, _type));

		return control;
	}

}
