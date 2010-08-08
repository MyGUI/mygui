/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinManager.h"

template <> tools::SkinManager* MyGUI::Singleton<tools::SkinManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::SkinManager>::mClassTypeName("SkinManager");

namespace tools
{

	SkinManager::SkinManager()
	{
	}

	SkinManager::~SkinManager()
	{
	}

	void SkinManager::initialise()
	{
	}

	void SkinManager::shutdown()
	{
		destroyAllChilds();
	}

} // namespace tools
