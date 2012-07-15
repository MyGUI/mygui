/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ScopeManager.h"

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
	}

	void ScopeManager::shutdown()
	{
	}

}
