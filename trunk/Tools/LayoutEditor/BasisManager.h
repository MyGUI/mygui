#ifndef __BASIS_MANAGER_H__
#define __BASIS_MANAGER_H__

#include "EditorState.h"
#include "MyGUI_OgrePlatform.h"

namespace input { class InputManager; }

class BasisManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
{
private:

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	std::string mResourcePath;
	bool m_exit; // ������� �� ����� ����������
	size_t mWidth, mHeight; // ������ � ������ ������

	std::vector<BasisState*> mStates; // ������ ���������
	EditorState mEditor;
	typedef std::vector<std::string> Params;
	Params mParams;

	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

public:

	BasisManager();

	// ��������� ������ � ������ ����������
	void addCommandParam(const std::string& _param);
	// ���������� ������ ���������� ���������� ������
	const Params & getCommandParams() { return mParams; }

	void setWindowCaption(const std::string& _text);

	static BasisManager & getInstance() {static BasisManager instance;return instance;}

	void createBasisManager(); // ������� ��������� ����� ������� ����������
	void destroyBasisManager(); // ������� ��� ��������� ������� ����������

	void setMainWindowIcon(size_t _iconId);

	void dropFile(const std::wstring& _file);
	void windowClose();

	bool isFullscreen() { return mFullscreen; }
	void setFullscreen(bool _fullscreen);

	void eventExit() {m_exit = true;}

	std::string getLanguage() {return mLanguage;}
	void setLanguage(std::string _language) {mLanguage = _language;}

private:
	void setupResources(); // ��������� ��� ������� ����������
	void addResourceLocation(const std::string & _name, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, const std::string & _type = "FileSystem", bool _recursive = false);


	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void changeState(BasisState* state);
	void pushState(BasisState* state);
	void popState();

	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

	void startRendering();

	void loadLocation(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

private:
	std::string mRootMedia;
	bool mFullscreen;

	// ������� �����
	input::InputManager * mInput;
	std::string mLanguage;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// ���������� ������ �������� ����
	size_t mHwnd;
#endif

};
#endif // __BASIS_MANAGER_H__
