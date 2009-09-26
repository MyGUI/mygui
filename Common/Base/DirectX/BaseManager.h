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

#include <MyGUI.h>
#include <MyGUI_DirectXPlatform.h>
#include "../StatisticInfo.h"
#include <OIS/OIS.h>

namespace base
{

	class BaseManager : public OIS::MouseListener, public OIS::KeyListener
	{
	public:
		static BaseManager & getInstance();

		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv); // инициализация коммандной строки
		bool create(); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();
		void quit() { m_exit = true; }

		int getWidth() { return (int)mWidth; }
		int getHeight() { return (int)mHeight; }

		void setWindowCaption(const std::string & _text);
		void createDefaultScene() { }

		MyGUI::Gui* getGUI() { return mGUI; }
		const std::string& getRootMedia() { return mRootMedia; }
		void setResourceFilename(const std::string& _flename) { mResourceFileName = _flename; }
		void addResourceLocation(const std::string & _name, bool _recursive = false);
		statistic::StatisticInfo * getStatisticInfo() { return mInfo; }

	/*internal:*/
		void _windowResized();

	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		virtual bool injectMouseMove(int _absx, int _absy, int _absz);
		virtual bool injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual bool injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual bool injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual bool injectKeyRelease(MyGUI::KeyCode _key);

	private:
		void createInput(); // создаем систему ввода
		void destroyInput(); // удаляем систему ввода

		void createGui();
		void destroyGui();

		void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);
		void injectFrameEntered();

		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );

	private:
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse*    mMouse;

		static BaseManager * m_instance;

		size_t mWidth, mHeight; // ширина и высота экрана

		MyGUI::Gui* mGUI;
		MyGUI::DirectXPlatform* mPlatform;
		statistic::StatisticInfo* mInfo;

		HWND hwnd;
	    IDirect3D9 *d3d;
		IDirect3DDevice9 *device;
	    D3DPRESENT_PARAMETERS d3dpp;
		HINSTANCE hInstance;

		bool m_exit;

		std::string mRootMedia;
		std::string mResourceFileName;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
