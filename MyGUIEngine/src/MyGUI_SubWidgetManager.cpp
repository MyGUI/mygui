/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_CommonStateInfo.h"

#include "MyGUI_SubSkin.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_PolygonalSkin.h"
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_SimpleText.h"
#include "MyGUI_EditText.h"
#include "MyGUI_TileRect.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(SubWidgetManager);

	SubWidgetManager::SubWidgetManager() :
		mSingletonHolder(this),
		mCategoryName("BasisSkin"),
		mStateCategoryName("BasisSkin/State")
	{
	}

	void SubWidgetManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		FactoryManager& factory = FactoryManager::getInstance();

		factory.registerFactory<SubSkinStateInfo>(mStateCategoryName, "SubSkin");
		factory.registerFactory<SubSkinStateInfo>(mStateCategoryName, "MainSkin");
		factory.registerFactory<SubSkinStateInfo>(mStateCategoryName, "PolygonalSkin");
		factory.registerFactory<TileRectStateInfo>(mStateCategoryName, "TileRect");
		factory.registerFactory<EditTextStateInfo>(mStateCategoryName, "EditText");
		factory.registerFactory<EditTextStateInfo>(mStateCategoryName, "SimpleText");

		factory.registerFactory<SubSkin>(mCategoryName);
		factory.registerFactory<MainSkin>(mCategoryName);
		factory.registerFactory<PolygonalSkin>(mCategoryName);
#ifndef MYGUI_DONT_USE_OBSOLETE
		// Keep the legacy factory available for existing skins.
		factory.registerFactory<RotatingSkinStateInfo>(mStateCategoryName, "RotatingSkin");
		MYGUI_SUPPRESS_MSVC(4996)
		MYGUI_SUPPRESS_GCC("-Wdeprecated-declarations")
		factory.registerFactory<RotatingSkin>(mCategoryName);
		MYGUI_UNSUPPRESS_GCC()
		MYGUI_UNSUPPRESS_MSVC()
#endif // MYGUI_DONT_USE_OBSOLETE
		factory.registerFactory<TileRect>(mCategoryName);
		factory.registerFactory<EditText>(mCategoryName);
		factory.registerFactory<SimpleText>(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void SubWidgetManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		FactoryManager& factory = FactoryManager::getInstance();

		factory.unregisterFactory(mStateCategoryName, "SubSkin");
		factory.unregisterFactory(mStateCategoryName, "MainSkin");
		factory.unregisterFactory(mStateCategoryName, "PolygonalSkin");
		factory.unregisterFactory(mStateCategoryName, "TileRect");
		factory.unregisterFactory(mStateCategoryName, "EditText");
		factory.unregisterFactory(mStateCategoryName, "SimpleText");

		factory.unregisterFactory<SubSkin>(mCategoryName);
		factory.unregisterFactory<MainSkin>(mCategoryName);
		factory.unregisterFactory<PolygonalSkin>(mCategoryName);
#ifndef MYGUI_DONT_USE_OBSOLETE
		factory.unregisterFactory(mStateCategoryName, "RotatingSkin");
		MYGUI_SUPPRESS_MSVC(4996)
		MYGUI_SUPPRESS_GCC("-Wdeprecated-declarations")
		factory.unregisterFactory<RotatingSkin>(mCategoryName);
		MYGUI_UNSUPPRESS_GCC()
		MYGUI_UNSUPPRESS_MSVC()
#endif // MYGUI_DONT_USE_OBSOLETE
		factory.unregisterFactory<TileRect>(mCategoryName);
		factory.unregisterFactory<EditText>(mCategoryName);
		factory.unregisterFactory<SimpleText>(mCategoryName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	const std::string& SubWidgetManager::getCategoryName() const
	{
		return mCategoryName;
	}

	const std::string& SubWidgetManager::getStateCategoryName() const
	{
		return mStateCategoryName;
	}

} // namespace MyGUI
