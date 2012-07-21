/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "TexturePropertyInitialisator.h"
#include "FactoryManager.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TexturePropertyInitialisator)

	TexturePropertyInitialisator::TexturePropertyInitialisator()
	{
	}

	TexturePropertyInitialisator::~TexturePropertyInitialisator()
	{
	}

	void TexturePropertyInitialisator::initialise(Property* _property)
	{
		if (SettingsManager::getInstance().getValue<bool>("Settings/SaveLastTexture"))
		{
			std::string texture = SettingsManager::getInstance().getValueString("Resources/LastTextureName");
			_property->setValue(texture);
		}
	}

}
