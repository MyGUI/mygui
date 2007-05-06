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

	void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
	void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события

	MyGUI::Button * m_buttonExit;
	MyGUI::Button * m_buttonSave;
	MyGUI::Button * m_buttonLoad;

	ComboBox * m_comboBackground; // фон
	vector <string> m_straColour;

	SkinEditor mEditor; // редактор скинов

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

};
//===================================================================================
