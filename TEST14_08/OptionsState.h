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

	void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
	void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // дополнительные события
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	MyGUI::Button * m_buttonExit;
	MyGUI::Button * m_buttonEditor;

	MyGUI::ComboBox * m_comboBackground; // фон
	std::vector <std::string> m_straColour;

	SkinEditor::SkinEditor * mEditor; // редактор скинов

public:
	void enter(bool bIsChangeState);
	void exit();
	void windowResize(); // уведомление об изменении размеров окна рендера

	bool toggle; //переключение движения m_widgetChild и m_widgetSubChild
	Widget * m_widget1;
	Widget * m_widget2;
	Widget * m_widget3;
	Widget * m_widget4;

};