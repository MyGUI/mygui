#include "PlatformFixture.h"

namespace platformtest
{

	std::string_view backendName()
	{
#if defined(MYGUI_OPENGL_PLATFORM)
		return "OpenGL";
#elif defined(MYGUI_OPENGL3_PLATFORM)
		return "OpenGL3";
#elif defined(MYGUI_OPENGLES_PLATFORM)
		return "OpenGLES";
#elif defined(MYGUI_OSG_PLATFORM)
		return "Osg";
#elif defined(MYGUI_DIRECTX_PLATFORM)
		return "DirectX";
#elif defined(MYGUI_DIRECTX11_PLATFORM)
		return "DirectX11";
#elif defined(MYGUI_VULKAN_PLATFORM)
		return "Vulkan";
#elif defined(MYGUI_OGRE_PLATFORM)
		return "Ogre";
#elif defined(MYGUI_OGRENEXT_PLATFORM)
		return "OgreNext";
#else
	#error Unsupported platform test backend
#endif
	}

	// Only shader source selection varies by backend; the pixel assertions and
	// registration/selection sequence use the shared rendering interfaces.
	std::pair<std::string, std::string> shaderFiles()
	{
#if defined(MYGUI_OPENGL3_PLATFORM)
		return {"MyGUI_OpenGL3_VP.glsl", "PlatformSwap_FP.glsl"};
#elif defined(MYGUI_OSG_PLATFORM)
		return {"MyGUI_Osg_VP.glsl", "PlatformSwap_FP.glsl"};
#elif defined(MYGUI_OPENGLES_PLATFORM)
		return {"MyGUI_OpenGLES_VP.glsl", "PlatformSwap_GLES_FP.glsl"};
#elif defined(MYGUI_DIRECTX11_PLATFORM)
		return {"MyGUI_DirectX11_VP.hlsl", "PlatformSwap_DirectX11_FP.hlsl"};
#elif defined(MYGUI_VULKAN_PLATFORM)
		return {"MyGUI_Vulkan_VP.spv", "PlatformSwap_Vulkan_FP.spv"};
#elif defined(MYGUI_OGRE_PLATFORM)
		const auto extension = MyGUI::OgreRenderManager::getInstance().getShaderExtension(true);
		if (extension == "glsl" || extension == "hlsl")
			return {"MyGUI_Ogre_VP." + extension, "PlatformSwap_Ogre_FP." + extension};
		return {};
#elif defined(MYGUI_OGRENEXT_PLATFORM)
		const auto extension = MyGUI::OgreNextRenderManager::getInstance().getShaderExtension();
		if (extension == "glsl" || extension == "hlsl" || extension == "metal")
			return {"mygui/VP", "PlatformSwap_OgreNext_FP." + extension};
		return {};
#else
		return {};
#endif
	}

}
