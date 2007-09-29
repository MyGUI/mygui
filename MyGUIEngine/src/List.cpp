
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
		mWidgetScroll(null), mWidgetClient(null)
	{
		std::string skinScroll, skinClient;
		FloatRect offsetScroll;
		FloatRect offsetClient;

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinScroll");
			if (iter != param.end()) skinScroll = iter->second;
			iter = param.find("OffsetScroll");
			if (iter != param.end()) offsetScroll = util::parseFloatRect(iter->second);
			iter = param.find("SkinClient");
			if (iter != param.end()) skinClient = iter->second;
			iter = param.find("OffsetClient");
			if (iter != param.end()) offsetClient = util::parseFloatRect(iter->second);
			iter = param.find("SkinLine");
			if (iter != param.end()) mSkinLine = iter->second;
			iter = param.find("HeightLine");
			if (iter != param.end()) mHeightLine = util::parseSizeT(iter->second);
		}

		ASSERT(skinScroll.size());
		ASSERT(skinClient.size());
		ASSERT(mSkinLine.size());
		ASSERT(mHeightLine);

		offsetScroll = WidgetManager::convertOffset(offsetScroll, ALIGN_RIGHT | ALIGN_VSTRETCH, _info->getSize(), m_cx, m_cy);
		mWidgetScroll = static_cast<VScrollPtr>(createWidget("VScroll", skinScroll, offsetScroll.left, offsetScroll.top, offsetScroll.right, offsetScroll.bottom, ALIGN_RIGHT | ALIGN_VSTRETCH));
		mWidgetScroll->eventScrollChangePosition = newDelegate(this, &List::notifyScrollChangePosition);

		offsetClient = WidgetManager::convertOffset(offsetClient, ALIGN_STRETCH, _info->getSize(), m_cx, m_cy);
		mWidgetClient = createWidget("Widget", skinClient, offsetClient.left, offsetClient.top, offsetClient.right, offsetClient.bottom, ALIGN_STRETCH);

		// делегаты для событий
//		mWidgetCaption->eventMouseButtonPressed = newDelegate(this, &Window::notifyMousePressed);
//		mWidgetCaption->eventMouseMove = newDelegate(this, &Window::notifyMouseMovedCaption);

//		mHeightLine = 20;
		mCountLine = 10;

//		mWidgetScroll->setScrollRange((mHeightLine * mCountLine) - m_cy);
		updateScroll();
	}

	void List::size(int _cx, int _cy)
	{
		Widget::size(_cx, _cy);
		updateScroll();
		updateLine();
	}

	void List::move(int _x, int _y, int _cx, int _cy)
	{
		Widget::move(_x, _y, _cx, _cy);
		updateScroll();
		updateLine();
	}

	void List::updateScroll()
	{
		int pos = (int)(mHeightLine * mCountLine) - mWidgetClient->height();

		if ( (pos <= 1) || (mWidgetScroll->left() <= mWidgetClient->left()) ) {
			if (mWidgetScroll->isShow()) {
				mWidgetScroll->show(false);
				// увеличиваем клиентскую зону на ширину скрола
				mWidgetClient->size(mWidgetClient->width() + mWidgetScroll->width(), mWidgetClient->height());
			}
			return;
		}
		if (!mWidgetScroll->isShow()) {
			mWidgetClient->size(mWidgetClient->width() - mWidgetScroll->width(), mWidgetClient->height());
			mWidgetScroll->show(true);
		}

		mWidgetScroll->setScrollRange(pos);
//		OUT((mHeightLine * mCountLine) - m_cy);
	}

	void List::updateLine()
	{
		// старые значения размеров
		static int old_cx=0, old_cy=0;

		// если ширина измениласть то вытягиваем линии
		if (old_cx != m_cx) {
			for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
				mWidgetLines[pos]->size(mWidgetClient->width(), mWidgetLines[pos]->height());
			}
		}

		// если высота увеличивалась то добавляем виджеты
		if (old_cy < m_cy) {
			size_t height = mWidgetLines.size() * mHeightLine;
			while ((int)height < mWidgetClient->height()) {
				// создаем линию
				WidgetPtr line = mWidgetClient->createWidget("Button", mSkinLine, 0, (int)height, mWidgetClient->width(), (int)mHeightLine, ALIGN_TOP | ALIGN_LEFT);
				// и сохраняем
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}
		}

		old_cx = m_cx;
		old_cy = m_cy;
	}

	void List::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel)
	{
		// смещение с отрицательной стороны
		int offset = 0 - (_rel % (int)mHeightLine);

		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			mWidgetLines[pos]->move(0, offset);
			offset += (int)mHeightLine;
			if ((int)offset >= mWidgetClient->height()) break;
		}
	}

} // namespace MyGUI