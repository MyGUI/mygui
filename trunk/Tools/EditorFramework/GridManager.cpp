/*!
	@file
	@author		George Evmenov
	@date		03/2011
*/

#include "Precompiled.h"
#include "GridManager.h"
#include "SettingsManager.h"

template <> tools::GridManager* MyGUI::Singleton<tools::GridManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::GridManager>::mClassTypeName = "GridManager";

namespace tools
{

	GridManager::GridManager() :
		mGridStep(0)
	{
	}

	GridManager::~GridManager()
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

	void GridManager::notifySettingsChanged(const std::string& _path)
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
		else if (_line == Previous)
			return (_value - 1) / mGridStep * mGridStep;
		else if (_line == Next)
			return (_value + mGridStep) / mGridStep * mGridStep;

		return _value;
	}

}
