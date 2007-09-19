
#include "xmlDocument.h"
#include "Common.h"
#include "InputManager.h"
#include "PointerManager.h"
#include "Widget.h"

namespace MyGUI
{

	const std::string INPUT_DEFAULT_LANGUAGE = "English";

	InputManager::InputManager() :
		m_widgetMouseFocus(0), m_widgetKeyFocus(0),
		m_isWidgetMouseCapture(false),
		m_isCharShift(false)
	{
		createDefaultCharSet();
		loadCharSet("main.lang");
	}

	bool InputManager::injectMouseMove( const OIS::MouseEvent & _arg)
	{
		// ������� ������
		PointerManager::getInstance().move(_arg.state.X.abs, _arg.state.Y.abs);

		// �������� �� ������
		if (_arg.state.Z.rel != 0) {
			if (m_widgetMouseFocus != null) m_widgetMouseFocus->OnMouseSheel(_arg.state.Z.rel);
			return isCaptureMouse();
		}

		if (m_isWidgetMouseCapture) {
			if (m_widgetMouseFocus != null) m_widgetMouseFocus->OnMouseMove(_arg.state.X.abs, _arg.state.Y.abs);
			else m_isWidgetMouseCapture = false;
			return true;
		}

		// ���� �������� ����
		LayerItemInfoPtr info = LayerManager::getInstance().findItem(_arg.state.X.abs, _arg.state.Y.abs);
		WidgetPtr item = info ? dynamic_cast<WidgetPtr>(info) : null;
		if ( (item!= null) && (!item->isEnable()) ) item = null; // ���������� ����

		// ������ �� ����������
		if (m_widgetMouseFocus == item) return isCaptureMouse();

		// ����� ������
		if (m_widgetMouseFocus != null) m_widgetMouseFocus->OnMouseLostFocus(item);
		if (item != null) item->OnMouseSetFocus(m_widgetMouseFocus);

		// ���������� ������� ����
		m_widgetMouseFocus = item;

		return isCaptureMouse();
	}

	bool InputManager::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (!isCaptureMouse()) {
			resetKeyFocusWidget();
			return false;
		}

		// ����������� ������ �� ����� ������� � ������ ���� ������� ����
		if ( _id == OIS::MB_Left ) {
			// ������ ����
			m_isWidgetMouseCapture = true;
			// ���������� ����� �������
			m_lastLeftPressed.left = (int)_arg.state.X.abs;
			m_lastLeftPressed.top = (int)_arg.state.Y.abs;
		}
			
		m_widgetMouseFocus->OnMouseButtonPressed(_id == OIS::MB_Left);
		setKeyFocusWidget(m_widgetMouseFocus);

