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
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_OgreVertexBuffer.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OgreRenderManager);

	void OgreRenderManager::initialise(Ogre::RenderWindow* _window)
	{
		//MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		//MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// �������������
		mSceneManager = nullptr;
		mUpdate = false;
		//mMaximumDepth = 0;
		mListener = nullptr;
		mRenderSystem = nullptr;

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
			// ������ ����� � ��������
			Ogre::VertexElementType vertext_type = root->getRenderSystem()->getColourVertexElementType();
			if (vertext_type == Ogre::VET_COLOUR_ARGB) mVertexFormat = VertexColourType::ColourARGB;
			else if (vertext_type == Ogre::VET_COLOUR_ABGR) mVertexFormat = VertexColourType::ColourABGR;

			Ogre::SceneManagerEnumerator::SceneManagerIterator iter = root->getSceneManagerIterator();
			if (iter.hasMoreElements())
			{
				mSceneManager = iter.getNext();
				mSceneManager->addRenderQueueListener(this);
			}

			// ������������� �� ������ �����
			mRenderSystem = root->getRenderSystem();
			if (mRenderSystem != nullptr)
			{
				mRenderSystem->addListener(this);

				//mMaximumDepth = mRenderSystem->getMaximumDepthInputValue();
				//mTexelOffset.set(mRenderSystem->getHorizontalTexelOffset(), mRenderSystem->getVerticalTexelOffset());
			}
		}

		// ��������� �������
		mActiveViewport = 0;
		// ��������� ���� � �������
		mWindow = _window;
		if (mWindow != nullptr)
		{
			//MYGUI_ASSERT(mWindow->getNumViewports(), "You must have viewport for MyGUI initialisation.");
			//mViewSize.set(mWindow->getViewport(mActiveViewport)->getActualWidth(), mWindow->getViewport(mActiveViewport)->getActualHeight());
			Ogre::Viewport* port = mWindow->getViewport(mActiveViewport);
			mViewSize.set(port->getActualWidth(), port->getActualHeight());

			if (mRenderSystem != nullptr)
			{
				mRenderTargetInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
				mRenderTargetInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
				mRenderTargetInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
				mRenderTargetInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
				mRenderTargetInfo.pixScaleX = 1.0 / float(mViewSize.width);
				mRenderTargetInfo.pixScaleY = 1.0 / float(mViewSize.height);
			}
		}

		// ������������� �� ��������� �������� ���� � ����� ���������
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
			windowResized(mWindow);
		}

		//MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreRenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		//MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		clearTextures();

		// ������������
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
		}

		// ������� �������� �� ������ �����
		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			root->getRenderSystem()->removeListener(this);
		}

		setSceneManager(nullptr);

		//MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void OgreRenderManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		Ogre::Viewport * vp = mSceneManager->getCurrentViewport();
		if (nullptr == vp || !vp->getOverlaysEnabled()) return;

		if (mListener != nullptr) 
		{
			begin();
			mListener->doRender(mUpdate);
			end();
		}

		// ���������� ����
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
			// �������� ����
			mUpdate = true;
		}
	}

	void OgreRenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
	{
		mListener = _listener;
		mUpdate = true;
	}

	ITexture* OgreRenderManager::createTexture(const std::string& _name, const std::string& _group)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_ASSERT(item==mTextures.end(), "Resource '" << _name << "' already exist");

		OgreTexture* texture = new OgreTexture(_name, _group);
		mTextures[_name] = texture;
		
		return texture;
	}

	void OgreRenderManager::destroyTexture(ITexture* _texture)
	{
		for (MapTexture::iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
		{
			if (item->second == _texture)
			{
				delete _texture;
				mTextures.erase(item);
				break;
			}
		}
	}

	ITexture* OgreRenderManager::getByName(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		return item == mTextures.end() ? nullptr : item->second;
	}

	void OgreRenderManager::clearTextures()
	{
		for (MapTexture::iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
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
		MYGUI_ASSERT(mWindow, "Gui is not initialised.");
		MYGUI_ASSERT(mWindow->getNumViewports() >= _num, "index out of range");
		mActiveViewport = _num;
		// �������� ����������
		windowResized(mWindow);
	}

	// ��� ���������� �� ��������� ���� �������
	void OgreRenderManager::windowResized(Ogre::RenderWindow* rw)
	{
		Ogre::Viewport * port = rw->getViewport(mActiveViewport);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());

		// �������� ����
		mUpdate = true;

		if (mRenderSystem != nullptr)
		{
			mRenderTargetInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
			mRenderTargetInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
			mRenderTargetInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
			mRenderTargetInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
			mRenderTargetInfo.pixScaleX = 1.0 / float(mViewSize.width);
			mRenderTargetInfo.pixScaleY = 1.0 / float(mViewSize.height);
		}

		Gui* gui = Gui::getInstancePtr();
		if (gui != nullptr) gui->resizeWindow(mViewSize);
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

	void OgreRenderManager::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
		mRenderSystem->_setTexture(0, true, _texture);

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
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

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

} // namespace MyGUI
