#include "MyGUI_OgreNextRenderManager.h"

#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextTexture.h"
#include "MyGUI_OgreNextVertexBuffer.h"
#include "MyGUI_OgreNextCompositorPassMyGUIProvider.h"
#include "MyGUI_OgreNextDataManager.h"
#include "MyGUI_Timer.h"
#include "MyGUI_Gui.h"

#include <Compositor/OgreCompositorManager2.h>
#include <OgreRoot.h>
#include <OgreWindow.h>
#include <OgreSceneManager.h>
#include <OgreTextureGpu.h>
#include <OgreTextureGpuManager.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreHighLevelGpuProgram.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	OgreNextRenderManager& OgreNextRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OgreNextRenderManager* OgreNextRenderManager::getInstancePtr()
	{
		return static_cast<OgreNextRenderManager*>(RenderManager::getInstancePtr());
	}

	OgreNextRenderManager::OgreNextRenderManager() = default;

	void OgreNextRenderManager::initialise(Ogre::SceneManager* scene)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mSceneManager = scene;
		MYGUI_PLATFORM_ASSERT(mSceneManager != nullptr, "SceneManager must be non-null");

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		MYGUI_PLATFORM_ASSERT(root != nullptr, "Ogre::Root must exist");

		setRenderSystem(root->getRenderSystem());
		root->addFrameListener(this);

		mManager = std::make_unique<OgreNextManager>(mSceneManager);

		installPassProvider();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreNextRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		for (auto& item : mTextures)
			delete item.second;
		mTextures.clear();

		uninstallPassProvider();

		mManager.reset();

		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			mWindow = nullptr;
		}

		if (mRenderSystem != nullptr)
		{
			mRenderSystem->removeListener(this);
			mRenderSystem = nullptr;
		}

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
			root->removeFrameListener(this);

		mSceneManager = nullptr;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void OgreNextRenderManager::installPassProvider()
	{
		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root == nullptr)
			return;

		auto* cm = root->getCompositorManager2();
		if (cm == nullptr)
			return;

		if (cm->getCompositorPassProvider() != nullptr)
		{
			MYGUI_PLATFORM_LOG(
				Warning,
				"A CompositorPassProvider is already installed. MyGUI passes will not resolve unless "
				"the existing provider forwards to MyGUI. Use OgreNextRenderManager::uninstallPassProvider "
				"before initialising if you want to swap it manually.");
			return;
		}

		mPassProvider = std::make_unique<CompositorPassMyGUIProvider>();
		cm->setCompositorPassProvider(mPassProvider.get());
	}

	void OgreNextRenderManager::uninstallPassProvider()
	{
		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root == nullptr || !mPassProvider)
			return;

		auto* cm = root->getCompositorManager2();
		if (cm != nullptr && cm->getCompositorPassProvider() == mPassProvider.get())
			cm->setCompositorPassProvider(nullptr);

		mPassProvider.reset();
	}

	void OgreNextRenderManager::attachToWindow(Ogre::Window* window)
	{
		if (mWindow != nullptr)
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

		mWindow = window;

		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
			windowResized(mWindow);
		}
	}

	void OgreNextRenderManager::setRenderSystem(Ogre::RenderSystem* rs)
	{
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->removeListener(this);
			mRenderSystem = nullptr;
		}

		mRenderSystem = rs;

		if (mRenderSystem != nullptr)
		{
			mRenderSystem->addListener(this);

			const Ogre::VertexElementType t = mRenderSystem->getColourVertexElementType();
			if (t == Ogre::VET_COLOUR_ABGR)
				mVertexFormat = VertexColourType::ColourABGR;
			else if (t == Ogre::VET_COLOUR_ARGB)
				mVertexFormat = VertexColourType::ColourARGB;

			updateRenderInfo();
		}
	}

	void OgreNextRenderManager::renderIntoCurrentBatch()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		onRenderToTarget(this, mUpdate);
		mUpdate = false;
	}

	bool OgreNextRenderManager::frameStarted(const Ogre::FrameEvent& /*evt*/)
	{
		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		const unsigned long now_time = timer.getMilliseconds();
		const unsigned long delta = now_time - last_time;
		last_time = now_time;

		onFrameEvent(static_cast<float>(static_cast<double>(delta) / 1000.0));
		return true;
	}

	void OgreNextRenderManager::eventOccurred(
		const Ogre::String& eventName,
		const Ogre::NameValuePairList* /*parameters*/)
	{
		if (eventName == "DeviceRestored")
			mUpdate = true;
	}

	void OgreNextRenderManager::windowResized(Ogre::Window* window)
	{
		if (window == nullptr)
			return;

		mViewSize.set(static_cast<int>(window->getWidth()), static_cast<int>(window->getHeight()));
		mUpdate = true;

		updateRenderInfo();

		onResizeView(mViewSize);
	}

	void OgreNextRenderManager::windowClosed(Ogre::Window* window)
	{
		if (window == mWindow)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			mWindow = nullptr;
		}
	}

	void OgreNextRenderManager::updateRenderInfo()
	{
		if (mRenderSystem == nullptr || mViewSize.width == 0 || mViewSize.height == 0)
			return;

		mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
		mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
		mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);
	}

	// ---- MyGUI::RenderManager ----

	IVertexBuffer* OgreNextRenderManager::createVertexBuffer()
	{
		return new OgreNextVertexBuffer();
	}

	void OgreNextRenderManager::destroyVertexBuffer(IVertexBuffer* buffer)
	{
		delete buffer;
	}

	ITexture* OgreNextRenderManager::createTexture(const std::string& name)
	{
		MYGUI_PLATFORM_ASSERT(mTextures.find(name) == mTextures.end(), "Texture '" << name << "' already exists");
		auto* texture = new OgreNextTexture(name, OgreNextDataManager::getInstance().getGroup());
		mTextures[name] = texture;
		return texture;
	}

	void OgreNextRenderManager::destroyTexture(ITexture* texture)
	{
		if (texture == nullptr)
			return;

		auto it = mTextures.find(texture->getName());
		MYGUI_PLATFORM_ASSERT(it != mTextures.end(), "Texture '" << texture->getName() << "' not found");

		auto* ogreTex = static_cast<OgreNextTexture*>(texture)->getOgreTexture();
		if (ogreTex != nullptr && mManager)
			mManager->notifyTextureDestroyed(ogreTex);

		mTextures.erase(it);
		delete texture;
	}

	ITexture* OgreNextRenderManager::getTexture(const std::string& name)
	{
		auto it = mTextures.find(name);
		if (it != mTextures.end())
			return it->second;

		Ogre::TextureGpuManager* tm = mRenderSystem != nullptr ? mRenderSystem->getTextureGpuManager() : nullptr;
		if (tm == nullptr)
			return nullptr;

		if (tm->findResourceNameStr(name) == nullptr && tm->findTextureNoThrow(name) == nullptr)
			return nullptr;

		Ogre::TextureGpu* existing = tm->createOrRetrieveTexture(
			name,
			Ogre::GpuPageOutStrategy::Discard,
			0u,
			Ogre::TextureTypes::Type2D,
			Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		if (existing == nullptr)
			return nullptr;

		auto* wrapper = static_cast<OgreNextTexture*>(createTexture(name));
		wrapper->setOgreTexture(existing);
		return wrapper;
	}

	bool OgreNextRenderManager::isFormatSupported(PixelFormat format, TextureUsage /*usage*/)
	{
		if (format == PixelFormat::L8A8)
			return false; // Requires fragment-shader swizzle for RG8 -> RGBA. Not implemented yet.
		return true;
	}

	bool OgreNextRenderManager::checkTexture(ITexture* texture)
	{
		for (const auto& item : mTextures)
			if (item.second == texture)
				return true;
		return false;
	}

	const IntSize& OgreNextRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	void OgreNextRenderManager::setViewSize(int width, int height)
	{
		mViewSize.set(width, height);
		mUpdate = true;
		updateRenderInfo();
		onResizeView(mViewSize);
	}

	std::string OgreNextRenderManager::getShaderExtension() const
	{
		auto& mgr = Ogre::HighLevelGpuProgramManager::getSingleton();
		if (mgr.isLanguageSupported("hlsl"))
			return "hlsl";
		if (mgr.isLanguageSupported("glsl"))
			return "glsl";
		if (mgr.isLanguageSupported("glslvk"))
			return "glslvk";
		if (mgr.isLanguageSupported("metal"))
			return "metal";
		MYGUI_PLATFORM_EXCEPT("No supported shader language found (need one of hlsl/glsl/glslvk/metal)");
		return {};
	}

	const OgreNextRenderManager::ShaderEntry* OgreNextRenderManager::findRegisteredShader(const std::string& name) const
	{
		auto it = mRegisteredShaders.find(name);
		return it == mRegisteredShaders.end() ? nullptr : &it->second;
	}

	namespace
	{
		std::string detectShaderLanguage(const std::string& filename)
		{
			const auto dot = filename.find_last_of('.');
			if (dot == std::string::npos)
				return {};
			const std::string ext = filename.substr(dot + 1);
			if (ext == "hlsl")
				return "hlsl";
			if (ext == "glsl")
				return "glsl";
			if (ext == "glslvk")
				return "glslvk";
			if (ext == "metal")
				return "metal";
			return {};
		}

		Ogre::HighLevelGpuProgramPtr loadShaderFile(
			const std::string& filename,
			Ogre::GpuProgramType type,
			const std::string& group)
		{
			auto& mgr = Ogre::HighLevelGpuProgramManager::getSingleton();

			// Allow referencing an already-registered program by name (e.g. our
			// built-in "mygui/VP" / "mygui/FP", or one registered by another call
			// to registerShader). Search the caller's group first, then anywhere.
			Ogre::HighLevelGpuProgramPtr existing = mgr.getByName(filename, group);
			if (!existing)
				existing = mgr.getByName(filename, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
			if (existing)
				return existing;

			const std::string language = detectShaderLanguage(filename);
			MYGUI_PLATFORM_ASSERT(
				!language.empty(),
				"registerShader: cannot detect shader language from filename '" << filename << "'");

			Ogre::HighLevelGpuProgramPtr program = mgr.createProgram(filename, group, language, type);
			program->setSourceFile(filename);
			if (language == "hlsl")
			{
				program->setParameter("target", type == Ogre::GPT_VERTEX_PROGRAM ? "vs_5_0 vs_4_0" : "ps_5_0 ps_4_0");
				program->setParameter("entry_point", "main");
			}
			program->load();
			return program;
		}
	}

	void OgreNextRenderManager::registerShader(
		const std::string& shaderName,
		const std::string& vertexProgramFile,
		const std::string& fragmentProgramFile)
	{
		MYGUI_PLATFORM_LOG(
			Info,
			"* OgreNextRenderManager::registerShader('"
				<< shaderName << "', '" << vertexProgramFile << "', '" << fragmentProgramFile << "')");

		const std::string& group = OgreNextDataManager::getInstance().getGroup();
		auto vp = loadShaderFile(vertexProgramFile, Ogre::GPT_VERTEX_PROGRAM, group);
		auto fp = loadShaderFile(fragmentProgramFile, Ogre::GPT_FRAGMENT_PROGRAM, group);

		ShaderEntry entry;
		entry.vertexProgram = vp->getName();
		entry.fragmentProgram = fp->getName();
		mRegisteredShaders[shaderName] = entry;
	}

	VertexColourType OgreNextRenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	const RenderTargetInfo& OgreNextRenderManager::getInfo() const
	{
		return mInfo;
	}

	// ---- IRenderTarget: begin/end are no-ops in the compositor-pass model. ----

	void OgreNextRenderManager::begin()
	{
	}

	void OgreNextRenderManager::end()
	{
	}

	void OgreNextRenderManager::doRender(IVertexBuffer* buffer, ITexture* texture, size_t count)
	{
		if (mManager)
			mManager->submitDraw(buffer, texture, count);
	}

	size_t OgreNextRenderManager::getBatchCount() const
	{
		return mManager ? mManager->getDrawCount() : 0u;
	}

} // namespace MyGUI
