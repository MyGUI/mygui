#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#if defined(MYGUI_OGRE_PLATFORM)
	#include "MyGUI_OgreRenderManager.h"
#endif

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_MsdfFont");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		// MsdfFontShader uses custom fragment program and default vertex program
#if defined(MYGUI_OGRE_PLATFORM)
		auto& renderManager = MyGUI::OgreRenderManager::getInstance();
		std::string vpFile = "MyGUI_Ogre_VP." + renderManager.getShaderExtension();
		std::string fpFile = "Msdf_Ogre_FP." + renderManager.getShaderExtension();
		// custom VP for glsles to match shader versions (Msdf fragment shader require glsl es 300, while default MyGUI shader use glsl es 100)
		if (renderManager.getShaderExtension() == "glsles")
			vpFile = "Msdf_Ogre_VP." + renderManager.getShaderExtension();
		MyGUI::RenderManager::getInstance().registerShader("MsdfFontShader", vpFile, fpFile);
#elif defined(MYGUI_OPENGL_PLATFORM)
		// TODO not implemented in RenderManager
#elif defined(MYGUI_DIRECTX_PLATFORM)
		// TODO not implemented in RenderManager
#elif defined(MYGUI_DIRECTX11_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader(
			"MsdfFontShader",
			"MyGUI_DirectX11_VP.hlsl",
			"Msdf_DirectX11_FP.hlsl");
#elif defined(MYGUI_OPENGL3_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader(
			"MsdfFontShader",
			"MyGUI_OpenGL3_VP.glsl",
			"Msdf_OpenGL3_FP.glsl");
#elif defined(MYGUI_OPENGLES_PLATFORM)
		MyGUI::RenderManager::getInstance().registerShader(
			"MsdfFontShader",
			"MyGUI_OpenGLES_VP.glsl",
			"Msdf_OpenGLES_FP.glsl");
#endif

		MyGUI::ResourceManager::getInstance().load("MsdfFont.xml");
		MyGUI::ResourceManager::getInstance().load("DejaVuSansFont_126.xml");

#ifdef MYGUI_MSDF_FONTS
		MyGUI::ResourceManager::getInstance().load("MsdfFont_Runtime.xml");
#endif

		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption(
				"Msdf font, that can be scaled and rendered with a high quality, while single small texture is used "
				"for all sizes. Small-size fonts are still better with a regular texture.");

		MyGUI::Button* button1 = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
			"Button",
			MyGUI::IntCoord(110, 10, 200, 50),
			MyGUI::Align::Default,
			"Main");
		button1->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::increaseFontHeight);
		button1->setCaption("Increase font height");
		MyGUI::Button* button2 = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>(
			"Button",
			MyGUI::IntCoord(320, 10, 200, 50),
			MyGUI::Align::Default,
			"Main");
		button2->eventMouseButtonClick += MyGUI::newDelegate(this, &DemoKeeper::decreaseFontHeight);
		button2->setCaption("Decrease font height");

		MyGUI::ImageBox* fontTexture = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>(
			"ImageBox",
			MyGUI::IntCoord(110, 70, 256, 256),
			MyGUI::Align::Default,
			"Back");
		fontTexture->setImageTexture("MsdfFont.png");

		int index = 0;
		auto createEditBox =
			[&](const std::string& fontName, int fontHeight, const std::string& captionPrefix) -> MyGUI::EditBox*
		{
			std::string iStr = std::to_string(index);
			const MyGUI::VectorWidgetPtr& widgets =
				MyGUI::LayoutManager::getInstance().loadLayout("EditPanel.layout", iStr);
			auto* edit = widgets.at(0)->findWidget(iStr + "Edit")->castType<MyGUI::EditBox>();
			if (!fontName.empty())
				edit->setFontName(fontName);
			if (fontHeight)
				edit->setFontHeight(fontHeight);
			widgets.at(0)->setPosition((index + 1) * 100, (index + 1) * 75);
			if (!captionPrefix.empty())
				widgets.at(0)->castType<MyGUI::Window>()->setCaption(captionPrefix);
			mEditBoxes.push_back(edit);
			index++;
			return edit;
		};

		createEditBox({}, 0, "Regular font.");
		createEditBox("MsdfFont", 15, "Msdf is not optimal for small fonts.");
		createEditBox("MsdfFont", 45, "MsdfFont 40 (one 256x256 texture for all sizes)");
		createEditBox("MsdfFont", 126, "MsdfFont 126 (one 256x256 texture for all sizes)");
		createEditBox("DejaVuSansFont_126", 0, "DejaVuSansFont_126 (1024x512 texture)");
#ifdef MYGUI_MSDF_FONTS
		createEditBox("MsdfFont_Runtime", 45, "Runtime generated MSDF font.");
#endif
	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::increaseFontHeight(MyGUI::Widget* _sender)
	{
		for (auto* edit : mEditBoxes)
		{
			edit->setFontHeight(edit->getFontHeight() + 1);
		}
	}

	void DemoKeeper::decreaseFontHeight(MyGUI::Widget* _sender)
	{
		for (auto* edit : mEditBoxes)
		{
			edit->setFontHeight(edit->getFontHeight() - 1);
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
