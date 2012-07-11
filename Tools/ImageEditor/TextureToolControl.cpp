/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "TextureToolControl.h"
#include "Localise.h"
#include "SettingsManager.h"
#include "CommandManager.h"

namespace tools
{

	TextureToolControl::TextureToolControl(MyGUI::Widget* _parent) :
		TextureControl("TextureControl.layout", _parent),
		mCurrentScaleValue(100),
		mActivate(true)
	{
		MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
		setColour(colour);

		CommandManager::getInstance().registerCommand("Command_ChangeNextScale", MyGUI::newDelegate(this, &TextureToolControl::CommandChangeNextScale));
		CommandManager::getInstance().registerCommand("Command_ChangePrevScale", MyGUI::newDelegate(this, &TextureToolControl::CommandChangePrevScale));
		CommandManager::getInstance().registerCommand("Command_ChangeScale", MyGUI::newDelegate(this, &TextureToolControl::CommandChangeScale));

		mScaleValue = SettingsManager::getInstance().getSector("TextureScale")->getPropertyValueList<size_t>("ScaleValue");

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &TextureToolControl::notifySettingsChanged);
	}

	TextureToolControl::~TextureToolControl()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &TextureToolControl::notifySettingsChanged);
	}

	void TextureToolControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "ColourBackground")
			{
				MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
				setColour(colour);
			}
		}
	}

	void TextureToolControl::CommandChangeNextScale(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		doNextScale();

		_result = true;
	}

	void TextureToolControl::CommandChangePrevScale(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		doPrevScale();

		_result = true;
	}

	void TextureToolControl::CommandChangeScale(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkMenuCommand())
			return;

		size_t scaleValue = MyGUI::utility::parseValue<size_t>(CommandManager::getInstance().getCommandData());
		if (scaleValue == mCurrentScaleValue)
			return;

		if (std::find(mScaleValue.begin(), mScaleValue.end(), scaleValue) == mScaleValue.end())
			return;

		mCurrentScaleValue = scaleValue;
		setScale((double)mCurrentScaleValue / (double)100);

		_result = true;
	}

	bool TextureToolControl::checkMenuCommand()
	{
		return mActivate &&
			!getSelectorsCapture();
	}

	bool TextureToolControl::checkCommand()
	{
		return mMainWidget->getRootKeyFocus() &&
			mActivate &&
			!getSelectorsCapture();
	}

	void TextureToolControl::onMouseWheel(int _rel)
	{
		if (_rel < 0)
			doPrevScale();
		else
			doNextScale();
	}

	bool TextureToolControl::doPrevScale()
	{
		for (VectorSizeT::const_iterator item = mScaleValue.begin(); item != mScaleValue.end(); ++item)
		{
			if ((*item) == mCurrentScaleValue)
			{
				if (item != mScaleValue.begin())
				{
					--item;
					mCurrentScaleValue = *item;
					setScale((double)mCurrentScaleValue / (double)100);
					return true;
				}
				break;
			}
		}
		return false;
	}

	bool TextureToolControl::doNextScale()
	{
		for (VectorSizeT::const_iterator item = mScaleValue.begin(); item != mScaleValue.end(); ++item)
		{
			if ((*item) == mCurrentScaleValue)
			{
				++item;
				if (item != mScaleValue.end())
				{
					mCurrentScaleValue = *item;
					setScale((double)mCurrentScaleValue / (double)100);
					return true;
				}
				break;
			}
		}
		return false;
	}

	void TextureToolControl::setActivate(bool _value)
	{
		mActivate = _value;

		onChangeActivate();
	}

	bool TextureToolControl::getActivate() const
	{
		return mActivate;
	}

	void TextureToolControl::onChangeActivate()
	{
	}

} // namespace tools
