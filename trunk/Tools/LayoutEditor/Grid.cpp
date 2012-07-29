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
		mGridStep = SettingsManager::getInstance().getValue<int>("Settings/GridStep");
		SettingsManager::getInstance().eventSettingsChanged.connect(this, &Grid::notifySettingsChanged);
	}

	void Grid::shutdown()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
	}

	void Grid::notifySettingsChanged(const std::string& _path)
	{
		if (_path == "Settings/GridStep")
			mGridStep = SettingsManager::getInstance().getValue<int>("Settings/GridStep");
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
