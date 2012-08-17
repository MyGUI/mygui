#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportPlatform.h"
#include "MyGUI_CustomLayer.h"

namespace Export
{
	MyGUI::ExportPlatform* mPlatform = nullptr;

	MYGUIEXPORT void MYGUICALL ExportPlatform_Create(Convert<const std::string&>::Type _logName)
	{
		mPlatform = new MyGUI::ExportPlatform();
		mPlatform->initialise(Convert<const std::string&>::From(_logName));
	}

	MYGUIEXPORT void MYGUICALL ExportPlatform_Destroy()
	{
		mPlatform->shutdown();
		delete mPlatform;
		mPlatform = nullptr;
	}

	MYGUIEXPORT void MYGUICALL ExportRenderManager_DrawOneFrame()
	{
		mPlatform->getRenderManagerPtr()->drawOneFrame();
	}

	MYGUIEXPORT void MYGUICALL ExportRenderManager_SetViewSize(
		Convert<int>::Type _width,
		Convert<int>::Type _height)
	{
		mPlatform->getRenderManagerPtr()->setViewSize(_width, _height);
	}

	MYGUIEXPORT void MYGUICALL ExportPlatform_Log(
		Convert<MyGUI::LogLevel>::Type _level,
		Convert<const std::string&>::Type _message )
	{
		MyGUI::LogManager::getInstance().log(
			Convert<const std::string&>::From(MYGUI_PLATFORM_LOG_SECTION),
			Convert<MyGUI::LogLevel>::From(_level),
			Convert<const std::string&>::From(_message),
			__FILE__,
			__LINE__);
	}

	MyGUI::Gui* mGui = nullptr;

	MYGUIEXPORT void MYGUICALL ExportGui_Create(Convert<const std::string&>::Type _coreFileName)
	{
		mGui = new MyGUI::Gui();
		mGui->initialise(Convert<const std::string&>::From(_coreFileName));

		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::CustomLayer>("Layer");
	}

	MYGUIEXPORT void MYGUICALL ExportGui_Destroy()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::CustomLayer>("Layer");

		mGui->shutdown();
		delete mGui;
		mGui = nullptr;
	}
}