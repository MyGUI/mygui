
#include "VScroll.h"
//#include "VScrollParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<VScroll> VScrollFactoryInstance("VScroll"); }
	// парсер команд
//	namespace parser { VScrollParser VScrollParserInstance; }

	VScroll::VScroll(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_widgetTop(null), m_widgetBottom(null), m_widgetTrack(null)
	{

		// запомием размер скина
		intSize skinSize = _info->getSize();

		std::string skin;
		floatRect offset;
		char align;

		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("SkinTop");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetTop");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignTop");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		m_widgetTop = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);

		iter = param.find("SkinBottom");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetBottom");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignBottom");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		m_widgetBottom = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);

		iter = param.find("SkinTrack");
		if (iter != param.end()) skin = iter->second;
		iter = param.find("OffsetTrack");
		if (iter != param.end()) offset = floatRect::parse(iter->second);
		iter = param.find("AlignTrack");
		if (iter != param.end()) align = SkinManager::getInstance().parseAlign(iter->second);

		offset = WidgetManager::convertOffset(offset, align, skinSize, m_cx, m_cy);
		m_widgetTrack = createWidget("Button", skin, offset.left, offset.top, offset.right, offset.bottom, align);
	}

/*	void Button::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		m_isFocus = true;
		updateButtonState();
	}

	void Button::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		m_isFocus = false;
		updateButtonState();
	}

	void Button::_onMouseButtonPressed(bool _left)
	{
		Widget::_onMouseButtonPressed(_left);
		if (!_left) return;
		m_isPressed = true;
		updateButtonState();
	}

	void Button::_onMouseButtonReleased(bool _left)
	{
		Widget::_onMouseButtonReleased(_left);
		if (!_left) return;
		m_isPressed = false;
		updateButtonState();
	}*/

} // namespace MyGUI