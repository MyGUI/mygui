
#include "WidgetEvent.h"

namespace widget
{

	WidgetEvent::WidgetEvent() :
		m_widgetEventSender(0),
		m_needKeyFocus(false)
	{
	}

	void WidgetEvent::OnMouseLostFocus(WidgetPtr _new)
	{
		eventMouseLostFocus(m_widgetEventSender, _new);
	}

	void WidgetEvent::OnMouseSetFocus(WidgetPtr _old)
	{
		eventMouseSetFocus(m_widgetEventSender, _old);
	}

	void WidgetEvent::OnMouseMove(int _x, int _y)
	{
		eventMouseMove(m_widgetEventSender, _x, _y);
	}

	void WidgetEvent::OnMouseSheel(int _rel)
	{
		eventMouseSheel(m_widgetEventSender, _rel);
	}

	void WidgetEvent::OnMouseButtonPressed(bool _left)
	{
		eventMouseButtonPressed(m_widgetEventSender, _left);
	}

	void WidgetEvent::OnMouseButtonReleased(bool _left)
	{
		eventMouseButtonReleased(m_widgetEventSender, _left);
	}

	void WidgetEvent::OnMouseButtonClick(bool _double)
	{
		eventMouseButtonClick(m_widgetEventSender, _double);
	}

	void WidgetEvent::OnKeyLostFocus(WidgetPtr _new)
	{
		eventKeyLostFocus(m_widgetEventSender, _new);
	}

	void WidgetEvent::OnKeySetFocus(WidgetPtr _old)
	{
		eventKeySetFocus(m_widgetEventSender, _old);
	}

	void WidgetEvent::OnKeyButtonPressed(int _key, wchar_t _char)
	{
		eventKeyButtonPressed(m_widgetEventSender, _key, _char);
	}

	void WidgetEvent::OnKeyButtonReleased(int _key)
	{
		eventKeyButtonReleased(m_widgetEventSender, _key);
	}

} // namespace widget