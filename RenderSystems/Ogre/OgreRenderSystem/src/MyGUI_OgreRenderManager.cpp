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

#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace MyGUI
{

	#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	// This function will locate the path to our application on OS X,
	// unlike windows you can not rely on the curent working directory
	// for locating your configuration files and resources.
	std::string MYGUI_EXPORT macBundlePath()
	{
		char path[1024];
		CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
		CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		return std::string(path);
	}
	#endif

	MYGUI_INSTANCE_IMPLEMENT(OgreRenderManager);

	void OgreRenderManager::initialise(Ogre::RenderWindow* _window)
	{
		//MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		//MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// инициализация
		mSceneManager = nullptr;
		mUpdate = false;
		mMaximumDepth = 0;
		mListener = nullptr;

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
			Ogre::RenderSystem * render = root->getRenderSystem();
			if (render != nullptr)
			{
				render->addListener(this);

				mMaximumDepth = render->getMaximumDepthInputValue();
				mTexelOffset.set(render->getHorizontalTexelOffset(), render->getVerticalTexelOffset());
			}
		}

		// дефолтный вьюпорт
		mActiveViewport = 0;
		// сохраняем окно и размеры
		mWindow = _window;
		if (mWindow != nullptr)
		{
			//MYGUI_ASSERT(mWindow->getNumViewports(), "You must have viewport for MyGUI initialisation.");
			mViewSize.set(mWindow->getViewport(mActiveViewport)->getActualWidth(), mWindow->getViewport(mActiveViewport)->getActualHeight());
		}

		// подписываемся на изменение размеров окна и сразу оповещаем
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

		clear();

		// отписываемся
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
		}

		// удаляем подписку на рендер евент
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
		if ((nullptr == vp) || (false == vp->getOverlaysEnabled())) return;

		if (mListener!= nullptr) mListener->doRender(mUpdate);

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

	void OgreRenderManager::clear()
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

	void OgreRenderManager::setActiveViewport(size_t _num)
	{
		if (_num == mActiveViewport) return;
		MYGUI_ASSERT(mWindow, "Gui is not initialised.");
		MYGUI_ASSERT(mWindow->getNumViewports() >= _num, "index out of range");
		mActiveViewport = _num;
		// рассылка обновлений
		windowResized(mWindow);
	}

	// для оповещений об изменении окна рендера
	void OgreRenderManager::windowResized(Ogre::RenderWindow* rw)
	{
		Ogre::Viewport * port = rw->getViewport(mActiveViewport);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());

		// обновить всех
		mUpdate = true;

		Gui* gui = Gui::getInstancePtr();
		if (gui != nullptr) gui->resizeWindow(mViewSize);
	}

	const VectorString& OgreRenderManager::getVectorResourcePath(
		const std::string& _pattern,
		const std::string& _group,
		bool _fullpath,
		bool _fullmatch)
	{
		static VectorString result;
		result.clear();
//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//		Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, convert::utf8_to_ansi(_pattern));
//#else
		Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _pattern);
//#endif

		result.reserve(pFileInfo->size());

		for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi )
		{
			if (!_fullmatch || fi->path.empty())
			{
				if (_fullpath)
				{
					std::string path = fi->archive->getName() + "/" + fi->filename;
					bool find = false;
					for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter)
					{
						if (*iter == path) { find = true; break; }
					}
					if (!find)
					{
//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//						result.push_back(convert::ansi_to_utf8(path));
//#else
						result.push_back(path);
//#endif
					}
				}
				else
				{
					bool find = false;
					for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter)
					{
						if (*iter == fi->filename) { find = true; break; }
					}
					if (!find)
					{
//#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
//						result.push_back(convert::ansi_to_utf8(fi->filename));
//#else
						result.push_back(fi->filename);
//#endif
					}
				}

			}
		}

		pFileInfo.setNull();

		return result;
	}

} // namespace MyGUI
