/*!
	@file
	@author		Albert Semenov
	@date		04/2008
*/

#include "MyGUI_Ogre2DataManager.h"
#include "MyGUI_Ogre2RenderManager.h"
#include "MyGUI_Ogre2Texture.h"
#include "MyGUI_Ogre2VertexBuffer.h"
#include "MyGUI_Ogre2Diagnostic.h"
#include "MyGUI_Timer.h"
#include "MyGUI_Gui.h"

#include <Compositor/OgreCompositorManager2.h>

namespace MyGUI
{
	const Ogre::uint8 Ogre2RenderManager::RENDER_QUEUE_OVERLAY = 254;
	Ogre::IdString OgreCompositorPassProvider::mPassId = Ogre::IdString("MYGUI");

	MyGUIPass::MyGUIPass(const Ogre::CompositorPassDef *definition, const Ogre::CompositorChannel &target,
		Ogre::CompositorNode *parentNode)
		: Ogre::CompositorPass(definition, target, parentNode)
	{

	}

	void MyGUIPass::execute(const Ogre::Camera *lodCameraconst)
	{
		static_cast<MyGUI::Ogre2RenderManager*>(MyGUI::RenderManager::getInstancePtr())->render();
	}

	Ogre2RenderManager& Ogre2RenderManager::getInstance()
	{
		return *getInstancePtr();
	}
	Ogre2RenderManager* Ogre2RenderManager::getInstancePtr()
	{
		return static_cast<Ogre2RenderManager*>(RenderManager::getInstancePtr());
	}

	Ogre2RenderManager::Ogre2RenderManager() :
		mUpdate(false),
		mSceneManager(nullptr),
		mWindow(nullptr),
		mRenderSystem(nullptr),
		mIsInitialise(false),
		mManualRender(false),
		mCountBatch(0)
	{
	}

	void Ogre2RenderManager::initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mPassProvider.reset(new OgreCompositorPassProvider());

		Ogre::CompositorManager2* pCompositorManager = Ogre::Root::getSingleton().getCompositorManager2();

		// don't overwrite a custom pass provider that the user may have registered already
		if (!pCompositorManager->getCompositorPassProvider())
		{
			pCompositorManager->setCompositorPassProvider(mPassProvider.get());
		}
		else
		{
			MYGUI_PLATFORM_LOG(Warning, "A custom pass provider is already installed." <<
										"MyGui passes will not work unless the registered provider can create MyGui passes");
		}

		mSceneManager = nullptr;
		mWindow = nullptr;
		mUpdate = false;
		mRenderSystem = nullptr;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
			setRenderSystem(root->getRenderSystem());
		setRenderWindow(_window);
		setSceneManager(_scene);

		root->addFrameListener(this);

		mQueueMoveable = new Ogre2GuiMoveable(Ogre::Id::generateNewId<Ogre2GuiRenderable>(), &mDummyObjMemMgr, mSceneManager, RENDER_QUEUE_OVERLAY);
		mQueueSceneNode = new Ogre::SceneNode(0, 0, &mDummyNodeMemMgr, 0);
		mQueueSceneNode->_getFullTransformUpdated();
		mQueueSceneNode->attachObject(mQueueMoveable);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void Ogre2RenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		delete mQueueSceneNode;
		delete mQueueMoveable;

		setSceneManager(nullptr);
		setRenderWindow(nullptr);
		setRenderSystem(nullptr);

		Ogre::Root::getSingleton().removeFrameListener(this);

		Ogre::CompositorManager2* pCompositorManager = Ogre::Root::getSingleton().getCompositorManager2();
		if (pCompositorManager->getCompositorPassProvider() == mPassProvider.get())
			pCompositorManager->setCompositorPassProvider(NULL);

		mPassProvider.reset();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void Ogre2RenderManager::setRenderSystem(Ogre::RenderSystem* _render)
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

