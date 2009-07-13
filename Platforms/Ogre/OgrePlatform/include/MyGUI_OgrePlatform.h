/*!
	@file
	@author		Albert Semenov
	@date		04/2009
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
#ifndef __MYGUI_OGRE_PLATFORM_H__
#define __MYGUI_OGRE_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_OgreVertexBuffer.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreTextureManager.h"
#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreViewport.h"
#include "MyGUI_OgreDiagnostic.h"

namespace MyGUI
{

	class OgrePlatform
	{
	public:
		OgrePlatform() :
			mIsInitialise(false)
		{
			mRenderManager = new OgreRenderManager();
			mTextureManager = new OgreTextureManager();
			mDataManager = new OgreDataManager();
		}

		~OgrePlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mTextureManager;
			delete mDataManager;
		}

		void initialise(Ogre::RenderWindow* _window, const std::string& _group = "General", const std::string& _logname = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			LogManager::registerSection(MYGUI_PLATFORM_LOG_SECTION, _logname);

			mRenderManager->initialise(_window);
			mTextureManager->initialise();
			mDataManager->initialise(_group);
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mTextureManager->shutdown();
			mDataManager->shutdown();

			// last platform log
			LogManager::unregisterSection(MYGUI_PLATFORM_LOG_SECTION);
		}

		OgreRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		OgreTextureManager* getTextureManagerPtr()
		{
			assert(mIsInitialise);
			return mTextureManager;
		}

		OgreDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		OgreRenderManager* mRenderManager;
		OgreTextureManager* mTextureManager;
		OgreDataManager* mDataManager;

	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_PLATFORM_H__
