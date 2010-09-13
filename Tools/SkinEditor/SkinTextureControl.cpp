/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinTextureControl.h"
#include "SkinManager.h"
#include "CommandManager.h"

namespace tools
{
	SkinTextureControl::SkinTextureControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent),
		mAreaSelectorControl(nullptr),
		mGridStep(10)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		addSelectorControl(mAreaSelectorControl);

		mAreaSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);

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

		initialiseAdvisor();
	}

	SkinTextureControl::~SkinTextureControl()
	{
		shutdownAdvisor();

		mAreaSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SkinTextureControl::notifyChangePosition);
	}

	void SkinTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Texture")
				updateTexture();
			else if (_sender->getName() == "Coord")
				updateCoord();
		}
	}

	void SkinTextureControl::updateSkinProperties()
	{
		updateTexture();
		updateCoord();
	}

	void SkinTextureControl::updateTexture()
	{
		MyGUI::UString texture;

		if (getCurrentSkin() != nullptr)
			texture = getCurrentSkin()->getPropertySet()->getPropertyValue("Texture");

		setTextureName(texture);
	}

	void SkinTextureControl::updateCoord()
	{
		MyGUI::UString value;

		if (getCurrentSkin() != nullptr)
			value = getCurrentSkin()->getPropertySet()->getPropertyValue("Coord");

		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(value, coord.left, coord.top, coord.width, coord.height))
		{
			mAreaSelectorControl->setVisible(true);

			mCoordValue = coord;
			updateRegionCoord();
		}
		else
		{
			mAreaSelectorControl->setVisible(false);
		}
	}

	void SkinTextureControl::updateRegionCoord()
	{
		mAreaSelectorControl->setCoord(mCoordValue);
	}

	void SkinTextureControl::notifyChangePosition()
	{
		mCoordValue = mAreaSelectorControl->getCoord();

		if (getCurrentSkin() != nullptr)
			getCurrentSkin()->getPropertySet()->setPropertyValue("Coord", mCoordValue.print(), mTypeName);
	}

	void SkinTextureControl::onMouseButtonClick(const MyGUI::IntPoint& _point)
	{
		mCoordValue.left = _point.left - (mCoordValue.width / 2);
		mCoordValue.top = _point.top - (mCoordValue.height / 2);

		updateFromCoordValue();
	}

	bool SkinTextureControl::checkCommand()
	{
		return mMainWidget->getRootKeyFocus() && !mAreaSelectorControl->getCapture();
	}

	void SkinTextureControl::updateFromCoordValue()
	{
		mAreaSelectorControl->setCoord(mCoordValue);

		if (getCurrentSkin() != nullptr)
			getCurrentSkin()->getPropertySet()->setPropertyValue("Coord", mCoordValue.print(), mTypeName);
	}

	int SkinTextureControl::toGrid(int _value)
	{
		return _value / mGridStep * mGridStep;
	}

	void SkinTextureControl::CommandMoveLeft(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.left --;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandMoveRight(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.left ++;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandMoveTop(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.top --;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandMoveBottom(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.top ++;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandSizeLeft(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.width --;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandSizeRight(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.width ++;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandSizeTop(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.height --;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandSizeBottom(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.height ++;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridMoveLeft(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(--mCoordValue.left);
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridMoveRight(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.left = toGrid(mCoordValue.left + mGridStep);
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridMoveTop(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(--mCoordValue.top);
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridMoveBottom(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.top = toGrid(mCoordValue.top + mGridStep);
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridSizeLeft(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() - 1) - mCoordValue.left;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridSizeRight(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.width = toGrid(mCoordValue.right() + mGridStep) - mCoordValue.left;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridSizeTop(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() - 1) - mCoordValue.top;
		updateFromCoordValue();
	}

	void SkinTextureControl::CommandGridSizeBottom(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		mCoordValue.height = toGrid(mCoordValue.bottom() + mGridStep) - mCoordValue.top;
		updateFromCoordValue();
	}

} // namespace tools
