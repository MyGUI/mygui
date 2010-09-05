/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_PLATFORM_H__
#define __MYGUI_DIRECTX_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DirectXRenderManager.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXTexture.h"
#include "MyGUI_DirectXVertexBuffer.h"
#include "MyGUI_DirectXDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class DirectXPlatform
	{
	public:
		DirectXPlatform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new DirectXRenderManager();
			mDataManager = new DirectXDataManager();
		}

		~DirectXPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(IDirect3DDevice9* _device, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(_device);
			mDataManager->initialise();
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		DirectXRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		DirectXDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		DirectXRenderManager* mRenderManager;
		DirectXDataManager* mDataManager;
		LogManager* mLogManager;

	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_PLATFORM_H__
