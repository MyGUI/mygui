/*!
	@file
	@author		Albert Semenov
	@date		04/2008
*/

#include "MyGUI_Ogre21DataManager.h"
#include "MyGUI_Ogre21RenderManager.h"
#include "MyGUI_Ogre21Texture.h"
#include "MyGUI_Ogre21VertexBuffer.h"
#include "MyGUI_Ogre21Diagnostic.h"
#include "MyGUI_Timer.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{
	const Ogre::uint8 Ogre21RenderManager::RENDER_QUEUE_OVERLAY = 254;

	Ogre21RenderManager& Ogre21RenderManager::getInstance()
	{
		return *getInstancePtr();
	}
	Ogre21RenderManager* Ogre21RenderManager::getInstancePtr()
	{
		return static_cast<Ogre21RenderManager*>(RenderManager::getInstancePtr());
	}

	Ogre21RenderManager::Ogre21RenderManager() :
		mUpdate(false),
		mSceneManager(nullptr),
		mWindow(nullptr),
		mActiveViewport(0),
		mRenderSystem(nullptr),
		mIsInitialise(false),
		mManualRender(false),
		mCountBatch(0)
	{
	}

	void Ogre21RenderManager::initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mSceneManager = nullptr;
		mWindow = nullptr;
		mUpdate = false;
		mRenderSystem = nullptr;
		mActiveViewport = 0;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
			setRenderSystem(root->getRenderSystem());
		setRenderWindow(_window);
		setSceneManager(_scene);

		mQueueMoveable = new Ogre21GuiMoveable(_scene);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void Ogre21RenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		setSceneManager(nullptr);
		setRenderWindow(nullptr);
		setRenderSystem(nullptr);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void Ogre21RenderManager::setRenderSystem(Ogre::RenderSystem* _render)
	{
		// отписываемся
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->removeListener(this);
			mRenderSystem = nullptr;
		}

		mRenderSystem = _render;

		// подписываемся на рендер евент
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->addListener(this);

			// формат цвета в вершинах
			Ogre::VertexElementType vertex_type = mRenderSystem->getColourVertexElementType();
			if (vertex_type == Ogre::VET_COLOUR_ARGB)
				mVertexFormat = VertexColourType::ColourARGB;
			else if (vertex_type == Ogre::VET_COLOUR_ABGR)
				mVertexFormat = VertexColourType::ColourABGR;

			updateRenderInfo();
		}
	}

	Ogre::RenderSystem* Ogre21RenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	void Ogre21RenderManager::setRenderWindow(Ogre::RenderWindow* _window)
	{
		// отписываемся
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			mWindow = nullptr;
		}

		mWindow = _window;

		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

			if (mWindow->getNumViewports() <= mActiveViewport &&
				!mWindow->getViewport(mActiveViewport)->getOverlaysEnabled())
			{
				MYGUI_PLATFORM_LOG(Warning, "Overlays are disabled. MyGUI won't render in selected viewport.");
			}

			windowResized(mWindow);
		}
	}

	void Ogre21RenderManager::setSceneManager(Ogre::SceneManager* _scene)
	{
		if (nullptr != mSceneManager)
		{
			mSceneManager->removeRenderQueueListener(this);
			mSceneManager = nullptr;
		}

		mSceneManager = _scene;

		if (nullptr != mSceneManager)
		{
			mSceneManager->addRenderQueueListener(this);
		}
	}

	Ogre::SceneManager* Ogre21RenderManager::getSceneManager()
	{
		return mSceneManager;
	}

	void Ogre21RenderManager::setActiveViewport(unsigned short _num)
	{
		mActiveViewport = _num;

		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

			if (mWindow->getNumViewports() <= mActiveViewport)
			{
				MYGUI_PLATFORM_LOG(Error, "Invalid active viewport index selected. There is no viewport with given index.");
			}

			// рассылка обновлений
			windowResized(mWindow);
		}
	}

	void Ogre21RenderManager::renderQueueStarted( Ogre::RenderQueue *rq, Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation )
	{

		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		if (RENDER_QUEUE_OVERLAY != queueGroupId)
			return;

		mCountBatch = 0;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		//begin();
		onRenderToTarget(this, mUpdate);
		//end();

		// сбрасываем флаг
		mUpdate = false;
	}

	void Ogre21RenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
	{
		if (eventName == "DeviceLost")
		{
		}
		else if (eventName == "DeviceRestored")
		{
			// обновить всех
			mUpdate = true;
		}
	}

	IVertexBuffer* Ogre21RenderManager::createVertexBuffer()
	{
		return new Ogre21VertexBuffer();
	}

	void Ogre21RenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	// для оповещений об изменении окна рендера
	void Ogre21RenderManager::windowResized(Ogre::RenderWindow* _window)
	{

		mViewSize.set(_window->getWidth(), _window->getHeight());

		// обновить всех
		mUpdate = true;

		updateRenderInfo();

		onResizeView(mViewSize);
	}

	void Ogre21RenderManager::updateRenderInfo()
	{
		if (mRenderSystem != nullptr)
		{
			mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
			mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
			mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
			mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
			mInfo.pixScaleX = 1.0f / float(mViewSize.width);
			mInfo.pixScaleY = 1.0f / float(mViewSize.height);
		}
	}

	void Ogre21RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{

		Ogre21GuiRenderable* renderable = createOrRetrieveRenderable(_buffer, _texture, _count);

		mSceneManager->getRenderQueue()->addRenderableV1(RENDER_QUEUE_OVERLAY, false, renderable , mQueueMoveable);

		++ mCountBatch;
	}

	void Ogre21RenderManager::begin()
	{
	}

	void Ogre21RenderManager::end()
	{
	}

	ITexture* Ogre21RenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		Ogre21Texture* texture = new Ogre21Texture(_name, Ogre21DataManager::getInstance().getGroup());
		mTextures[_name] = texture;
		return texture;
	}

	void Ogre21RenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr) return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* Ogre21RenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
		{
			Ogre::TexturePtr texture = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(_name);
			if (!texture.isNull())
			{
				ITexture* result = createTexture(_name);
				static_cast<Ogre21Texture*>(result)->setOgreTexture(texture);
				return result;
			}
			return nullptr;
		}
		return item->second;
	}

	bool Ogre21RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return Ogre::TextureManager::getSingleton().isFormatSupported(
			Ogre::TEX_TYPE_2D,
			Ogre21Texture::convertFormat(_format),
			Ogre21Texture::convertUsage(_usage));
	}

	void Ogre21RenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

