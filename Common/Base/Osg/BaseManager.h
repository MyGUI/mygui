#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_OsgPlatform.h>

#include <osg/ref_ptr>

namespace osg
{
	class Group;
}
namespace osgViewer
{
	class Viewer;
}

namespace base
{

	class BaseManager : public SdlBaseManager
	{
	public:
		BaseManager();
		~BaseManager() override;

		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

		/*internal:*/
		osg::Group* getSceneRoot()
		{
			return mSceneRoot;
		}

	private:
		MyGUI::OsgPlatform* mPlatform = nullptr;
		osg::ref_ptr<osgViewer::Viewer> mViewer;
		osg::ref_ptr<osg::Group> mSceneRoot;
		int mRenderWidth = 0;
		int mRenderHeight = 0;
	};

} // namespace base
