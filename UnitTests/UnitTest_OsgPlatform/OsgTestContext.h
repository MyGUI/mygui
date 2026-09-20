#ifndef MYGUI_UNITTEST_OSG_TEST_CONTEXT_H_
#define MYGUI_UNITTEST_OSG_TEST_CONTEXT_H_

#include "BehaviourTestSupport.h"
#include "MyGUI_OsgPlatform.h"
#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgVertexBuffer.h"
#include <osg/Camera>
#include <osg/Group>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/BufferObject>
#include <osg/StateSet>

namespace unittest
{

	class OsgTestContext
	{
	public:
		OsgTestContext()
		{
			platform.getDataManagerPtr()->addResourceLocation(MYGUI_OSG_TEST_MEDIA, false);
			platform.getDataManagerPtr()->addResourceLocation(MYGUI_OSG_TEST_RESOURCES, false);
			platform.initialise(root, 64, 64, "");
		}
		~OsgTestContext()
		{
			platform.shutdown();
		}
		MyGUI::OsgRenderManager& render()
		{
			return *platform.getRenderManagerPtr();
		}
		MyGUI::OsgTexture* texture(const std::string& name)
		{
			return static_cast<MyGUI::OsgTexture*>(render().createTexture(name));
		}
		osg::ref_ptr<osg::Group> root = new osg::Group;
		MyGUI::OsgPlatform platform;
	};

	template<typename Function>
	void requireThrows(Function function, const char* message)
	{
		bool caught = false;
		try
		{
			function();
		}
		catch (const MyGUI::Exception&)
		{
			caught = true;
		}
		require(caught, message);
	}

}

#ifdef MYGUI_OSG_RENDER_TESTS
int runOsgRenderTests();
int runOsgBenchmark();
#endif

#endif