#if MYGUI_DEBUG_MODE == 1
	bool Ogre21RenderManager::checkTexture(ITexture* _texture)
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			if (item->second == _texture)
				return true;
		}
		return false;
	}
#endif

	const IntSize& Ogre21RenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType Ogre21RenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	const RenderTargetInfo& Ogre21RenderManager::getInfo()
	{
		return mInfo;
	}

	size_t Ogre21RenderManager::getActiveViewport()
	{
		return mActiveViewport;
	}

	Ogre::RenderWindow* Ogre21RenderManager::getRenderWindow()
	{
		return mWindow;
	}

	bool Ogre21RenderManager::getManualRender()
	{
		return mManualRender;
	}

	void Ogre21RenderManager::setManualRender(bool _value)
	{
		mManualRender = _value;
	}

	size_t Ogre21RenderManager::getBatchCount() const
	{
		return mCountBatch;
	}

	Ogre21GuiRenderable* Ogre21RenderManager::createOrRetrieveRenderable( IVertexBuffer* _buffer , ITexture* _texture , size_t _count )
	{

		Ogre21VertexBuffer* vertexBuffer = static_cast<Ogre21VertexBuffer*>(_buffer);
		Ogre21Texture* texture = static_cast<Ogre21Texture*>(_texture);

		const unsigned int ID = vertexBuffer->getUniqueID();

		Ogre21GuiRenderable* renderable;

		MapRenderable::const_iterator item = mRenderables.find(ID);
		if (item == mRenderables.end())
		{
			mRenderables[ID] = new Ogre21GuiRenderable();
		}

		renderable = mRenderables[ID];

		vertexBuffer->setOperationVertexCount(_count);

		renderable->setRenderOperation(*vertexBuffer->getRenderOperation());
		renderable->setDatablock(texture->getDataBlock());

		return renderable;
	}

} // namespace MyGUI
