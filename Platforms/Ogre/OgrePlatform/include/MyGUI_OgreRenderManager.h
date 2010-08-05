/*!
	@file
	@author		Albert Semenov
	@date		04/2008
*/

#ifndef __MYGUI_OGRE_RENDER_MANAGER_H__
#define __MYGUI_OGRE_RENDER_MANAGER_H__

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
		public Ogre::WindowEventListener,
		public Ogre::RenderQueueListener,
		public Ogre::RenderSystem::Listener
	{
	public:
		OgreRenderManager();

		void initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene);
		void shutdown();

		static OgreRenderManager& getInstance() { return *getInstancePtr(); }
		static OgreRenderManager* getInstancePtr() { return static_cast<OgreRenderManager*>(RenderManager::getInstancePtr()); }

		/** @see RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const { return mViewSize; }

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat() { return mVertexFormat; }

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
		virtual const RenderTargetInfo& getInfo() { return mInfo; }

		void setRenderSystem(Ogre::RenderSystem* _render);
		Ogre::RenderSystem* getRenderSystem();

		void setRenderWindow(Ogre::RenderWindow* _window);

		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager* _scene);

		/** Get GUI viewport index */
		size_t getActiveViewport() { return mActiveViewport; }

		/** Set GUI viewport index */
		void setActiveViewport(size_t _num);

		Ogre::RenderWindow * getRenderWindow() { return mWindow; }

		void setTextureFilter(Ogre::FilterOptions _value);

#if MYGUI_DEBUG_MODE == 1
		virtual bool checkTexture(ITexture* _texture);
#endif

	private:
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
		virtual void windowResized(Ogre::RenderWindow* _window);

		// восстанавливаем буферы
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
		size_t mActiveViewport;

		Ogre::RenderSystem* mRenderSystem;
		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode;
		Ogre::LayerBlendModeEx mColorBlendMode, mAlphaBlendMode;

		RenderTargetInfo mInfo;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;

		bool mIsInitialise;
		Ogre::FilterOptions mTextureFilter;
		ITexture* mTexture;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_RENDER_MANAGER_H__
