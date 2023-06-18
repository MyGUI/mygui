/*!
	@file
	@author		George Evmenov
	@date		03/2011
*/

#include "Precompiled.h"
#include "GridManager.h"
#include "SettingsManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(GridManager);

	GridManager::GridManager() :
		mSingletonHolder(this)
	{
	}

	void GridManager::initialise()
	{
		mGridStep = SettingsManager::getInstance().getValue<int>("Settings/GridStep");
		SettingsManager::getInstance().eventSettingsChanged.connect(this, &GridManager::notifySettingsChanged);
	}

	void GridManager::shutdown()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
	}

	void GridManager::notifySettingsChanged(std::string_view _path)
	{
		if (_path == "Settings/GridStep")
		{
			mGridStep = SettingsManager::getInstance().getValue<int>("Settings/GridStep");
		}
	}

	int GridManager::toGrid(int _value, GridLine _line) const
	{
		if (mGridStep < 1)
			return _value;

		if (_line == Closest)
			return (_value + mGridStep / 2) / mGridStep * mGridStep;
		if (_line == Previous)
			return (_value - 1) / mGridStep * mGridStep;
		if (_line == Next)
			return (_value + mGridStep) / mGridStep * mGridStep;

		return _value;
	}

}
