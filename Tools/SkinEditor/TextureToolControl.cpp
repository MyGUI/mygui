/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "TextureToolControl.h"
#include "Localise.h"
#include "SettingsManager.h"

namespace tools
{

	TextureToolControl::TextureToolControl(MyGUI::Widget* _parent) :
		TextureControl("TextureControl.layout", _parent)
	{
		MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
		setColour(colour);

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

} // namespace tools
