/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ScopeManager.h"
#include "CommandManager.h"

template <> tools::ScopeManager* MyGUI::Singleton<tools::ScopeManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ScopeManager>::mClassTypeName("ScopeManager");

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
		CommandManager::getInstance().registerCommand("Command_ChangeScope", MyGUI::newDelegate(this, &ScopeManager::commandChangeScope));

		mCurrentScope = "Image";
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
