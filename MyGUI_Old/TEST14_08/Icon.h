	/**********************************************************************************
	*  SetMainWindowInfo.h
	*  ����� ������ � ����� ���� OGRE DirectX � OpenGL [my.name (my.work(a)inbox.ru)]
	***********************************************************************************/
	#ifndef __SetMainWindowInfo_H__
	#define __SetMainWindowInfo_H__

	#include "windows.h"
	#include "resource.h"

	void g_setMainWindowInfo(char *strWindowCaption, unsigned int uIconResourceID)
	{
		HWND hWnd = ::FindWindow("OgreD3D9Wnd", "OGRE Render Window");
		if (!::IsWindow(hWnd)) hWnd = ::FindWindow("OgreGLWindow", "OGRE Render Window");
		if (::IsWindow(hWnd)) {
			if (strWindowCaption) ::SetWindowText(hWnd, strWindowCaption);
			// ����� ��� ������ ���������
			char buf[1024];
			::GetModuleFileName(0, (LPCH)&buf, 1024);
			// ����� ������� ������ ������
			HINSTANCE instance = ::GetModuleHandle(buf);
			// ���������� ������ ������
			HICON hIcon = ::LoadIcon(instance, (LPCTSTR)uIconResourceID);
			if (hIcon) {
				::SendMessage(hWnd, WM_SETICON, 1, (LPARAM)hIcon);
				::SendMessage(hWnd, WM_SETICON, 0, (LPARAM)hIcon);
			}
		}
	}

	#endif
