/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*//*
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
#include "MyGUI_RenderManager.h"
#include "MyGUI_Texture.h"
#include "MyGUI_VertexBuffer.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(RenderManager);

	void RenderManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// инициализация
		mSceneManager = nullptr;
		mPixScaleX = mPixScaleY = 1;
        mHOffset = mVOffset = 0;
		mAspectCoef = 1;
		mUpdate = false;
		mMaximumDepth = 0;
		mListener = nullptr;

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			// формат цвета в вершинах
			Ogre::VertexElementType vertext_type = root->getRenderSystem()->getColourVertexElementType();
			if (vertext_type == Ogre::VET_COLOUR_ARGB) mVertexFormat = VertexFormat::ColourARGB;
			else if (vertext_type == Ogre::VET_COLOUR_ABGR) mVertexFormat = VertexFormat::ColourABGR;

			Ogre::SceneManagerEnumerator::SceneManagerIterator iter = root->getSceneManagerIterator();
			if (iter.hasMoreElements())
			{
				mSceneManager = iter.getNext();
				mSceneManager->addRenderQueueListener(this);
			}

			// подписываемся на рендер евент
			Ogre::RenderSystem * render = root->getRenderSystem();
			if (render != nullptr)
			{
				render->addListener(this);
				// не забывай, о великий построитель гуёв
				// Кто здесь?
				mMaximumDepth = render->getMaximumDepthInputValue();
			}
		}

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void RenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		clear();

		// удаляем подписку на рендер евент
		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			root->getRenderSystem()->removeListener(this);
		}

		setSceneManager(nullptr);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void RenderManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		Ogre::Viewport * vp = mSceneManager->getCurrentViewport();
		if ((nullptr == vp) || (false == vp->getOverlaysEnabled())) return;

		mCountBatch = 0;

		if (mListener!= nullptr) mListener->doRender(mUpdate);

		// сбрасываем флаг
		mUpdate = false;
	}

	void RenderManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void RenderManager::_windowResized(const IntSize& _size)
	{
		// новый размер
		mPixScaleX = 1.0 / _size.width;
		mPixScaleY = 1.0 / _size.height;
		mAspectCoef = float(_size.height) / _size.width;

		Ogre::RenderSystem * render = Ogre::Root::getSingleton().getRenderSystem();

        mHOffset = render->getHorizontalTexelOffset() / _size.width;
        mVOffset = render->getVerticalTexelOffset() / _size.height;

		// обновить всех
		mUpdate = true;
	}

	void RenderManager::setSceneManager(Ogre::SceneManager * _scene)
	{
		if (nullptr != mSceneManager) mSceneManager->removeRenderQueueListener(this);
		mSceneManager = _scene;
		if (nullptr != mSceneManager) mSceneManager->addRenderQueueListener(this);
	}

	void RenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
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

	void RenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
	{
		mListener = _listener;
		mUpdate = true;
	}

	ITexture* RenderManager::createTexture(const std::string& _name, const std::string& _group)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_ASSERT(item==mTextures.end(), "Resource '" << _name << "' already exist");

		Texture* texture = new Texture(_name, _group);
		mTextures[_name] = texture;
		
		return texture;
	}

	void RenderManager::destroyTexture(ITexture* _texture)
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

	ITexture* RenderManager::getByName(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		return item == mTextures.end() ? nullptr : item->second;
	}

	bool RenderManager::isExist(const std::string& _name)
	{
		return mTextures.find(_name) != mTextures.end();
	}

	void RenderManager::clear()
	{
		for (MapTexture::iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

	const std::string& RenderManager::getDefaultGroup()
	{
		return ResourceManager::getInstance().getResourceGroup();
	}

	IVertexBuffer* RenderManager::createVertexBuffer()
	{
		return new VertexBuffer();
	}

} // namespace MyGUI
