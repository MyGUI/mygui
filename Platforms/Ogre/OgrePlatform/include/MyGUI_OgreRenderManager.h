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

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreRenderManager :
		public RenderManager,
		public IRenderTarget,
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener
	{
	public:
		OgreRenderManager();

		void initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene);
		void shutdown();

		static OgreRenderManager& getInstance();
		static OgreRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const;

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat();

		/** @see RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);

		/** @see RenderManager::isFormatSupported */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();

		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo();

		void setRenderSystem(Ogre::RenderSystem* _render);
		Ogre::RenderSystem* getRenderSystem();

		void setRenderWindow(Ogre::RenderWindow* _window);

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager* _scene);

		/** Get GUI viewport index */
		size_t getActiveViewport();

		/** Set GUI viewport index */
		void setActiveViewport(unsigned short _num);

		Ogre::RenderWindow* getRenderWindow();

		bool getManualRender();
		void setManualRender(bool _value);

		size_t getBatchCount() const;

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

#if MYGUI_DEBUG_MODE == 1
		virtual bool checkTexture(ITexture* _texture);
#endif

		std::string getShaderExtension() const;

	/*internal:*/
		/* for use with RTT, flips Y coordinate if necessary when rendering */
		void doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, bool flipY);

	private:
		virtual void renderQueueStarted(
			Ogre::uint8 queueGroupId,
			const Ogre::String& invocation,
			bool& skipThisInvocation);
		virtual void renderQueueEnded(
			Ogre::uint8 queueGroupId,
			const Ogre::String& invocation,
			bool& repeatThisInvocation);
		virtual void windowResized(Ogre::RenderWindow* _window);

		// restore buffers
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters);

		void destroyAllResources();
		void updateRenderInfo();

	private:
		// флаг для обновления всех и вся
		bool mUpdate;

		IntSize mViewSize;

		Ogre::SceneManager* mSceneManager;

		VertexColourType mVertexFormat;

		// окно, на которое мы подписываемся для изменения размеров
		Ogre::RenderWindow* mWindow;

		// вьюпорт, с которым работает система
		unsigned short mActiveViewport;

		Ogre::RenderSystem* mRenderSystem;
		RenderTargetInfo mInfo;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;

		bool mIsInitialise;
		bool mManualRender;
		size_t mCountBatch;

		struct DummyRenderable : public Ogre::Renderable
		{
			DummyRenderable() {
				mUseIdentityProjection = true;
				mUseIdentityView = true;
			}

			Ogre::RenderOperation mRenderOp;
			Ogre::MaterialPtr mMaterial;

			void getWorldTransforms(Ogre::Matrix4* xform) const { *xform = Ogre::Matrix4::IDENTITY; }
			void getRenderOperation(Ogre::RenderOperation& op) { op = mRenderOp; }
			const Ogre::MaterialPtr& getMaterial() const { return mMaterial; }
			const Ogre::LightList& getLights(void) const { static Ogre::LightList ll; return ll; }
			Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const { return 0; }
		} mRenderable;

	};

} // namespace MyGUI

#endif // MYGUI_OGRE_RENDER_MANAGER_H_
