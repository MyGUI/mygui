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

	template <> SubWidgetManager* Singleton<SubWidgetManager>::msInstance = nullptr;
	template <> const char* Singleton<SubWidgetManager>::mClassTypeName = "SubWidgetManager";

	SubWidgetManager::SubWidgetManager() :
		mIsInitialise(false),
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
		factory.registerFactory<RotatingSkinStateInfo>(mStateCategoryName, "RotatingSkin");
		factory.registerFactory<TileRectStateInfo>(mStateCategoryName, "TileRect");
		factory.registerFactory<EditTextStateInfo>(mStateCategoryName, "EditText");
		factory.registerFactory<EditTextStateInfo>(mStateCategoryName, "SimpleText");

		factory.registerFactory<SubSkin>(mCategoryName);
		factory.registerFactory<MainSkin>(mCategoryName);
		factory.registerFactory<PolygonalSkin>(mCategoryName);
		factory.registerFactory<RotatingSkin>(mCategoryName);
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
		factory.unregisterFactory(mStateCategoryName, "RotatingSkin");
		factory.unregisterFactory(mStateCategoryName, "TileRect");
		factory.unregisterFactory(mStateCategoryName, "EditText");
		factory.unregisterFactory(mStateCategoryName, "SimpleText");

		factory.unregisterFactory<SubSkin>(mCategoryName);
		factory.unregisterFactory<MainSkin>(mCategoryName);
		factory.unregisterFactory<PolygonalSkin>(mCategoryName);
		factory.unregisterFactory<RotatingSkin>(mCategoryName);
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
