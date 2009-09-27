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

	class BaseManager :
		public input::InputManager
	{
	public:
		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv); // инициализация коммандной строки
		bool create(); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();
		void quit() { mExit = true; }

		void setWindowCaption(const std::string& _text);
		void createDefaultScene() { }

		MyGUI::Gui* getGUI() { return mGUI; }
		const std::string& getRootMedia() { return mRootMedia; }
		void setResourceFilename(const std::string& _flename) { mResourceFileName = _flename; }
		void addResourceLocation(const std::string& _name, bool _recursive = false);
		statistic::StatisticInfo * getStatisticInfo() { return mInfo; }

	/*internal:*/
		void _windowResized();

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

		void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);
		void injectFrameEntered();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::DirectXPlatform* mPlatform;
		statistic::StatisticInfo* mInfo;

		HWND hwnd;
	    IDirect3D9 *d3d;
		IDirect3DDevice9 *device;
	    D3DPRESENT_PARAMETERS d3dpp;
		HINSTANCE hInstance;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
