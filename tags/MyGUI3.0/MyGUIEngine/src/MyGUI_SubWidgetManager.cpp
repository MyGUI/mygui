/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
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
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_SimpleText.h"
#include "MyGUI_EditText.h"
#include "MyGUI_RawRect.h"
#include "MyGUI_TileRect.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(SubWidgetManager);

	void SubWidgetManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		FactoryManager& factory = FactoryManager::getInstance();

		factory.registryFactory<SubSkinStateInfo>("BasisSkin/State", "SubSkin");
		factory.registryFactory<SubSkinStateInfo>("BasisSkin/State", "MainSkin");
		factory.registryFactory<SubSkinStateInfo>("BasisSkin/State", "RotatingSkin");
		factory.registryFactory<SubSkinStateInfo>("BasisSkin/State", "RawRect");
		factory.registryFactory<TileRectStateInfo>("BasisSkin/State", "TileRect");
		factory.registryFactory<EditTextStateInfo>("BasisSkin/State", "EditText");
		factory.registryFactory<EditTextStateInfo>("BasisSkin/State", "SimpleText");

		factory.registryFactory<SubSkin>("BasisSkin");
		factory.registryFactory<MainSkin>("BasisSkin");
		factory.registryFactory<RotatingSkin>("BasisSkin");
		factory.registryFactory<RawRect>("BasisSkin");
		factory.registryFactory<TileRect>("BasisSkin");
		factory.registryFactory<EditText>("BasisSkin");
		factory.registryFactory<SimpleText>("BasisSkin");

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SubWidgetManager::shutdown()
	{
		if (!mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		FactoryManager& factory = FactoryManager::getInstance();

		factory.unregistryFactory("BasisSkin/State", "SubSkin");
		factory.unregistryFactory("BasisSkin/State", "MainSkin");
		factory.unregistryFactory("BasisSkin/State", "RotatingSkin");
		factory.unregistryFactory("BasisSkin/State", "RawRect");
		factory.unregistryFactory("BasisSkin/State", "TileRect");
		factory.unregistryFactory("BasisSkin/State", "EditText");
		factory.unregistryFactory("BasisSkin/State", "SimpleText");

		factory.unregistryFactory<SubSkin>("BasisSkin");
		factory.unregistryFactory<MainSkin>("BasisSkin");
		factory.unregistryFactory<RotatingSkin>("BasisSkin");
		factory.unregistryFactory<RawRect>("BasisSkin");
		factory.unregistryFactory<TileRect>("BasisSkin");
		factory.unregistryFactory<EditText>("BasisSkin");
		factory.unregistryFactory<SimpleText>("BasisSkin");

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

} // namespace MyGUI
