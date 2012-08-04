#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportPlatform.h"

namespace Export
{
	MyGUI::ExportPlatform* mPlatform = nullptr;

	MYGUIEXPORT void MYGUICALL ExportGui_CreatePlatform(Convert<const std::string&>::Type _logName)
	{
		mPlatform = new MyGUI::ExportPlatform();
		mPlatform->initialise(Convert<const std::string&>::From(_logName));
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
		mGui->initialise(Convert<const std::string&>::From(_coreFileName));
	}

	MYGUIEXPORT void MYGUICALL ExportGui_DestroyGui()
	{
		mGui->shutdown();
		delete mGui;
		mGui = nullptr;
	}
}