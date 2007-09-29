
#include "List.h"
//#include "ListParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<List> ListFactoryInstance("List"); }
	// парсер команд
//	namespace parser { WindowParser WindowParserInstance; }

	List::List(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetScroll(null)
	{
		std::string skin;

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinScroll");
			if (iter != param.end()) skin = iter->second;
			iter = param.find("OffsetScroll");
			if (iter != param.end()) mOffsetScroll = util::parseFloatRect(iter->second);
		}

		ASSERT(skin.size());

		FloatRect offset = WidgetManager::convertOffset(mOffsetScroll, ALIGN_RIGHT | ALIGN_VSTRETCH, _info->getSize(), m_cx, m_cy);
		mWidgetScroll = static_cast<VScrollPtr>(createWidget("VScroll", skin, offset.left, offset.top, offset.right, offset.bottom, ALIGN_RIGHT | ALIGN_VSTRETCH));

		// делегаты для событий
//		mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
//		mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);

		mHeightLine = 20;
		mCountLine = 10;

		mWidgetScroll->setScrollRange((mHeightLine * mCountLine) - m_cy);
	}

	void List::size(int _cx, int _cy)
	{
		Widget::size(_cx, _cy);
		updateScroll();
	}

	void List::move(int _x, int _y, int _cx, int _cy)
	{
		Widget::move(_x, _y, _cx, _cy);
		updateScroll();
	}

	void List::updateScroll()
	{
		int pos = (int)(mHeightLine * mCountLine) - m_cy;
		if (pos <= 0) {
			mWidgetScroll->show(false);
			return;
		}
		if (!mWidgetScroll->isShow()) mWidgetScroll->show(true);

		mWidgetScroll->setScrollRange(pos);
//		OUT((mHeightLine * mCountLine) - m_cy);
	}


} // namespace MyGUI