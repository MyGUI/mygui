#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_DummyPlatform.h"

namespace Export
{
	MyGUI::DummyPlatform* mPlatform = nullptr;

	MYGUIEXPORT void MYGUICALL ExportGui_CreatePlatform(Convert<const std::string&>::Type _logName)
	{
		mPlatform = new MyGUI::DummyPlatform();
		mPlatform->initialise(_logName);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyPlatform()
	{
		mPlatform->shutdown();
		delete mPlatform;
		mPlatform = nullptr;
	}

	MyGUI::Gui* mGui = nullptr;

	MYGUIEXPORT void MYGUICALL ExportGui_CreateGui(Convert<const std::string&>::Type _coreFileName)
	{
		mGui = new MyGUI::Gui();
		mGui->initialise(_coreFileName);
	}

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyGui()
	{
		mGui->shutdown();
		delete mGui;
		mGui = nullptr;
	}
}