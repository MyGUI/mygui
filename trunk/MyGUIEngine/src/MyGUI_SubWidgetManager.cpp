/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
		factory.registerFactory<SubSkinStateInfo>(mStateCategoryName, "RotatingSkin");
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
