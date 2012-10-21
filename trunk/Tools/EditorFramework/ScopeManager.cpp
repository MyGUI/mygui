/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ScopeManager.h"
#include "CommandManager.h"
#include "SettingsManager.h"

template <> tools::ScopeManager* MyGUI::Singleton<tools::ScopeManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ScopeManager>::mClassTypeName = "ScopeManager";

namespace tools
{

	ScopeManager::ScopeManager()
	{
	}

	ScopeManager::~ScopeManager()
	{
	}

	void ScopeManager::initialise()
	{
		CommandManager::getInstance().getEvent("Command_ChangeScope")->connect(this, &ScopeManager::commandChangeScope);

		mCurrentScope = SettingsManager::getInstance().getValue("Editor/DefaultScope");
	}

	void ScopeManager::shutdown()
	{
	}

	void ScopeManager::commandChangeScope(const MyGUI::UString& _commandName, bool& _result)
	{
		if (CommandManager::getInstance().getCommandData() != mCurrentScope)
		{
			mCurrentScope = CommandManager::getInstance().getCommandData();
			eventChangeScope(mCurrentScope);
		}

		_result = true;
	}

	const std::string& ScopeManager::getCurrentScope() const
	{
		return mCurrentScope;
	}

}
