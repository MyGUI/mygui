/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{

	const int SCROLL_VIEW_MOUSE_WHEEL = 50; // колличество пикселей для колеса мыши
	const int SCROLL_VIEW_SCROLL_PAGE = 16; // колличество пикселей для кнопок скрола

	//MYGUI_RTTI_CHILD_IMPLEMENT2( ScrollView, Widget );

	ScrollView::ScrollView(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mIsFocus(false),
		mIsPressed(false),
		mVScroll(null),
		mHScroll(null),
		mShowHScroll(true),
		mShowVScroll(true),
		mVRange(0),
		mHRange(0),
		mClient(null),
		mAlignCanvas(Align::Center)
	{
		initialiseWidgetSkin(_info);
	}

	ScrollView::~ScrollView()
	{
		shutdownWidgetSkin();
	}

	void ScrollView::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void ScrollView::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mClient, "widget already assigned");
				mClient = (*iter);
				mClient->eventMouseSetFocus = newDelegate(this, &ScrollView::notifyMouseSetFocus);
				mClient->eventMouseLostFocus = newDelegate(this, &ScrollView::notifyMouseLostFocus);
				mClient->eventMouseWheel = newDelegate(this, &ScrollView::notifyMouseWheel);

				// создаем холт, реальный владелец детей
				mWidgetClient = mClient->createWidget<Widget>("Default", IntCoord(), Align::Default);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ScrollView::notifyMouseWheel);
				mWidgetClient->eventMouseSetFocus = newDelegate(this, &ScrollView::notifyMouseSetFocus);
				mWidgetClient->eventMouseLostFocus = newDelegate(this, &ScrollView::notifyMouseLostFocus);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "VScroll") {
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = (*iter)->castType<VScroll>();
				mVScroll->eventScrollChangePosition = newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "HScroll") {
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = (*iter)->castType<HScroll>();
				mHScroll->eventScrollChangePosition = newDelegate(this, &ScrollView::notifyScrollChangePosition);
			}
		}

		MYGUI_ASSERT(null != mClient, "Child Widget Client not found in skin (ScrollView must have Client)");

		updateView();
	}

	void ScrollView::shutdownWidgetSkin()
	{
		mWidgetClient = null;
		mVScroll = null;
		mClient = null;
		mHScroll = null;
	}

	void ScrollView::notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old)
	{
		if ( (_old == mClient) || (mIsFocus) ) return;
		mIsFocus = true;
		updateScrollViewState();
	}

	void ScrollView::notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		if ( (_new == mClient) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateScrollViewState();
	}

	void ScrollView::onKeySetFocus(WidgetPtr _old)
	{
		if (false == mIsPressed) {
			mIsPressed = true;
			updateScrollViewState();
		}
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onKeySetFocus(_old);
	}

	void ScrollView::onKeyLostFocus(WidgetPtr _new)
	{
		if (mIsPressed) {
			mIsPressed = false;
			updateScrollViewState();
		}
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onKeyLostFocus(_new);
	}

	void ScrollView::updateScrollViewState()
	{
		if (!mEnabled) setState("disabled");
		else if (mIsPressed) {
			if (mIsFocus) setState("pushed");
			else setState("normal_checked");
		}
		else if (mIsFocus) setState("highlighted");
		else setState("normal");
	}

	void ScrollView::setPosition(const IntPoint & _point)
	{
		Widget::setPosition(_point);
	}

	void ScrollView::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateView();
	}

	void ScrollView::setCoord(const IntCoord & _coord)
	{
		Widget::setCoord(_coord);
		updateView();
	}

	void ScrollView::setTextAlign(Align _align)
	{
		Widget::setTextAlign(_align);
		// так как мы сами рулим смещениями
		updateView();
	}

	void ScrollView::updateView()
	{

		// проверяем скролы
		updateScroll();

		// размер контекста
		IntSize size = mWidgetClient->getSize();
		// текущее смещение контекста
		IntPoint point(-mWidgetClient->getLeft(), -mWidgetClient->getTop());
		// расчетное смещение
		IntPoint offset = point;

		if (size.width > mClient->getWidth()) {
			// максимальный выход влево
			if ((offset.left + mClient->getWidth()) > size.width) {
				offset.left = size.width - mClient->getWidth();
			}
			// максимальный выход вправо
			else if (offset.left < 0) {
				offset.left = 0;
			}
		}
		else {
			if (mAlignCanvas.isLeft()) {
				offset.left = 0;
			}
			else if (mAlignCanvas.isRight()) {
				offset.left = size.width - mClient->getWidth();
			}
			else {
				offset.left = (size.width - mClient->getWidth()) / 2;
			}
		}

		if (size.height > mClient->getHeight()) {
			// максимальный выход вверх
			if ((offset.top + mClient->getHeight()) > size.height) {
				offset.top = size.height - mClient->getHeight();
			}
			// максимальный выход вниз
			else if (offset.top < 0) {
				offset.top = 0;
			}
		}
		else {
			if (mAlignCanvas.isTop()) {
				offset.top = 0;
			}
			else if (mAlignCanvas.isBottom()) {
				offset.top = size.height - mClient->getHeight();
			}
			else {
				offset.top = (size.height - mClient->getHeight()) / 2;
			}
		}

		if (offset != point) {
			if (null != mVScroll) mVScroll->setScrollPosition(offset.top);
			if (null != mHScroll) mHScroll->setScrollPosition(offset.left);
			mWidgetClient->setPosition(-offset.left, -offset.top);
		}
	}

	void ScrollView::updateScroll()
	{
		IntSize size = mWidgetClient->getSize();

		// вертикальный не помещается
		if (size.height > mClient->getHeight()) {
			if (mVScroll != null) {
				if (( ! mVScroll->isShow()) && (mShowVScroll)) {
					mVScroll->show();
					mClient->setSize(mClient->getWidth() - mVScroll->getWidth(), mClient->getHeight());

					// размер может измениться
					//size = mWidgetClient->getSize();

					if (mHScroll != null) {
						mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

						// если показали вертикальный скрол бар, уменьшилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет показа
						if ((size.width > mClient->getWidth()) && ( ! mHScroll->isShow()) && (mShowHScroll)) {
							mHScroll->show();
							mClient->setSize(mClient->getWidth(), mClient->getHeight() - mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

							// размер может измениться
							//size = mWidgetClient->getSize();
						}
					}
				}
			}
		}
		// вертикальный помещается
		else {
			if (mVScroll != null) {
				if (mVScroll->isShow()) {
					mVScroll->hide();
					mClient->setSize(mClient->getWidth() + mVScroll->getWidth(), mClient->getHeight());

					// размер может измениться
					//size = mWidgetClient->getSize();

					if (mHScroll != null) {
						mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

						// если скрыли вертикальный скрол бар, увеличилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет скрытия
						if ((size.width <= mClient->getWidth()) && (mHScroll->isShow())) {
							mHScroll->hide();
							mClient->setSize(mClient->getWidth(), mClient->getHeight() + mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

							// размер может измениться
							//size = mWidgetClient->getSize();
						}
					}
				}
			}
		}


		// горизонтальный не помещается
		if (size.width > mClient->getWidth()) {
			if (mHScroll != null) {
				if (( ! mHScroll->isShow()) && (mShowHScroll)) {
					mHScroll->show();
					mClient->setSize(mClient->getWidth(), mClient->getHeight() - mHScroll->getHeight());

					// размер может измениться
					//size = mWidgetClient->getSize();

					if (mVScroll != null) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

						// если показали горизонтальный скрол бар, уменьшилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет показа
						if ((size.height > mClient->getHeight()) && ( ! mVScroll->isShow()) && (mShowVScroll)) {
							mVScroll->show();
							mClient->setSize(mClient->getWidth() - mVScroll->getWidth(), mClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

							// размер может измениться
							//size = mWidgetClient->getSize();
						}
					}
				}
			}
		}
		// горизонтальный помещается
		else {
			if (mHScroll != null) {
				if (mHScroll->isShow()) {
					mHScroll->hide();
					mClient->setSize(mClient->getWidth(), mClient->getHeight() + mHScroll->getHeight());

					// размер может измениться
					//size = mWidgetClient->getSize();

					if (mVScroll != null) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

						// если скрыли горизонтальный скрол бар, увеличилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет скрытия
						if ((size.height <= mClient->getHeight()) && (mVScroll->isShow())) {
							mVScroll->hide();
							mClient->setSize(mClient->getWidth() + mVScroll->getWidth(), mClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

							// размер может измениться
							//size = mWidgetClient->getSize();
						}
					}
				}
			}
		}

		mVRange = (mClient->getHeight() >= size.height) ? 0 : size.height - mClient->getHeight();
		mHRange = (mClient->getWidth() >= size.width) ? 0 : size.width - mClient->getWidth();

		size_t page = SCROLL_VIEW_SCROLL_PAGE;
		if (mVScroll != null) {
			mVScroll->setScrollPage(page);
			mVScroll->setScrollViewPage(mCoord.width > (int)page ? mCoord.width : page);
			mVScroll->setScrollRange(mVRange + 1);
			if (size.height) mVScroll->setTrackSize( int( float(mVScroll->getLineSize()) * float(mClient->getHeight()) / float(size.height) ));
		}
		if (mHScroll != null) {
			mHScroll->setScrollPage(page);
			mHScroll->setScrollViewPage(mCoord.height > (int)page ? mCoord.height : page);
			mHScroll->setScrollRange(mHRange + 1);
			if (size.width) mHScroll->setTrackSize( int( float(mHScroll->getLineSize()) * float(mClient->getWidth()) / float(size.width) ));
		}

	}

	void ScrollView::notifyScrollChangePosition(WidgetPtr _sender, size_t _position)
	{
		if (_sender == mVScroll) {
			IntPoint point = mWidgetClient->getPosition();
			point.top = -(int)_position;
			mWidgetClient->setPosition(point);
		}
		else if (_sender == mHScroll) {
			IntPoint point = mWidgetClient->getPosition();
			point.left = -(int)_position;
			mWidgetClient->setPosition(point);
		}
	}

	void ScrollView::notifyMouseWheel(WidgetPtr _sender, int _rel)
	{
		if (mVRange != 0) {
			IntPoint point = mWidgetClient->getPosition();
			int offset = -point.top;
			if (_rel < 0) offset += SCROLL_VIEW_MOUSE_WHEEL;
			else  offset -= SCROLL_VIEW_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mVRange) offset = mVRange;

			if (offset != point.top) {
				point.top = -offset;
				if (mVScroll != null) {
					mVScroll->setScrollPosition(offset);
				}
				mWidgetClient->setPosition(point);
			}
		}
		else if (mHRange != 0) {
			IntPoint point = mWidgetClient->getPosition();
			int offset = -point.left;
			if (_rel < 0) offset += SCROLL_VIEW_MOUSE_WHEEL;
			else  offset -= SCROLL_VIEW_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mHRange) offset = mHRange;

			if (offset != point.left) {
				point.left = -offset;
				if (mHScroll != null) {
					mHScroll->setScrollPosition(offset);
				}
				mWidgetClient->setPosition(point);
			}
		}
	}

	WidgetPtr ScrollView::baseCreateWidget(WidgetType _behaviour, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		return mWidgetClient->createWidgetT(_behaviour, _type, _skin, _coord, _align, _layer, _name);
	}

} // namespace MyGUI