	Ogre::RenderSystem* Ogre2RenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	void Ogre2RenderManager::setRenderWindow(Ogre::RenderWindow* _window)
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
			windowResized(mWindow);
		}
	}

	void Ogre2RenderManager::setSceneManager(Ogre::SceneManager* _scene)
	{
		if (nullptr != mSceneManager)
		{
			mSceneManager = nullptr;
		}

		mSceneManager = _scene;
		if (mSceneManager != nullptr)
		{
			mSceneManager->getRenderQueue()->setSortRenderQueue(RENDER_QUEUE_OVERLAY, Ogre::RenderQueue::RqSortMode::DisableSort);
		}
	}

	Ogre::SceneManager* Ogre2RenderManager::getSceneManager()
	{
		return mSceneManager;
	}

	bool Ogre2RenderManager::frameStarted(const Ogre::FrameEvent &evt)
	{
		// this used to be done in render(), but can't do this anymore:
		// adding Workspaces (e.g. RenderBox::requestUpdateCanvas)
		// can't be done while the CompositorManager is still iterating through workspaces in its update() method
		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		return true;
	}

	void Ogre2RenderManager::render()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		mCountBatch = 0;

		setManualRender(true);

		mSceneManager->getRenderQueue()->clear();

		//get mygui to itterate through renderables and call 'doRender'
		//This will add renderbles to the Ogre render queue
		onRenderToTarget(this, mUpdate);

		mSceneManager->getRenderQueue()->render(mSceneManager->getDestinationRenderSystem(), RENDER_QUEUE_OVERLAY, RENDER_QUEUE_OVERLAY+1, false, false);

		// сбрасываем флаг
		mUpdate = false;
	}

	void Ogre2RenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
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

	IVertexBuffer* Ogre2RenderManager::createVertexBuffer()
	{
		return new Ogre2VertexBuffer();
	}

	void Ogre2RenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	// для оповещений об изменении окна рендера
	void Ogre2RenderManager::windowResized(Ogre::RenderWindow* _window)
	{
		float scale = _window->getViewPointToPixelScale();
		mViewSize.set(_window->getWidth() / scale, _window->getHeight() / scale);
		// обновить всех
		mUpdate = true;

		updateRenderInfo();

		onResizeView(mViewSize);
	}

	void Ogre2RenderManager::updateRenderInfo()
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

	void Ogre2RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{

		Ogre2GuiRenderable* renderable = createOrRetrieveRenderable(_buffer, _texture, _count);

		mSceneManager->getRenderQueue()->addRenderableV1(RENDER_QUEUE_OVERLAY, false, renderable , mQueueMoveable);

		++ mCountBatch;
	}

	void Ogre2RenderManager::begin()
	{
	}

	void Ogre2RenderManager::end()
	{
	}

	ITexture* Ogre2RenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		Ogre2Texture* texture = new Ogre2Texture(_name, Ogre2DataManager::getInstance().getGroup());
		mTextures[_name] = texture;
		return texture;
	}

	void Ogre2RenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr) return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* Ogre2RenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
		{
			Ogre::TexturePtr texture = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(_name);
			if (!texture.isNull())
			{
				ITexture* result = createTexture(_name);
				static_cast<Ogre2Texture*>(result)->setOgreTexture(texture);
				return result;
			}
			return nullptr;
		}
		return item->second;
	}

	bool Ogre2RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return Ogre::TextureManager::getSingleton().isFormatSupported(
			Ogre::TEX_TYPE_2D,
			Ogre2Texture::convertFormat(_format),
			Ogre2Texture::convertUsage(_usage));
	}

	void Ogre2RenderManager::destroyAllResources()
	{
		for (MapRenderable::const_iterator item = mRenderables.begin(); item != mRenderables.end(); ++item)
		{
			delete item->second;
		}

		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

#if MYGUI_DEBUG_MODE == 1
	bool Ogre2RenderManager::checkTexture(ITexture* _texture)
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			if (item->second == _texture)
				return true;
		}
		return false;
	}
#endif

	const IntSize& Ogre2RenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType Ogre2RenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	const RenderTargetInfo& Ogre2RenderManager::getInfo()
	{
		return mInfo;
	}

	Ogre::RenderWindow* Ogre2RenderManager::getRenderWindow()
	{
		return mWindow;
	}

	bool Ogre2RenderManager::getManualRender()
	{
		return mManualRender;
	}

	void Ogre2RenderManager::setManualRender(bool _value)
	{
		mManualRender = _value;
	}

	size_t Ogre2RenderManager::getBatchCount() const
	{
		return mCountBatch;
	}

	Ogre2GuiRenderable* Ogre2RenderManager::createOrRetrieveRenderable( IVertexBuffer* _buffer , ITexture* _texture , size_t _count )
	{

		Ogre2VertexBuffer* vertexBuffer = static_cast<Ogre2VertexBuffer*>(_buffer);
		Ogre2Texture* texture = static_cast<Ogre2Texture*>(_texture);

		const unsigned int ID = vertexBuffer->getUniqueID();

		Ogre2GuiRenderable* renderable;

		MapRenderable::const_iterator item = mRenderables.find(ID);
		if (item == mRenderables.end())
		{
			mRenderables[ID] = new Ogre2GuiRenderable();
		}

		renderable = mRenderables[ID];

		vertexBuffer->setOperationVertexCount(_count);

		renderable->setRenderOperation(*vertexBuffer->getRenderOperation());
		renderable->setDatablock(texture->getDataBlock());

		return renderable;
	}

} // namespace MyGUI
