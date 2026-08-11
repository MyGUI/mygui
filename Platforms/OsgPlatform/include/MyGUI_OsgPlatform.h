/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#ifndef MYGUI_OSG_PLATFORM_H_
#define MYGUI_OSG_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgRenderManager.h"
#include "MyGUI_OsgDataManager.h"
#include "MyGUI_LogManager.h"

namespace osg
{
	class Group;
}

namespace MyGUI
{

	class OsgPlatform
	{
	public:
		OsgPlatform();
		~OsgPlatform();

		void initialise(
			osg::Group* _sceneRoot,
			int _width,
			int _height,
			std::string_view _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		OsgRenderManager* getRenderManagerPtr() const;
		OsgDataManager* getDataManagerPtr() const;

	private:
		bool mIsInitialise{false};
		OsgRenderManager* mRenderManager;
		OsgDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_OSG_PLATFORM_H_