		// ��������� ������, ��������
		if (m_widgetMouseFocus != null) {
			WidgetPtr tmp = m_widgetMouseFocus;
			while (tmp->getParent() != null) tmp = tmp->getParent();
			LayerManager::getInstance().upItem(tmp);
		}
		return true;
	}

	bool InputManager::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id )
	{

		if (isCaptureMouse()) {

			// ���������� ������
			m_isWidgetMouseCapture = false;

			m_widgetMouseFocus->OnMouseButtonReleased(_id == OIS::MB_Left);

			if ((_id == OIS::MB_Left) && m_time.getMilliseconds() < GUI_TIME_DOUBLE_CLICK) {
				m_widgetMouseFocus->OnMouseButtonClick(true);
			} else {
			    m_time.reset();
	            m_widgetMouseFocus->OnMouseButtonClick(false);
			}

			// ��� ����������� �����������
			injectMouseMove(_arg);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(const OIS::KeyEvent & _arg)
	{
		// �������� �� ������������ ������
		detectLangShift(_arg.key, true);
		
		//Pass keystrokes to the current active text widget
		if (isCaptureKey()) m_widgetKeyFocus->OnKeyButtonPressed(_arg.key, getKeyChar(_arg.key));

		return isCaptureKey();
	}

	bool InputManager::injectKeyRelease(const OIS::KeyEvent & _arg)
	{
		// �������� �� ������������ ������
		detectLangShift(_arg.key, false);

		if (isCaptureKey()) m_widgetKeyFocus->OnKeyButtonReleased(_arg.key);

		return isCaptureKey();
	}

    //Detects switching from an english to a russian mode on a keyboard (?)
	void InputManager::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		// ���� ����������� �� ����
		if (m_mapLanguages.size() == 1) return;

		// ��� ���������� ������������� ������������ LeftAlt+LeftShift ��� LeftCtrl+LeftShift,LeftShift+LeftAlt ��� LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt ��� LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // ��� ���� ������

		if ((keyEvent == OIS::KC_LSHIFT) || (keyEvent == OIS::KC_RSHIFT))
		{
			if (bIsKeyPressed) {
				m_isCharShift = true;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				m_isCharShift = false;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// ��������� ����
					changeLanguage();
				}
			}
		}
		else if ((keyEvent == OIS::KC_LMENU) || (keyEvent == OIS::KC_LCONTROL))
		{
			if (bIsKeyPressed) {
				bIsFirstKeyPressed = true;
				if (bIsSecondKeyPressed) bIsTwoKeyPressed = true;
			} else {
				bIsFirstKeyPressed = false;
				if ((!bIsSecondKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					// ��������� ����
					changeLanguage();
				}
			}
		}
		else
		{
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}
	}

	wchar_t InputManager::getKeyChar(int keyEvent) // ���������� ������ �� ��� ���� ����
	{
		if (keyEvent < 58) return m_currentLanguage->second[keyEvent + (m_isCharShift ? 58 : 0)];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return m_nums[keyEvent-71];
		} else if (keyEvent == OIS::KC_DIVIDE) return m_currentLanguage->second[OIS::KC_SLASH + (m_isCharShift ? 58 : 0)];//L'/'; // �� �������� ���������� //????m_langs[m_currentLang][OIS::KC_BACKSLASH + (m_bShiftChars ? 58 : 0)];
		else if (keyEvent == OIS::KC_OEM_102) return m_currentLanguage->second[OIS::KC_BACKSLASH + (m_isCharShift ? 58 : 0)];
		return 0;
	}

	void InputManager::createDefaultCharSet()
	{
		// ��������� ���������� ����
		m_mapLanguages[INPUT_DEFAULT_LANGUAGE] = LangInfo();
		m_currentLanguage = m_mapLanguages.find(INPUT_DEFAULT_LANGUAGE);
		m_currentLanguage->second.resize(116);

		// ��������� ������
		wchar_t chars[116] = {
			0, 0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, 0, 0, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 91, 93, 0, 0, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 39, 96, 0, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47, 0, 42, 0, 32, // normal
			0, 0, 33, 64, 35, 36, 37, 94, 38, 42, 40, 41, 95, 43, 0, 0, 81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 123, 125, 0, 0, 65, 83, 68, 70, 71, 72, 74, 75, 76, 58, 34, 126, 0, 124, 90, 88, 67, 86, 66, 78, 77, 60, 62, 63, 0, 42, 0, 32 // shift
		};
		// �������� � ���������� �����
		LangInfo & lang = m_currentLanguage->second;
		for (size_t i=0; i<116; i++) lang[i] = chars[i];

		// ��������� ������� ��� �������
		wchar_t nums[13] = {55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46};
		m_nums.resize(13);
		// �������� � ���������� �����
		for (size_t i=0; i<13; i++) m_nums[i] = nums[i];
	}

	void InputManager::loadCharSet(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.load(path::getFullPath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() != "MyGUI_LangInfo") return;

		// ����� ����� � ��������
		xml::VectorNode & langs = xml_root->getChilds();
		for (size_t i=0; i<langs.size(); i++) {
			xml::xmlNodePtr langInfo = langs[i];
			if (langInfo->getName() != "Lang") continue;

			// ������ �������� �����
			const xml::VectorAttributes & attrib = langInfo->getAttributes();
			std::string name;
			for (size_t ia=0; ia<attrib.size(); ia++) {
				// ������� ���� ������� - ��������
				const xml::PairAttributes & pairAttributes = attrib[ia];
				if (pairAttributes.first == "Name") name = pairAttributes.second;
			}

			const std::vector<std::string> & chars = util::split(langInfo->getBody());
			if (chars.size() == 116) {

				// ������� ��������� ���� �� ����� ���� ���
				MapLang::iterator iter = m_mapLanguages.find(name);
				if (iter != m_mapLanguages.end()) OGRE_EXCEPT(0, "name language is exist", "void InputManager::loadCharSet(const std::string & _file)");

				// ������� ����
				m_mapLanguages[name] = LangInfo();
				iter = m_mapLanguages.find(name);
				iter->second.resize(116);
				LangInfo & lang = iter->second;

				// � ��������� ���
				for (size_t j=0; j<116; j++) lang[j] = parseInt(chars[j]);

			} else {LOG("count char is not 116");}

		}
		// ����������� ��������� ��������, ��� ��� �� �������������� ��� ����������
		m_currentLanguage = m_mapLanguages.find(INPUT_DEFAULT_LANGUAGE);
	}

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{
		if (_widget != m_widgetKeyFocus) {
			if (m_widgetKeyFocus != null) m_widgetKeyFocus->OnKeyLostFocus(_widget);
			if (_widget != null) {
				if (_widget->isNeedKeyFocus()) {
					_widget->OnKeySetFocus(m_widgetKeyFocus);
					m_widgetKeyFocus = _widget;
					return;
				}
			}
			m_widgetKeyFocus = null;
		}
	}

	void InputManager::clearFocus()
	{
		m_widgetMouseFocus = null;
		m_widgetKeyFocus = null;
	}

} // namespace MyGUI
