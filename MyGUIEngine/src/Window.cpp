
#include "Window.h"
#include "WindowParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Window> WindowFactoryInstance("Window"); }
	// парсер команд
	namespace parser { WindowParser WindowParserInstance; }

	Window::Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetCaption(null), mWidgetX(null), mWidgetResize(null),
		mAlignCaption(ALIGN_NONE), mAlignX(ALIGN_NONE), mAlignResize(ALIGN_NONE)
	{

		// запомием размер скина
		mSkinSize = _info->getSize();

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinCaption");
			if (iter != param.end()) mSkinCaption = iter->second;
			iter = param.find("SkinX");
			if (iter != param.end()) mSkinX = iter->second;
			iter = param.find("SkinResize");
			if (iter != param.end()) mSkinResize = iter->second;
			iter = param.find("OffsetCaption");
			if (iter != param.end()) mOffsetCaption = floatRect::parse(iter->second);
			iter = param.find("OffsetX");
			if (iter != param.end()) mOffsetX = floatRect::parse(iter->second);
			iter = param.find("OffsetResize");
			if (iter != param.end()) mOffsetResize = floatRect::parse(iter->second);
			iter = param.find("AlignCaption");
			if (iter != param.end()) mAlignCaption = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignX");
			if (iter != param.end()) mAlignX = SkinManager::getInstance().parseAlign(iter->second);
			iter = param.find("AlignResize");
			if (iter != param.end()) mAlignResize = SkinManager::getInstance().parseAlign(iter->second);
		}
	}

	void Window::showWindowCaption(bool _show)
	{
		if ( (_show == (mWidgetCaption == null)) || (mSkinCaption.empty()) ) return;
		if (mWidgetCaption != null) WidgetManager::getInstance().destroyWidget(mWidgetCaption);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetCaption, mAlignCaption, mSkinSize, m_cx, m_cy);
			mWidgetCaption = createWidget("Button", mSkinCaption, offset.left, offset.top, offset.right, offset.bottom, mAlignCaption);
		}
	}

	void Window::showWindowX(bool _show)
	{
		if ( (_show == (mWidgetX == null)) || (mSkinX.empty()) ) return;
		if (mWidgetX != null) WidgetManager::getInstance().destroyWidget(mWidgetX);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetX, mAlignX, mSkinSize, m_cx, m_cy);
			mWidgetX = createWidget("Button", mSkinX, offset.left, offset.top, offset.right, offset.bottom, mAlignX);
		}
	}

	void Window::showWindowResize(bool _show)
	{
		if ( (_show == (mWidgetResize == null)) || (mSkinResize.empty()) ) return;
		if (mWidgetResize != null) WidgetManager::getInstance().destroyWidget(mWidgetResize);
		else {
			floatRect offset = WidgetManager::convertOffset(mOffsetResize, mAlignResize, mSkinSize, m_cx, m_cy);
			mWidgetResize = createWidget("Button", mSkinResize, offset.left, offset.top, offset.right, offset.bottom, mAlignResize);
		}
	}

	void Window::OnMouseSetFocus(WidgetPtr _old)
	{
		Widget::OnMouseSetFocus(_old);
	}

	void Window::OnMouseLostFocus(WidgetPtr _new)
	{
		Widget::OnMouseLostFocus(_new);
	}

	void Window::OnMouseButtonPressed(bool _left)
	{
		Widget::OnMouseButtonPressed(_left);
	}

	void Window::OnMouseButtonReleased(bool _left)
	{
		Widget::OnMouseButtonReleased(_left);
	}

} // namespace MyGUI