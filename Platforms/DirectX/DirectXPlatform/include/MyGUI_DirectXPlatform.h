/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/

#ifndef __MYGUI_DIRECTX_PLATFORM_H__
#define __MYGUI_DIRECTX_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DirectXRenderManager.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXTexture.h"
#include "MyGUI_DirectXVertexBuffer.h"
#include "MyGUI_DirectXDiagnostic.h"

namespace MyGUI
{

	class DirectXPlatform
	{
	public:
		DirectXPlatform() :
			mIsInitialise(false)
		{
			mRenderManager = new DirectXRenderManager();
			mDataManager = new DirectXDataManager();
		}

		~DirectXPlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
		}

		void initialise(IDirect3DDevice9 *_device, const std::string& _logname = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			LogManager::registerSection(MYGUI_PLATFORM_LOG_SECTION, _logname);

			mRenderManager->initialise(_device);
			mDataManager->initialise();
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();

			// last platform log
			LogManager::unregisterSection(MYGUI_PLATFORM_LOG_SECTION);
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

	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_PLATFORM_H__
