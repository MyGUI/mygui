/*!
	@file
	@author     George Evmenov
	@date       08/2026
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
#include "BurnNodeAnimator.h"
#include "ShaderFadeNodeAnimator.h"
#include "ShaderWobbleNodeAnimator.h"

#if defined(MYGUI_OGRE_PLATFORM)
	#include "MyGUI_OgreRenderManager.h"
#elif defined(MYGUI_OGRENEXT_PLATFORM)
	#include "MyGUI_OgreNextRenderManager.h"
#endif

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_ShaderAnimations");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();

#if defined(MYGUI_OGRE_PLATFORM)
		{
			auto ext = MyGUI::OgreRenderManager::getInstance().getShaderExtension(true);
			MyGUI::RenderManager::getInstance().registerShader(
				"FadeShader",
				"MyGUI_Ogre_VP." + ext,
				"Fade_Ogre_FP." + ext);
			MyGUI::RenderManager::getInstance().registerShader(
				"WobbleShader",
				"Wobble_Ogre_VP." + ext,
				"MyGUI_Ogre_FP." + ext);
			MyGUI::RenderManager::getInstance().registerShader(
				"BurnShader",
				"MyGUI_Ogre_VP." + ext,
				"Burn_Ogre_FP." + ext);
		}
#elif defined(MYGUI_OGRENEXT_PLATFORM)
		{
			auto ext = MyGUI::OgreNextRenderManager::getInstance().getShaderExtension();
			MyGUI::RenderManager::getInstance().registerShader("FadeShader", "mygui/VP", "Fade_OgreNext_FP." + ext);
			MyGUI::RenderManager::getInstance().registerShader("WobbleShader", "Wobble_OgreNext_VP." + ext, "mygui/FP");
			MyGUI::RenderManager::getInstance().registerShader("BurnShader", "mygui/VP", "Burn_OgreNext_FP." + ext);
		}
#elif defined(MYGUI_OPENGL3_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader(
			"FadeShader",
			"MyGUI_OpenGL3_VP.glsl",
			"Fade_OpenGL3_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader(
			"WobbleShader",
			"Wobble_OpenGL3_VP.glsl",
			"MyGUI_OpenGL3_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader(
			"BurnShader",
			"MyGUI_OpenGL3_VP.glsl",
			"Burn_OpenGL3_FP.glsl");
#elif defined(MYGUI_OSG_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader("FadeShader", "MyGUI_Osg_VP.glsl", "Fade_OpenGL3_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader(
			"WobbleShader",
			"Wobble_Osg_VP.glsl",
			"MyGUI_OpenGL3_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader("BurnShader", "MyGUI_Osg_VP.glsl", "Burn_OpenGL3_FP.glsl");
#elif defined(MYGUI_OPENGLES_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader(
			"FadeShader",
			"MyGUI_OpenGLES_VP.glsl",
			"Fade_OpenGLES_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader(
			"WobbleShader",
			"Wobble_OpenGLES_VP.glsl",
			"MyGUI_OpenGLES_FP.glsl");
		MyGUI::RenderManager::getInstance().registerShader(
			"BurnShader",
			"MyGUI_OpenGLES_VP.glsl",
			"Burn_OpenGLES_FP.glsl");
#endif
		MyGUI::FactoryManager::getInstance().registerFactory<BurnNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().registerFactory<ShaderFadeNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().registerFactory<ShaderWobbleNodeAnimator>("NodeAnimator");

		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>(layerCategory);

		MyGUI::ResourceManager::getInstance().load("Layers.xml");

		createWindows();
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<BurnNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ShaderFadeNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ShaderWobbleNodeAnimator>("NodeAnimator");

		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>(layerCategory);
	}

	void DemoKeeper::notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name)
	{
		if (_name == "close")
		{
			mWidgets.erase(_sender);
			MyGUI::WidgetManager::getInstance().destroyWidget(_sender);
		}
	}

	void DemoKeeper::createWindows()
	{
		for (auto* widget : MyGUI::LayoutManager::getInstance().loadLayout("Windows.layout"))
		{
			MyGUI::Window* window = widget->castType<MyGUI::Window>();
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &DemoKeeper::notifyWindowButtonPressed);
			mWidgets.insert(window);
		}
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (_key == MyGUI::KeyCode::W)
		{
			createWindows();
		}
		else if (_key == MyGUI::KeyCode::V)
		{
			for (auto* widget : mWidgets)
			{
				const MyGUI::IntCoord coord(0, 0, 1024, 768);
				const MyGUI::IntSize size(300, 300);

				if (widget->getCoord().width != coord.width)
				{
					widget->setCoord(coord);
				}
				else
				{
					widget->setCoord(
						coord.width / 2 - size.width / 2,
						coord.height / 2 - size.height / 2,
						size.width,
						size.height);
				}
			}
		}

		return base::BaseDemoManager::injectKeyPress(_key, _text);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
