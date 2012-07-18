/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "SkinTextureControl.h"
#include "SkinManager.h"
#include "CommandManager.h"
#include "Localise.h"
#include "GridManager.h"

// FIXME времено включен
namespace tools
{

	SkinTextureControl::SkinTextureControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mAreaSelectorControl(nullptr)
	{
		mTypeName = MyGUI::utility::toString((size_t)this);

		addSelectorControl(mAreaSelectorControl);

		mAreaSelectorControl->eventChangePosition.connect(this, &SkinTextureControl::notifyChangePosition);

		CommandManager::getInstance().registerCommand("Command_MoveLeft", MyGUI::newDelegate(this, &SkinTextureControl::CommandMoveLeft));
		CommandManager::getInstance().registerCommand("Command_MoveRight", MyGUI::newDelegate(this, &SkinTextureControl::CommandMoveRight));
		CommandManager::getInstance().registerCommand("Command_MoveTop", MyGUI::newDelegate(this, &SkinTextureControl::CommandMoveTop));
		CommandManager::getInstance().registerCommand("Command_MoveBottom", MyGUI::newDelegate(this, &SkinTextureControl::CommandMoveBottom));
		CommandManager::getInstance().registerCommand("Command_SizeLeft", MyGUI::newDelegate(this, &SkinTextureControl::CommandSizeLeft));
		CommandManager::getInstance().registerCommand("Command_SizeRight", MyGUI::newDelegate(this, &SkinTextureControl::CommandSizeRight));
		CommandManager::getInstance().registerCommand("Command_SizeTop", MyGUI::newDelegate(this, &SkinTextureControl::CommandSizeTop));
		CommandManager::getInstance().registerCommand("Command_SizeBottom", MyGUI::newDelegate(this, &SkinTextureControl::CommandSizeBottom));
		CommandManager::getInstance().registerCommand("Command_GridMoveLeft", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridMoveLeft));
		CommandManager::getInstance().registerCommand("Command_GridMoveRight", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridMoveRight));
		CommandManager::getInstance().registerCommand("Command_GridMoveTop", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridMoveTop));
		CommandManager::getInstance().registerCommand("Command_GridMoveBottom", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridMoveBottom));
		CommandManager::getInstance().registerCommand("Command_GridSizeLeft", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridSizeLeft));
		CommandManager::getInstance().registerCommand("Command_GridSizeRight", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridSizeRight));
		CommandManager::getInstance().registerCommand("Command_GridSizeTop", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridSizeTop));
		CommandManager::getInstance().registerCommand("Command_GridSizeBottom", MyGUI::newDelegate(this, &SkinTextureControl::CommandGridSizeBottom));

		updateCaption();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		mAreaSelectorControl->eventChangePosition.disconnect(this);
	}

	/*void SkinTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			else if (_sender->getName() == "Coord")
				updateCoord();
		}
	}*/

	void SkinTextureControl::updateSkinProperties()
	{
		updateTexture();
		updateCoord();
	}

	void SkinTextureControl::updateTexture()
	{
		MyGUI::UString texture;

		//if (getCurrentSkin() != nullptr)
			//texture = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		setTextureName(texture);
	}

	void SkinTextureControl::updateCoord()
	{
		MyGUI::UString value;

		//if (getCurrentSkin() != nullptr)
			//value = getCurrentSkin()->getPropertySet()->getPropertyValue("Coord");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mAreaSelectorControl->setVisible(true);

			mCoordValue = coord;
			mAreaSelectorControl->setCoord(mCoordValue);
		}
		else
		{
			mAreaSelectorControl->setVisible(false);
		}
	}

	void SkinTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		mCoordValue.left = _point.left - (mCoordValue.width / 2);
		mCoordValue.top = _point.top - (mCoordValue.height / 2);

		updateFromCoordValue();
	}

	void SkinTextureControl::updateFromCoordValue()
	{
		mAreaSelectorControl->setCoord(mCoordValue);

		//if (getCurrentSkin() != nullptr)
			//getCurrentSkin()->getPropertySet()->setPropertyValue("Coord", mCoordValue.print(), mTypeName);
	}

	void SkinTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left --;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left ++;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top --;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top ++;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = GridManager::getInstance().toGrid(mCoordValue.left, GridManager::Previous);
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = GridManager::getInstance().toGrid(mCoordValue.left, GridManager::Next);
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = GridManager::getInstance().toGrid(mCoordValue.top, GridManager::Previous);
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = GridManager::getInstance().toGrid(mCoordValue.top, GridManager::Next);
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Previous) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = GridManager::getInstance().toGrid(mCoordValue.right(), GridManager::Next) - mCoordValue.left;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Previous) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = GridManager::getInstance().toGrid(mCoordValue.bottom(), GridManager::Next) - mCoordValue.top;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width --;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandSizeRight(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.width ++;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandSizeTop(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height --;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		mCoordValue.height ++;
		updateFromCoordValue();

		_result = true;
	}

	void SkinTextureControl::notifyChangePosition(SelectorControl* _sender)
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
				mAreaSelectorControl->setCoord(mCoordValue);
			}
		}

		//if (getCurrentSkin() != nullptr)
			//getCurrentSkin()->getPropertySet()->setPropertyValue("Coord", mCoordValue.print(), mTypeName);
	}

	void SkinTextureControl::onChangeScale()
	{
		updateCaption();
	}

	void SkinTextureControl::updateCaption()
	{
		if (getActivate())
		{
			int scale = (int)(getScale() * (double)100);
			addUserTag("CurrentScale", MyGUI::utility::toString(scale));

			CommandManager::getInstance().executeCommand("Command_UpdateAppCaption");
		}
	}

	void SkinTextureControl::onChangeActivate()
	{
		if (getActivate())
			updateCaption();
	}

}
