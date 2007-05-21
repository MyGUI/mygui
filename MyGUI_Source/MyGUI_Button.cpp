#include "MyGUI_Button.h"
#include "MyGUI_AssetManager.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Button::Button(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
      
      : StaticText(PosX, PosY, SizeX, SizeY, parent, uAlign, uOverlay, Skin)
	{		
		this->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
	}

	void Button::_OnMouseChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �� �������
	{
		Window::_OnMouseChangeFocus(bIsFocus); // ��� ������� ��������
		showFocus(bIsFocus);
	}

	void Button::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // ���������� ��� ������� �������
	{
		Window::_OnMouseButtonPressed(bIsLeftButtonPressed); // ��� ������� ��������
		showPressed(bIsLeftButtonPressed);
	}

	void Button::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // ����� ������
	{
		if (pWindow->m_uExData & WES_BUTTON) showFocus(bIsFocus);
	}

	void Button::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) // ������ ����� ������ ����
	{
		if (pWindow->m_uExData & WES_BUTTON)
		    showPressed(bIsLeftButtonPressed);
	}

	void Button::showPressed(bool bIsLeftPressed) // ����� �� �� ����� �������
	{
		if (m_uState == WS_DEACTIVATED)
		    return;
		
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;
		bool bIsShiftText = false;

		if (bIsLeftPressed) {
		    switch(m_uState)
		    {
		        case WS_NORMAL:
		            Skin = SKIN_STATE_PRESSED;
		            break;
		        case __WS_ACTIVATED:
		            Skin = SKIN_STATE_SELECTED;
		            break;
		        default:
		            return;
			}
			bIsShiftText = true;
		} else {
		    switch(m_uState)
		    {
		        case WS_NORMAL:
		            Skin = SKIN_STATE_NORMAL;
		            break;
		        case __WS_ACTIVATED:
		            Skin = SKIN_STATE_ACTIVE;
		            break;
		        default:
		            return;
		    }
		}

		if (Skin != __SKIN_STATE_COUNT) { // ������ ����� ���������
			// �������� ����
			if (m_uExData & WES_BUTTON)
				setSkinState(Skin);
			
			//Set all the children windows' graphics to the current state
			for (ChildWindowsIterator i = mChildWindows.begin(); i != mChildWindows.end(); ++i)
				if ((*i)->m_uExData & WES_BUTTON)
				    (*i)->setSkinState(Skin);
		}
        
        shiftText(bIsShiftText);
	}
}