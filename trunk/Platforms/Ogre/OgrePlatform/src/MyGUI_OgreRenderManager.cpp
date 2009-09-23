/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_OgreVertexBuffer.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OgreRenderManager);

	void OgreRenderManager::initialise(Ogre::RenderWindow* _window)
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// инициализация
		mSceneManager = nullptr;
		mWindow = nullptr;
		mUpdate = false;
		mRenderSystem = nullptr;
		mActiveViewport = -1;

		mColorBlendMode.blendType	= Ogre::LBT_COLOUR;
		mColorBlendMode.source1		= Ogre::LBS_TEXTURE;
		mColorBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mColorBlendMode.operation	= Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType	= Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1		= Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation	= Ogre::LBX_MODULATE;

		mTextureAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			// формат цвета в вершинах
			Ogre::VertexElementType vertext_type = root->getRenderSystem()->getColourVertexElementType();
			if (vertext_type == Ogre::VET_COLOUR_ARGB) mVertexFormat = VertexColourType::ColourARGB;
			else if (vertext_type == Ogre::VET_COLOUR_ABGR) mVertexFormat = VertexColourType::ColourABGR;

			Ogre::SceneManagerEnumerator::SceneManagerIterator iter = root->getSceneManagerIterator();
			if (iter.hasMoreElements())
			{
				mSceneManager = iter.getNext();
				mSceneManager->addRenderQueueListener(this);
			}

			// подписываемся на рендер евент
			mRenderSystem = root->getRenderSystem();
			if (mRenderSystem != nullptr)
			{
				mRenderSystem->addListener(this);
			}
		}

		// дефолтный вьюпорт
		mActiveViewport = 0;
		// сохраняем окно и размеры
		mWindow = _window;
		if (mWindow != nullptr && mWindow->getNumViewports() > 0)
		{
			Ogre::Viewport* port = mWindow->getViewport(mActiveViewport);
			mViewSize.set(port->getActualWidth(), port->getActualHeight());

			if (mRenderSystem != nullptr)
			{
				mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
				mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
				mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
				mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
				mInfo.pixScaleX = 1.0 / float(mViewSize.width);
				mInfo.pixScaleY = 1.0 / float(mViewSize.height);
			}
		}
		else
		{
			mActiveViewport = -1;
		}

		mInfo.rttFlipY = Ogre::Root::getSingleton().getRenderSystem()->getName( ) == "OpenGL Rendering Subsystem";

		// подписываемся на изменение размеров окна и сразу оповещаем
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
			windowResized(mWindow);
		}

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreRenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		destroyAllResources();

		// отписываемся
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			mWindow = nullptr;
		}

		// удаляем подписку на рендер евент
		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			root->getRenderSystem()->removeListener(this);
		}

		setSceneManager(nullptr);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void OgreRenderManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		Ogre::Viewport * vp = mSceneManager->getCurrentViewport();
		if (nullptr == vp || !vp->getOverlaysEnabled()) return;

		begin();
		LayerManager::getInstance().renderToTarget(this, mUpdate);
		end();

		// сбрасываем флаг
		mUpdate = false;
	}

	void OgreRenderManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void OgreRenderManager::setSceneManager(Ogre::SceneManager * _scene)
	{
		if (nullptr != mSceneManager) mSceneManager->removeRenderQueueListener(this);
		mSceneManager = _scene;
		if (nullptr != mSceneManager) mSceneManager->addRenderQueueListener(this);
	}

	void OgreRenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
	{
		if(eventName == "DeviceLost")
		{
		}
		else if(eventName == "DeviceRestored")
		{
			// обновить всех
			mUpdate = true;
		}
	}

	IVertexBuffer* OgreRenderManager::createVertexBuffer()
	{
		return new OgreVertexBuffer();
	}

	void OgreRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OgreRenderManager::setActiveViewport(size_t _num)
	{
		if (_num == mActiveViewport) return;
		MYGUI_PLATFORM_ASSERT(mWindow, "Gui is not initialised.");
		MYGUI_PLATFORM_ASSERT(mWindow->getNumViewports() >= _num, "index out of range");
		mActiveViewport = _num;
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
		// рассылка обновлений
		windowResized(mWindow);
	}

	// для оповещений об изменении окна рендера
	void OgreRenderManager::windowResized(Ogre::RenderWindow* rw)
	{
		if(rw->getNumViewports() > 0)
		{
			Ogre::Viewport * port = rw->getViewport(mActiveViewport);
			mViewSize.set(port->getActualWidth(), port->getActualHeight());

			// обновить всех
			mUpdate = true;

			if (mRenderSystem != nullptr)
			{
				mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
				mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
				mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
				mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
				mInfo.pixScaleX = 1.0 / float(mViewSize.width);
				mInfo.pixScaleY = 1.0 / float(mViewSize.height);
			}

			Gui* gui = Gui::getInstancePtr();
			if (gui != nullptr) gui->resizeWindow(mViewSize);
		}
	}

	void OgreRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OgreTexture* texture = static_cast<OgreTexture*>(_texture);

		mRenderSystem->_setTexture(0, true, texture->getOgreTexture());
		
		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		mRenderSystem->_render(*operation);
	}

	void OgreRenderManager::begin()
	{
		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

		// initialise render settings
		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		mRenderSystem->setShadingType(Ogre::SO_GOURAUD);

		// initialise texture settings
		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
		mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
		mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0);
#else
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
#endif
		mRenderSystem->_setTextureBlendMode(0, mColorBlendMode);
		mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
		mRenderSystem->_disableTextureUnitsFrom(1);

		// enable alpha blending
		mRenderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}

	void OgreRenderManager::end()
	{
	}

	ITexture* OgreRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OgreTexture* texture = new OgreTexture(_name, OgreDataManager::getInstance().getGroup());
		mTextures[_name] = texture;
		return texture;
	}

	void OgreRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr) return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OgreRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end()) return nullptr;
		return item->second;
	}

	void OgreRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

} // namespace MyGUI