//===================================================================================
#pragma once
//===================================================================================
#include "BasisState.h"
#include "SkinEditor.h"
//===================================================================================
//===================================================================================
class OptionsState : public BasisState
{
public:
	enum {
		NONE,
		MESSAGE_ID_EXIT,
	};
public:

	void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
	void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������

	MyGUI::Button * m_buttonExit;

	ComboBox * m_comboBackground; // ���
	vector <string> m_straColour;

	SkinEditor mEditor; // �������� ������

public:
	void enter(bool bIsChangeState);
	void exit();

};
//===================================================================================
