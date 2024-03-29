/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef MYGUI_DIRECTX11_PLATFORM_H_
#define MYGUI_DIRECTX11_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11VertexBuffer.h"
#include "MyGUI_DirectX11Diagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class DirectX11Platform
	{
	public:
		DirectX11Platform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new DirectX11RenderManager();
			mDataManager = new DirectX11DataManager();
		}

		~DirectX11Platform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(ID3D11Device* _device, std::string_view _logName = MYGUI_PLATFORM_LOG_FILENAME)
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

		DirectX11RenderManager* getRenderManagerPtr()
		{
			return mRenderManager;
		}

		DirectX11DataManager* getDataManagerPtr()
		{
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		DirectX11RenderManager* mRenderManager;
		DirectX11DataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX11_PLATFORM_H_
