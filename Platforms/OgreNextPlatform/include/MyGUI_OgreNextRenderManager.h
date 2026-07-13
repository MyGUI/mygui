#ifndef MYGUI_OGRE_NEXT_RENDER_MANAGER_H_
#define MYGUI_OGRE_NEXT_RENDER_MANAGER_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextCompositorPassMyGUIProvider.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_IRenderTarget.h"

#include <OgreFrameListener.h>
#include <OgreRenderSystem.h>
#include <OgreWindowEventUtilities.h>

#include "MyGUI_LastHeader.h"

#include <map>
#include <memory>

namespace MyGUI
{
	class OgreNextRenderManager :
		public RenderManager,
		public IRenderTarget,
		public Ogre::WindowEventListener,
		public Ogre::RenderSystem::Listener,
		public Ogre::FrameListener
	{
	public:
		OgreNextRenderManager();

		static OgreNextRenderManager& getInstance();
		static OgreNextRenderManager* getInstancePtr();

		// Full initialise: installs shaders, creates the OgreNextManager,
		// and installs the compositor pass provider.
		void initialise(Ogre::SceneManager* scene);
		void shutdown();

		// If the caller wants to control the compositor themselves,
		// they can attach and detach the pass provider explicitly.
		void installPassProvider();
		void uninstallPassProvider();

		// Called from CompositorPassMyGUI::execute after the batch has begun.
		// Iterates MyGUI layers via onRenderToTarget which triggers doRender.
		void renderIntoCurrentBatch();

		OgreNextManager* getManager() const
		{
			return mManager.get();
		}

		Ogre::SceneManager* getSceneManager() const
		{
			return mSceneManager;
		}

		// Returns the file extension of the shader language supported by the
		// current render system: "hlsl", "glsl", "glslvk", or "metal".
		std::string getShaderExtension() const;

		// Looks up a shader registered via registerShader. Returns the pair of
		// (vertex program name, fragment program name) or an empty pair if the
		// name is not registered (or is the reserved built-in "Default").
		struct ShaderEntry
		{
			std::string vertexProgram;
			std::string fragmentProgram;
		};
		const ShaderEntry* findRegisteredShader(const std::string& name) const;

		// MyGUI::RenderManager
		const IntSize& getViewSize() const override;
		void setViewSize(int _width, int _height) override;
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;
		VertexColourType getVertexFormat() const override;
		IVertexBuffer* createVertexBuffer() override;
		void destroyVertexBuffer(IVertexBuffer* _buffer) override;
		ITexture* createTexture(const std::string& _name) override;
		void destroyTexture(ITexture* _texture) override;
		ITexture* getTexture(const std::string& _name) override;
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;
		bool checkTexture(ITexture* _texture) override;

		// MyGUI::IRenderTarget
		void begin() override;
		void end() override;
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		const RenderTargetInfo& getInfo() const override;

		// Deferred sampler restore for doManualRender (forwards to OgreNextManager).
		void deferSamplerRestore(Ogre::TextureUnitState* tu, const Ogre::HlmsSamplerblock* original);

		// Ogre::FrameListener
		bool frameStarted(const Ogre::FrameEvent& evt) override;

		// Ogre::RenderSystem::Listener
		void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters) override;

		// Ogre::WindowEventListener
		void windowResized(Ogre::Window* window) override;
		void windowMoved(Ogre::Window*) override
		{
		}
		bool windowClosing(Ogre::Window*) override
		{
			return true;
		}
		void windowClosed(Ogre::Window* window) override;
		void windowFocusChange(Ogre::Window*) override
		{
		}

		size_t getBatchCount() const;

		// Convenience: attaches to a window so we track resize events and
		// forward view size into MyGUI.
		void attachToWindow(Ogre::Window* window);
		Ogre::Window* getWindow() const
		{
			return mWindow;
		}

	private:
		void setRenderSystem(Ogre::RenderSystem* rs);
		void updateRenderInfo();

		std::unique_ptr<OgreNextManager> mManager;

		Ogre::SceneManager* mSceneManager{nullptr};
		Ogre::Window* mWindow{nullptr};
		Ogre::RenderSystem* mRenderSystem{nullptr};

		std::unique_ptr<CompositorPassMyGUIProvider> mPassProvider;

		IntSize mViewSize;
		RenderTargetInfo mInfo;
		VertexColourType mVertexFormat{VertexColourType::ColourABGR};

		bool mIsInitialise{false};
		bool mUpdate{true};

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;

		using ShaderMap = std::map<std::string, ShaderEntry>;
		ShaderMap mRegisteredShaders;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_RENDER_MANAGER_H_
