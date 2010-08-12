/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "ActionManager.h"

template <> tools::ActionManager* MyGUI::Singleton<tools::ActionManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ActionManager>::mClassTypeName("ActionManager");

namespace tools
{

	ActionManager::ActionManager()
	{
	}

	ActionManager::~ActionManager()
	{
	}

	void ActionManager::initialise()
	{
	}

	void ActionManager::shutdown()
	{
	}

	bool ActionManager::hasChanges()
	{
		return true;
	}

} // namespace tools
