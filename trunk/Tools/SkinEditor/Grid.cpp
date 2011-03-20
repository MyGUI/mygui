/*!
	@file
	@author		George Evmenov
	@date		03/2011
*/
#include "Precompiled.h"
#include "Grid.h"
#include "SettingsManager.h"

template <> tools::Grid* MyGUI::Singleton<tools::Grid>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::Grid>::mClassTypeName("Grid");

namespace tools
{
	Grid::Grid() :
		mGridStep(0)
	{
	}

	Grid::~Grid()
	{
	}

	void Grid::initialise()
	{
		mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &Grid::notifySettingsChanged);
	}

	void Grid::shutdown()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &Grid::notifySettingsChanged);
	}

	void Grid::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "Grid")
				mGridStep = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("Grid");
		}
	}

	int Grid::toGrid(int _value, GridLine _line) const
	{
		if (mGridStep < 1)
			return _value;

		if (_line == Closest)
			return (_value + mGridStep / 2) / mGridStep * mGridStep;
		else if (_line == Previous)
			return (_value - 1) / mGridStep * mGridStep;
		else if (_line == Next)
			return (_value + mGridStep) / mGridStep * mGridStep;

		return _value;
	}

} // namespace tools
