/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _80486629_6f18_4312_b5f0_3f06d96f7911_
#define _80486629_6f18_4312_b5f0_3f06d96f7911_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDataManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class ExportPlatform
	{
	public:
		ExportPlatform();
		~ExportPlatform();

		void initialise(const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		ExportRenderManager* getRenderManagerPtr();
		ExportDataManager* getDataManagerPtr();

	private:
		ExportRenderManager* mRenderManager;
		ExportDataManager* mDataManager;
		LogManager* mLogManager;
	};

}

#endif
