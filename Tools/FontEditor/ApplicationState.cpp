/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ApplicationState.h"
#include "FactoryManager.h"
#include "Application.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(ApplicationState)

	ApplicationState::ApplicationState()
	{
	}

	ApplicationState::~ApplicationState()
	{
	}

	void ApplicationState::initState()
	{
	}

	void ApplicationState::cleanupState()
	{
	}

	void ApplicationState::pauseState()
	{
	}

	void ApplicationState::resumeState()
	{
		Application::getInstance().quit();
	}

}
