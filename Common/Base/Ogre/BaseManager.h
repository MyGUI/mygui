/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <Ogre.h>
#include <MyGUI.h>
#include "Base/StatisticInfo.h"

namespace MyGUI { class OgrePlatform; }

namespace base
{

	class BaseManager :
		public input::InputManager,
		public Ogre::FrameListener,
		public Ogre::WindowEventListener
	{
	public:
		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv);
		bool create();
		void destroy();
		void run();
		void quit() { mExit = true; }

		void setWindowCaption(const std::string& _text);
		void createDefaultScene();

		MyGUI::Gui* getGUI() { return mGUI; }
		const std::string& getRootMedia() { return mRootMedia; }
		void setResourceFilename(const std::string& _flename) { mResourceFileName = _flename; }
		void addResourceLocation(const std::string & _name, bool _recursive = false);
		statistic::StatisticInfo * getStatisticInfo() { return mInfo; }

	/*internal:*/
		Ogre::SceneManager* getSceneManager() { return mSceneMgr; }
		Ogre::Camera* getCamera() { return mCamera; }

	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

	private:
		void createGui();
		void destroyGui();

		virtual bool frameStarted(const Ogre::FrameEvent& _evt);
		virtual bool frameEnded(const Ogre::FrameEvent& _evt);
		virtual void windowResized(Ogre::RenderWindow* _rw);
		virtual void windowClosed(Ogre::RenderWindow* _rw);

		void addResourceLocation(const std::string& _name, const std::string& _group, const std::string& _type, bool _recursive);

	private:
		MyGUI::Gui* mGUI;
		MyGUI::OgrePlatform* mPlatform;
		statistic::StatisticInfo* mInfo;

		Ogre::Root *mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;

		bool mExit;

		Ogre::String mResourcePath;
		std::string mPluginCfgName;
		std::string mResourceXMLName;
		std::string mResourceFileName;
		std::string mRootMedia;

		Ogre::SceneNode* mNode;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
