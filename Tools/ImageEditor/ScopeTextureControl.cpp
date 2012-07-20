/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "ScopeTextureControl.h"
#include "SkinManager.h"
#include "CommandManager.h"
#include "Localise.h"
#include "GridManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ScopeTextureControl)

	ScopeTextureControl::ScopeTextureControl() :
		mAreaSelectorControl(nullptr),
		mPositionSelectorControl(nullptr),
		mCurrentSelectorControl(nullptr),
		mActivePositionOnly(false)
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

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &ScopeTextureControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &ScopeTextureControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &ScopeTextureControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &ScopeTextureControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_SizeLeft", MyGUI::newDelegate(this, &ScopeTextureControl::CommandSizeLeft));
		CommandManager::getInstance().registerCommand("Command_SizeRight", MyGUI::newDelegate(this, &ScopeTextureControl::CommandSizeRight));
		CommandManager::getInstance().registerCommand("Command_SizeTop", MyGUI::newDelegate(this, &ScopeTextureControl::CommandSizeTop));
		CommandManager::getInstance().registerCommand("Command_SizeBottom", MyGUI::newDelegate(this, &ScopeTextureControl::CommandSizeBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridSizeLeft", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridSizeLeft));
		CommandManager::getInstance().registerCommand("Command_GridSizeRight", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridSizeRight));
		CommandManager::getInstance().registerCommand("Command_GridSizeTop", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridSizeTop));
		CommandManager::getInstance().registerCommand("Command_GridSizeBottom", MyGUI::newDelegate(this, &ScopeTextureControl::CommandGridSizeBottom));

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

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Previous) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Next) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Previous) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Next) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void ScopeTextureControl::CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
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
		bool changes = false;

		clearViewSelectors();

		while (_selectors.size() > mBlackSelectors.size())
		{
			PositionSelectorBlackControl* selector = nullptr;
			addSelectorControl(selector);

			mBlackSelectors.push_back(selector);

			changes = true;
		}

		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
		{
			if (index < _selectors.size())
			{
				mBlackSelectors[index]->setVisible(true);
				mBlackSelectors[index]->setCoord(_selectors[index]);
			}
			else
			{
				mBlackSelectors[index]->setVisible(false);
			}
		}

		if (changes)
			InitialiseSelectors();
	}

	void ScopeTextureControl::clearViewSelectors()
	{
		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
			mBlackSelectors[index]->setVisible(false);
	}

	void ScopeTextureControl::setActiveSelector(bool _positionOnly)
	{
		mActivePositionOnly = _positionOnly;

		if (mActivePositionOnly)
		{
			mCurrentSelectorControl = mPositionSelectorControl;
			mAreaSelectorControl->setVisible(false);
		}
		else
		{
			mCurrentSelectorControl = mAreaSelectorControl;
			mPositionSelectorControl->setVisible(false);
		}

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
}
