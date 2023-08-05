/*!
	@file
	@author		Albert Semenov
	@date		04/2008
*/

#ifndef MYGUI_OGRE_RENDER_MANAGER_H_
#define MYGUI_OGRE_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include <OgrePrerequisites.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderSystem.h>

namespace MyGUI
{

	struct OgreShaderInfo
	{
		Ogre::HighLevelGpuProgramPtr vertexProgram;
		Ogre::HighLevelGpuProgramPtr fragmentProgram;
	};

	class OgreRenderManager :
		public RenderManager,
		public IRenderTarget,
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener
	{
	public:
		void initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene);
		void shutdown();

		static OgreRenderManager& getInstance();
		static OgreRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override;

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override;

		/** @see RenderManager::createVertexBuffer */
		IVertexBuffer* createVertexBuffer() override;
		/** @see RenderManager::destroyVertexBuffer */
		void destroyVertexBuffer(IVertexBuffer* _buffer) override;

		/** @see RenderManager::createTexture */
		ITexture* createTexture(const std::string& _name) override;
		/** @see RenderManager::destroyTexture */
		void destroyTexture(ITexture* _texture) override;
		/** @see RenderManager::getTexture */
		ITexture* getTexture(const std::string& _name) override;

		/** @see RenderManager::isFormatSupported */
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;

		/** @see IRenderTarget::begin */
		void begin() override;
		/** @see IRenderTarget::end */
		void end() override;

		/** @see IRenderTarget::doRender */
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		/** @see IRenderTarget::getInfo */
		const RenderTargetInfo& getInfo() const override;

		void setRenderSystem(Ogre::RenderSystem* _render);
		Ogre::RenderSystem* getRenderSystem() const;

		void setRenderWindow(Ogre::RenderWindow* _window);

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager* _scene);

		/** Get GUI viewport index */
		size_t getActiveViewport() const;

		/** Set GUI viewport index */
		void setActiveViewport(unsigned short _num);

		Ogre::RenderWindow* getRenderWindow() const;

		bool getManualRender() const;
		void setManualRender(bool _value);

		size_t getBatchCount() const;

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		bool checkTexture(ITexture* _texture) override;

		std::string getShaderExtension() const;

		/*internal:*/
		/* for use with RTT */
		void beginRttRender(bool isFlippedTexture);
		void endRttRender();
		void doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, Ogre::RenderTexture* rtt);
		OgreShaderInfo* getShaderInfo(const std::string& _shaderName) const;

	private:
		void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
			override;
		void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
			override;
		virtual void windowResized(Ogre::RenderWindow* _window);

		// restore buffers
		void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters) override;

		void destroyAllResources();
		void updateRenderInfo();

		OgreShaderInfo* createShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile);

		void setShaderProjectionMatrix(bool isFlipped);

	private:
		// флаг для обновления всех и вся
		bool mUpdate{false};

		IntSize mViewSize;

		Ogre::SceneManager* mSceneManager{nullptr};

		VertexColourType mVertexFormat;

		// окно, на которое мы подписываемся для изменения размеров
		Ogre::RenderWindow* mWindow{nullptr};

		// вьюпорт, с которым работает система
		unsigned short mActiveViewport{0};

		Ogre::RenderSystem* mRenderSystem{nullptr};
		RenderTargetInfo mInfo;

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;

		bool mIsInitialise{false};
		bool mManualRender{false};
		size_t mCountBatch{0};

		OgreShaderInfo* mDefaultShader = nullptr;
		std::map<std::string, OgreShaderInfo*> mRegisteredShaders;

		Ogre::MaterialPtr mMaterial;
		Ogre::Pass* mPass;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_RENDER_MANAGER_H_
