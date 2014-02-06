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

	TextureToolControlLE::TextureToolControlLE(MyGUI::Widget* _parent) :
		TextureControlLE("TextureControl.layout", _parent),
		mCurrentScaleValue(100),
		mActivate(true)
	{
		MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/ColourBackground");
		setColour(colour);

		CommandManager::getInstance().getEvent("Command_ChangeNextScale")->connect(this, &TextureToolControlLE::CommandChangeNextScale);
		CommandManager::getInstance().getEvent("Command_ChangePrevScale")->connect(this, &TextureToolControlLE::CommandChangePrevScale);
		CommandManager::getInstance().getEvent("Command_ChangeScale")->connect(this, &TextureToolControlLE::CommandChangeScale);

		mScaleValue = SettingsManager::getInstance().getValueList<size_t>("Workspace/TextureScale/ScaleValue.List");

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &TextureToolControlLE::notifySettingsChanged);
	}

	TextureToolControlLE::~TextureToolControlLE()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
	}

	void TextureToolControlLE::notifySettingsChanged(const std::string& _path)
	{
		if (_path == "Workspace/Colours/ColourBackground")
		{
			MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/ColourBackground");
			setColour(colour);
		}
	}

	void TextureToolControlLE::CommandChangeNextScale(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		doNextScale();

		_result = true;
	}

	void TextureToolControlLE::CommandChangePrevScale(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		doPrevScale();

		_result = true;
	}

	void TextureToolControlLE::CommandChangeScale(const MyGUI::UString& _commandName, bool& _result)
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

	bool TextureToolControlLE::checkMenuCommand()
	{
		return mActivate &&
			!getSelectorsCapture();
	}

	bool TextureToolControlLE::checkCommand()
	{
		return mMainWidget->getRootKeyFocus() &&
			mActivate &&
			!getSelectorsCapture();
	}

	void TextureToolControlLE::onMouseWheel(int _rel)
	{
		if (_rel < 0)
			doPrevScale();
		else
			doNextScale();
	}

	bool TextureToolControlLE::doPrevScale()
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

	bool TextureToolControlLE::doNextScale()
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

	void TextureToolControlLE::setActivate(bool _value)
	{
		mActivate = _value;

		onChangeActivate();
	}

	bool TextureToolControlLE::getActivate() const
	{
		return mActivate;
	}

	void TextureToolControlLE::onChangeActivate()
	{
	}

}
