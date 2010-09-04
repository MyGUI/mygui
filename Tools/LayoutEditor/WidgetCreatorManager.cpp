/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "WidgetCreatorManager.h"

template <> tools::WidgetCreatorManager* MyGUI::Singleton<tools::WidgetCreatorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetCreatorManager>::mClassTypeName("WidgetCreatorManager");

namespace tools
{
	WidgetCreatorManager::WidgetCreatorManager()
	{
	}

	WidgetCreatorManager::~WidgetCreatorManager()
	{
	}

	void WidgetCreatorManager::initialise()
	{
	}

	void WidgetCreatorManager::shutdown()
	{
	}

} // namespace tools
