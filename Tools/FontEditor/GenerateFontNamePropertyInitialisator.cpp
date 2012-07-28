/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "GenerateFontNamePropertyInitialisator.h"
#include "FactoryManager.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(GenerateFontNamePropertyInitialisator)

	GenerateFontNamePropertyInitialisator::GenerateFontNamePropertyInitialisator()
	{
	}

	GenerateFontNamePropertyInitialisator::~GenerateFontNamePropertyInitialisator()
	{
	}

	void GenerateFontNamePropertyInitialisator::initialise(PropertyPtr _property)
	{
		_property->setValue(MyGUI::utility::toString((size_t)_property.get(), "_GenerateFontName"));
	}

}
