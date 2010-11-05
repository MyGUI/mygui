/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "RegionTextureControl.h"
#include "CommandManager.h"
#include "SettingsManager.h"
#include "Localise.h"

namespace tools
{
	RegionTextureControl::RegionTextureControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mTextureVisible(false),
		mAreaSelectorControl(nullptr),
		mPositionSelectorControl(nullptr),
		mGridStep(0)
	{
		mTypeName = MyGUI::utility::toString((size_t)this);

		// сразу рисуем рамки для стейтов
		std::vector<MyGUI::IntCoord> coords(10);
		drawUnselectedStates(coords);

		addSelectorControl(mAreaSelectorControl);
		addSelectorControl(mPositionSelectorControl);

		mPositionSelectorControl->setEnabled(false);

		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &RegionTextureControl::notifyChangePosition);

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &RegionTextureControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &RegionTextureControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &RegionTextureControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &RegionTextureControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_SizeLeft", MyGUI::newDelegate(this, &RegionTextureControl::CommandSizeLeft));
		CommandManager::getInstance().registerCommand("Command_SizeRight", MyGUI::newDelegate(this, &RegionTextureControl::CommandSizeRight));
		CommandManager::getInstance().registerCommand("Command_SizeTop", MyGUI::newDelegate(this, &RegionTextureControl::CommandSizeTop));
		CommandManager::getInstance().registerCommand("Command_SizeBottom", MyGUI::newDelegate(this, &RegionTextureControl::CommandSizeBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridSizeLeft", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridSizeLeft));
		CommandManager::getInstance().registerCommand("Command_GridSizeRight", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridSizeRight));
		CommandManager::getInstance().registerCommand("Command_GridSizeTop", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridSizeTop));
		CommandManager::getInstance().registerCommand("Command_GridSizeBottom", MyGUI::newDelegate(this, &RegionTextureControl::CommandGridSizeBottom));

		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &RegionTextureControl::notifySettingsChanged);

		initialiseAdvisor();

		updateCaption();
	}

	RegionTextureControl::~RegionTextureControl()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &RegionTextureControl::notifySettingsChanged);

		shutdownAdvisor();

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &RegionTextureControl::notifyChangePosition);
	}

	void RegionTextureControl::updateRegionProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Position")
				updatePosition();
			else if (_sender->getName() == "Visible")
				updateVisible();
			else if (_sender->getName() == "Enabled")
				updateVisible();
		}
	}

	void RegionTextureControl::updateRegionProperties()
	{
		updateVisible();
		updatePosition();

		updateUnselectedStates();
	}

	void RegionTextureControl::updateTextureControl()
	{
		if (mTextureVisible && !mTextureName.empty() && getCurrentSkin() != nullptr && getCurrentState() != nullptr)
		{
			setTextureName(mTextureName);
			setTextureRegion(mTextureRegion);
		}
		else
		{
			setTextureRegion(MyGUI::IntCoord());
		}
	}

	void RegionTextureControl::updateSkinProperties()
	{
		updateTextureName();
		updateRegionSize();
	}

	void RegionTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Texture")
			updateTextureName();
		else if (_sender->getName() == "Coord")
			updateRegionSize();
	}

	void RegionTextureControl::updateStateProperties()
	{
		updateRegionPosition();
		updateTextureVisible();
	}

	void RegionTextureControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Position")
			updateRegionPosition();
		else if (_sender->getName() == "Visible")
			updateTextureVisible();
	}

	void RegionTextureControl::updateTextureName()
	{
		mTextureName = "";
		if (getCurrentSkin() != nullptr)
			mTextureName = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		updateTextureControl();
	}

	void RegionTextureControl::updateRegionSize()
	{
		mTextureRegion.width = 0;
		mTextureRegion.height = 0;

		if (getCurrentSkin() != nullptr)
		{
			MyGUI::IntCoord coord = MyGUI::IntCoord::parse(getCurrentSkin()->getPropertySet()->getPropertyValue("Coord"));
			mTextureRegion.width = coord.width;
			mTextureRegion.height = coord.height;
		}

		updateTextureControl();

		updateUnselectedStates();
	}

	void RegionTextureControl::updateRegionPosition()
	{
		mTextureRegion.left = 0;
		mTextureRegion.top = 0;

		if (getCurrentState() != nullptr)
		{
			MyGUI::IntPoint position = MyGUI::IntPoint::parse(getCurrentState()->getPropertySet()->getPropertyValue("Position"));
			mTextureRegion.left = position.left;
			mTextureRegion.top = position.top;
		}

		updateTextureControl();
	}

	void RegionTextureControl::updateTextureVisible()
	{
		mTextureVisible = false;

		if (getCurrentState() != nullptr)
			mTextureVisible = getCurrentState()->getPropertySet()->getPropertyValue("Visible") == "True";

		updateTextureControl();
	}

	void RegionTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentRegion() != nullptr)
			value = getCurrentRegion()->getPropertySet()->getPropertyValue("Position");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mCoordValue = coord;
			mAreaSelectorControl->setCoord(mCoordValue);
			mPositionSelectorControl->setCoord(coord);
		}
	}

	void RegionTextureControl::updateVisible()
	{
		mAreaSelectorControl->setVisible(false);
		mPositionSelectorControl->setVisible(false);

		if (getCurrentRegion() != nullptr)
		{
			if (getCurrentRegion()->getPropertySet()->getPropertyValue("Visible") == "True")
			{
				if (getCurrentRegion()->getPropertySet()->getPropertyValue("Enabled") == "True")
				{
					if (!getCurrentRegion()->getPropertySet()->getPropertyReadOnly("Position"))
						mAreaSelectorControl->setVisible(true);
					else
						mPositionSelectorControl->setVisible(true);
				}
			}
		}
	}

	void RegionTextureControl::notifyChangePosition()
	{
		mCoordValue = mAreaSelectorControl->getCoord();

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
				mAreaSelectorControl->setCoord(mCoordValue);
			}
		}

		if (getCurrentRegion() != nullptr)
			getCurrentRegion()->getPropertySet()->setPropertyValue("Position", mCoordValue.print(), mTypeName);
	}

	void RegionTextureControl::updateUnselectedStates()
	{
		std::vector<MyGUI::IntCoord> coords;

		if (getCurrentSkin() != nullptr)
		{
			ItemHolder<RegionItem>::EnumeratorItem regions = getCurrentSkin()->getRegions().getChildsEnumerator();
			while (regions.next())
			{
				RegionItem* item = regions.current();
				if (item != getCurrentSkin()->getRegions().getItemSelected())
				{
					if (item->getPropertySet()->getPropertyValue("Visible") == "True")
					{
						if (item->getPropertySet()->getPropertyValue("Enabled") == "True")
						{
							addCoord(coords, item->getPropertySet()->getPropertyValue("Position"));
						}
					}
				}
			}
		}

		drawUnselectedStates(coords);
	}

	void RegionTextureControl::addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_coord, coord.left, coord.top, coord.width, coord.height))
		{
			for (std::vector<MyGUI::IntCoord>::iterator item = _coords.begin(); item != _coords.end(); ++item)
			{
				if ((*item) == coord)
					return;
			}

			_coords.push_back(coord);
		}
	}

	void RegionTextureControl::drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords)
	{
		while (_coords.size() > mBlackSelectors.size())
		{
			PositionSelectorBlackControl* selector = nullptr;
			addSelectorControl(selector);

			mBlackSelectors.push_back(selector);
		}

		for (size_t index = 0; index < mBlackSelectors.size(); ++index)
		{
			if (index < _coords.size())
			{
				mBlackSelectors[index]->setVisible(true);
				mBlackSelectors[index]->setCoord(_coords[index]);
			}
			else
			{
				mBlackSelectors[index]->setVisible(false);
			}
		}
	}

	void RegionTextureControl::updateSeparatorProperties()
	{
		updateUnselectedStates();
	}

	void RegionTextureControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Visible" || _sender->getName() == "Offset")
			updateUnselectedStates();
	}

	void RegionTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		mCoordValue.left = _point.left - (mCoordValue.width / 2);
		mCoordValue.top = _point.top - (mCoordValue.height / 2);

		updateFromCoordValue();
	}

	void RegionTextureControl::updateFromCoordValue()
	{
		mAreaSelectorControl->setCoord(mCoordValue);

		if (getCurrentRegion() != nullptr)
		{
			if (getCurrentRegion()->getPropertySet()->getPropertyValue("Visible") == "True")
			{
				if (getCurrentRegion()->getPropertySet()->getPropertyValue("Enabled") == "True")
				{
					if (!getCurrentRegion()->getPropertySet()->getPropertyReadOnly("Position"))
						getCurrentRegion()->getPropertySet()->setPropertyValue("Position", mCoordValue.print(), mTypeName);
				}
			}
		}
	}

	int RegionTextureControl::toGrid(int _value)
	{
		if (mGridStep < 1)
			return _value;
		return _value / mGridStep * mGridStep;
	}

	void RegionTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left --;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left ++;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top --;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top ++;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(--mCoordValue.left);
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(mCoordValue.left + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(--mCoordValue.top);
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(mCoordValue.top + mGridStep);
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() - 1) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() + mGridStep) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() - 1) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() + mGridStep) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height ++;
		updateFromCoordValue();

		_result = true;
	}

	void RegionTextureControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	void RegionTextureControl::onChangeScale()
	{
		updateCaption();
	}

	void RegionTextureControl::updateCaption()
	{
		if (getActivate())
		{
			int scale = (int)(getScale() * (double)100);
			addUserTag("CurrentScale", MyGUI::utility::toString(scale));

			CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
		}
	}

	void RegionTextureControl::onChangeActivate()
	{
		if (getActivate())
			updateCaption();
	}

} // namespace tools
