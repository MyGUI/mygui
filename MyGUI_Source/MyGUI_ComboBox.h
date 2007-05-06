//=========================================================================================
#ifndef __MyGUI_ComboBox_H__
#define __MyGUI_ComboBox_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class List;
	class GUI;

	class ComboBox : public Window {

	public:
		ComboBox(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		~ComboBox();

		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши
		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса ввода
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события

		inline void addString(const DisplayString & strElement){m_pList->addString(strElement);}; // добавить строку в список
		inline void insertString(uint16 index, const DisplayString & strElement){m_pList->insertString(index,strElement);}; // вставить строку в список
		inline void deleteString(uint16 index){m_pList->deleteString(index);}; // удалить строку из списка
		const DisplayString & getString(uint16 index); // возвращает строку
		void setString(uint16 index); // устанавливает строку из списка в поле
		inline uint16 getCurrentIndex() {return m_pList->m_uSelectItem;}; // возвращает текущее положение
		inline void deleteStringAll() {m_pList->deleteStringAll();setWindowText("");}; // удаляет все строки
		inline uint16 getStringCount() {return (uint16)m_pList->m_aString.size();}; // возвращает колличество строк
		
		Edit * m_pEdit; // едит для класса
		List * m_pList; // всплывающий список
		bool m_bIsListShow; // виден ли список
		bool m_bHideList; // скрыть список
	};

}
//=========================================================================================
#endif
//=========================================================================================
