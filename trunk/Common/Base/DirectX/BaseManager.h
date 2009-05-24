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

namespace base
{

	class BaseManager
	{
	public:
		static BaseManager & getInstance();

		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv); // ������������� ���������� ������
		bool create(); // ������� ��������� ����� ������� ����������
		void destroy(); // ������� ��� ��������� ������� ����������
		void run();

		int getWidth() { return (int)mWidth; }
		int getHeight() { return (int)mHeight; }

		void addResourceLocation(const std::string & _name, const std::string & _group = "General", const std::string & _type = "FileSystem", bool _recursive = false);

		void setWindowCaption(const std::string & _text);
		void setWallpaper(const std::string & _filename);
		void setDescriptionText(const MyGUI::UString & _text);

		statistic::StatisticInfo * getStatisticInfo() { return mInfo; }

		MyGUI::IViewport* getMainViewport() { return mViewport; }

	protected:
		void createInput(); // ������� ������� �����
		void destroyInput(); // ������� ������� �����

		void createGui();
		void destroyGui();

		virtual void createScene() { }
		virtual void destroyScene() { }

	private:
		void window_adjust_settings(HWND hWnd, int width, int height, bool fullScreen);

	protected:
		static BaseManager * m_instance;

		MyGUI::IViewport* mViewport;

		size_t mWidth, mHeight; // ������ � ������ ������

		MyGUI::Gui* mGUI;
		MyGUI::DirectXPlatform* mPlatform;
		statistic::StatisticInfo* mInfo;

		HWND hwnd;
	    IDirect3D9 *d3d;
		IDirect3DDevice9 *device;
	    D3DPRESENT_PARAMETERS d3dpp;
		HINSTANCE hInstance;

	};

} // namespace base

#endif // __BASE_MANAGER_H__
