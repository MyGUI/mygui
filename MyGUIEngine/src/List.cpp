
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
		mWidgetScroll(null), mWidgetClient(null),
		mOffsetTop(0),
		mTopIndex(0),
		mRangeIndex(0),
		mLastRedrawLine(0)
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
			if (iter != param.end()) mHeightLine = util::parseInt(iter->second);
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

//		mCountLine = 1;
		for (size_t pos=0; pos<10; pos++) mStringArray.push_back(util::toString(pos));

		updateScroll();
		updateLine();
//		changeIndex();
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
		mRangeIndex = (mHeightLine * (int)mStringArray.size()) - mWidgetClient->height();

		if ( (mRangeIndex < 1) || (mWidgetScroll->left() <= mWidgetClient->left()) ) {
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

		mWidgetScroll->setScrollRange(mRangeIndex + 1);
	}

	void List::updateLine(bool _reset)
	{
		// старые значения размеров
		static int old_cx=0, old_cy=0;
		if (_reset) {old_cx=0;old_cy=0;}

		// если ширина измениласть то вытягиваем линии
		if (old_cx != m_cx) {
			for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
				mWidgetLines[pos]->size(mWidgetClient->width(), mWidgetLines[pos]->height());
			}
		}

		// позиция скролла
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// если высота увеличивалась то добавляем виджеты
		if (old_cy < m_cy) {

			int height = (int)mWidgetLines.size() * mHeightLine;

			// до тех пор, пока не достигнем максимального колличества, и всегда на одну больше
			while ( (height <= (mWidgetClient->height() + mHeightLine)) && (mWidgetLines.size() < mStringArray.size()) ) {
				// создаем линию
				WidgetPtr line = mWidgetClient->createWidget("Button", mSkinLine, 0, height, mWidgetClient->width(), mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// и сохраняем
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}

			// проверяем на возможность не менять положение списка
			if (position >= mRangeIndex) {

				// размер всех помещается в клиент
				if (((int)mStringArray.size()*mHeightLine) <= mWidgetClient->height()) {

					// обнуляем, если надо
					if (position || mOffsetTop || mTopIndex) {

						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;

						// выравниваем
						int offset = 0;
						for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
							mWidgetLines[pos]->move(0, offset);
							offset += mHeightLine;
						}
					}

				} else {

					// прижимаем список к нижней границе
					int count = mWidgetClient->height() / mHeightLine;
					mOffsetTop = mHeightLine - (mWidgetClient->height() % mHeightLine);

					if (mOffsetTop == mHeightLine) {
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mStringArray.size() - count - 1;

					// выравниваем
					int offset = 0 - mOffsetTop;
					for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
						mWidgetLines[pos]->move(0, offset);
						offset += mHeightLine;
					}

					// высчитываем положение, должно быть максимальным
					position = top * mHeightLine + mOffsetTop;

					// если индех изменился, то перерисовываем линии
					if (top != mTopIndex) {
						mTopIndex = top;
						changeIndex();
					}

				}
			}// else {
				// увеличился размер, но прокрутки вниз небыло, обновляем линии снизу
				changeIndex(mLastRedrawLine);
			//}
		} // if (old_cy < m_cy) {

		// просчитываем положение скролла
		mWidgetScroll->setScrollPosition(position);

		old_cx = m_cx;
		old_cy = m_cy;
	}

	void List::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel)
	{
		mOffsetTop = (_rel % mHeightLine);

		// смещение с отрицательной стороны
		int offset = 0 - mOffsetTop;

		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			mWidgetLines[pos]->move(0, offset);
			offset += mHeightLine;
		}

		// если индех изменился, то перерисовываем линии
		int top = (_rel / mHeightLine);
		if (top != mTopIndex) {
			mTopIndex = top;
			changeIndex();
		}

		// прорисовываем все нижние строки, если они появились
		changeIndex(mLastRedrawLine);

	}

	void List::changeIndex(size_t _start)
	{
		// перерисовываем линии, только те, что видны
		size_t pos = _start;
		for (; pos<mWidgetLines.size(); pos++) {
			// индекс в нашем массиве
			size_t index = pos + (size_t)mTopIndex;

			// не будем заходить слишком далеко
			if (index >= mStringArray.size()) {
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}
			if (mWidgetLines[pos]->top() > mWidgetClient->height()) {
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}

			// если был скрыт, то покажем
			if (!mWidgetLines[pos]->isShow()) mWidgetLines[pos]->show(true);
			// обновляем текст
			mWidgetLines[pos]->setCaption(mStringArray[index]);
			//OUT(pos);

		}

		// если цикл весь прошли, то ставим максимальную линию
		if (pos >= mWidgetLines.size()) mLastRedrawLine = pos;
	}

	// перерисовывает индекс
	void List::redrawIndex(size_t _index)
	{
		// невидно
		if (_index < (size_t)mTopIndex) return;
		_index -= (size_t)mTopIndex;
		// тоже невидно
		if (_index > mLastRedrawLine) return;
		// перерисовываем
		mWidgetLines[_index]->setCaption(mStringArray[_index + mTopIndex]);
	}

	void List::insertIndexString(size_t _index, const Ogre::DisplayString & _item)
	{
	}

	void List::deleteIndexString(size_t _index)
	{
		// доверяй, но проверяй
		assert(_index < mStringArray.size());

		// удяляем физически строку
		_deleteString(_index);

		// если виджетов стало больше , то скрываем крайний
		if (mWidgetLines.size() > mStringArray.size()) {
			mWidgetLines[mStringArray.size()]->show(false);
		}

		// высчитывам положение удаляемой строки
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

		// строка, до первого видимого элемента
		if (_index < (size_t)mTopIndex) {
			mTopIndex --;
			// просчитываем положение скролла
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);

		// строка, после последнего видимого элемента
		} else if (mWidgetClient->height() < offset) {
			// просчитываем положение скролла
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);

		// строка в видимой области
		} else {

			// обновляем все
			updateScroll();
			updateLine(true);
			changeIndex(0);

		}
	}

	void List::_deleteString(size_t _index)
	{
		for (size_t pos=_index+1; pos<mStringArray.size(); pos++) {
			mStringArray[pos-1] = mStringArray[pos];
		}
		mStringArray.pop_back();
	}

} // namespace MyGUI