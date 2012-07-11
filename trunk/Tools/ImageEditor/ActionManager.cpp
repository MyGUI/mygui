/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "ActionManager.h"

template <> tools::ActionManager* MyGUI::Singleton<tools::ActionManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ActionManager>::mClassTypeName("ActionManager");

namespace tools
{

	ActionManager::ActionManager() :
		mChanges(false)
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

	bool ActionManager::getChanges() const
	{
		return mChanges;
	}

	void ActionManager::setChanges(bool _value)
	{
		if (mChanges != _value)
		{
			mChanges = _value;
			eventChanges(mChanges);
		}
	}

} // namespace tools
