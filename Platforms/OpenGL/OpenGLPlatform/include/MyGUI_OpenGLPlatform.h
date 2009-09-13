/*!
	@file
	@author		George Evmenov
	@date		07/2009
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
#ifndef __MYGUI_OPENGL_PLATFORM_H__
#define __MYGUI_OPENGL_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLTextureManager.h"
#include "MyGUI_OpenGLDataManager.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_OpenGLDiagnostic.h"

namespace MyGUI
{

	class OpenGLPlatform
	{
	public:
		OpenGLPlatform() :
			mIsInitialise(false)
		{
			mRenderManager = new OpenGLRenderManager();
			mTextureManager = new OpenGLTextureManager();
			mDataManager = new OpenGLDataManager();
		}

		~OpenGLPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mTextureManager;
			delete mDataManager;
		}

		void initialise(const std::string& _logname = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			LogManager::registerSection(MYGUI_PLATFORM_LOG_SECTION, _logname);

			mRenderManager->initialise();
			mTextureManager->initialise();
			mDataManager->initialise();
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

		OpenGLRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		OpenGLTextureManager* getTextureManagerPtr()
		{
			assert(mIsInitialise);
			return mTextureManager;
		}

		OpenGLDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		OpenGLRenderManager* mRenderManager;
		OpenGLTextureManager* mTextureManager;
		OpenGLDataManager* mDataManager;

	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_PLATFORM_H__
