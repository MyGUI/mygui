/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <MyGUI.h>
#include "StatisticInfo.h"

namespace base
{

	class BaseManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
	{
	public:
		static BaseManager & getInstance();

		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv); // ������������� ���������� ������
		void create(); // ������� ��������� ����� ������� ����������
		void destroy(); // ������� ��� ��������� ������� ����������
		void run();


		int getWidth() {return (int)mWidth;}
		int getHeight() {return (int)mHeight;}

		void addResourceLocation(const Ogre::String & _name, const Ogre::String & _type = "FileSystem", const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bool _recursive = false);

		void setWindowCaption(const std::string & _text);
		void setWallpaper(const std::string & _filename);
		void setDescriptionText(const Ogre::UTFString & _text);

		statistic::StatisticInfo * getStatisticInfo() { return mInfo; }

	protected:
		void createInput(); // ������� ������� �����
		void destroyInput(); // ������� ������� �����

		void createGui();
		void destroyGui();

		virtual void createScene() { }
		virtual void destroyScene() { }

		void setupResources(); // ��������� ��� ������� ����������

		virtual bool frameStarted(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );

		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

	protected:
		static BaseManager * m_instance;

		//OIS Input devices
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse*    mMouse;

		Ogre::Root *mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::String mResourcePath;

		bool m_exit; // ������� �� ����� ����������
		size_t mWidth, mHeight; // ������ � ������ ������

		MyGUI::Gui * mGUI;
		statistic::StatisticInfo * mInfo;

		std::string mPluginCfgName;
		std::string mResourceCfgName;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
