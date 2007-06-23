#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Button : public Window {
        Button(const __MYGUI_SUBSKIN_INFO * lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
	public:
		static Button *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_BUTTON);
	        
		// ��� ���������� ������� �����
		void _OnMouseSetFocus(MyGUI::Window * pWindowOld); // ���������� ��� ����� ���������� �� �������
		void _OnMouseLostFocus(MyGUI::Window * pWindowNew); // ���������� ��� ����� ���������� �� �������
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		// � ��� ��� ������
		void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // ����� ������
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����

        //Called when the user clicks on a button
        private:
		void showPressed(bool bIsPressed); // ����� �� �� ����� �������

	};

}