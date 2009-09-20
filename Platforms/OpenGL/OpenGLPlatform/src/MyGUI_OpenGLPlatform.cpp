/*!
	@file
	@author		Albert Semenov
	@date		09/2009
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
#include "MyGUI_OpenGLPlatform.h"

namespace MyGUI
{

	OpenGLPlatform::OpenGLPlatform() :
		mIsInitialise(false)
	{
		mRenderManager = new OpenGLRenderManager();
		mDataManager = new OpenGLDataManager();
	}

	OpenGLPlatform::~OpenGLPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
	}

	void OpenGLPlatform::initialise(const std::string& _logname)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		LogManager::registerSection(MYGUI_PLATFORM_LOG_SECTION, _logname);

		mRenderManager->initialise();
		mDataManager->initialise();
	}

	void OpenGLPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();

		// last platform log
		LogManager::unregisterSection(MYGUI_PLATFORM_LOG_SECTION);
	}

	OpenGLRenderManager* OpenGLPlatform::getRenderManagerPtr()
	{
		assert(mIsInitialise);
		return mRenderManager;
	}

	OpenGLDataManager* OpenGLPlatform::getDataManagerPtr()
	{
		assert(mIsInitialise);
		return mDataManager;
	}

} // namespace MyGUI
