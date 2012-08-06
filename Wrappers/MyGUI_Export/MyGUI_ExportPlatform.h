/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef __MYGUI_EXPORT_PLATFORM_H__
#define __MYGUI_EXPORT_PLATFORM_H__

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

} // namespace MyGUI

#endif // __MYGUI_EXPORT_PLATFORM_H__
