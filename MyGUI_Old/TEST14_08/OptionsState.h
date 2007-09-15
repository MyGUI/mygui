#pragma once

#include "BasisState.h"
#include "SkinEditor.h"
#include "Widget.h"

using namespace widget;

class OptionsState : public BasisState
{
public:
	enum {
		NONE,
		MESSAGE_ID_EXIT,
	};
public:

	void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
	void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // �������������� �������
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	MyGUI::Button * m_buttonExit;
	MyGUI::Button * m_buttonEditor;

	MyGUI::ComboBox * m_comboBackground; // ���
	std::vector <std::string> m_straColour;

	SkinEditor::SkinEditor * mEditor; // �������� ������

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // ����������� �� ��������� �������� ���� �������

	bool toggle; //������������ �������� m_widgetChild � m_widgetSubChild
	Widget * m_widget1;
	Widget * m_widget2;
	Widget * m_widget3;
	Widget * m_widget4;

};