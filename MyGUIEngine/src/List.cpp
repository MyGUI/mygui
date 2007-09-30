
#include "List.h"
//#include "ListParser.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<List> ListFactoryInstance("List"); }
	// ������ ������
//	namespace parser { WindowParser WindowParserInstance; }

	List::List(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetScroll(null), mWidgetClient(null),
		mOffsetTop(0),
		mTopIndex(0),
		mRangeIndex(0)
	{
		std::string skinScroll, skinClient;
		FloatRect offsetScroll;
		FloatRect offsetClient;

		// ������ ��������
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
		int pos = (mHeightLine * (int)mStringArray.size()) - mWidgetClient->height();

		if ( (pos < 1) || (mWidgetScroll->left() <= mWidgetClient->left()) /*|| (mCountLine < 2)*/ ) {
			if (mWidgetScroll->isShow()) {
				mWidgetScroll->show(false);
				// ����������� ���������� ���� �� ������ ������
				mWidgetClient->size(mWidgetClient->width() + mWidgetScroll->width(), mWidgetClient->height());
			}
			return;
		}
		if (!mWidgetScroll->isShow()) {
			mWidgetClient->size(mWidgetClient->width() - mWidgetScroll->width(), mWidgetClient->height());
			mWidgetScroll->show(true);
		}

		mRangeIndex = pos;
		mWidgetScroll->setScrollRange(mRangeIndex + 1);
	}

	void List::updateLine()
	{
		// ������ �������� ��������
		static int old_cx=0, old_cy=0;

		// ���� ������ ����������� �� ���������� �����
		if (old_cx != m_cx) {
			for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
				mWidgetLines[pos]->size(mWidgetClient->width(), mWidgetLines[pos]->height());
			}
		}

		// ������� �������
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// ���� ������ ������������� �� ��������� �������
		if (old_cy < m_cy) {

			int height = (int)mWidgetLines.size() * mHeightLine;

			// �� ��� ���, ���� �� ��������� ������������� �����������, � ������ �� ���� ������
			while ( (height <= (mWidgetClient->height() + mHeightLine)) && (mWidgetLines.size() < mStringArray.size()) ) {
				// ������� �����
				WidgetPtr line = mWidgetClient->createWidget("Button", mSkinLine, 0, height, mWidgetClient->width(), mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// � ���������
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}

			// ��������� �� ����������� �� ������ ��������� ������
			if (position >= mRangeIndex) {

//				OUT(position, "   ", mRangeIndex);

//				OUT((mCountLine*mHeightLine),"   ", mWidgetClient->height());
				// ������ ���� ���������� � ������
				if (((int)mStringArray.size()*mHeightLine) <= mWidgetClient->height()) {

//					OUT("pemesh");
					// ��������, ���� ����
					if (position || mOffsetTop || mTopIndex) {

						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;

						// �����������
						int offset = 0;
						for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
							mWidgetLines[pos]->move(0, offset);
							offset += mHeightLine;
						}

					}

				} else {

					// ��������� ������ � ������ �������
					int count = mWidgetClient->height() / mHeightLine;
					mOffsetTop = mHeightLine - (mWidgetClient->height() % mHeightLine);

					if (mOffsetTop == mHeightLine) {
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mStringArray.size() - count - 1;

//					OUT("calc : ", mTopIndex, "  ", mOffsetTop);

					// �����������
					int offset = 0 - mOffsetTop; // << ���������
					for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
						mWidgetLines[pos]->move(0, offset);
						offset += mHeightLine;
					}

					// ����������� ���������, ������ ���� ������������
					position = top * mHeightLine + mOffsetTop;

					// ���� ����� ���������, �� �������������� �����
					if (top != mTopIndex) {
						mTopIndex = top;
						changeIndex();
					}

//					OUT(position, "  ", mRangeIndex);

				}


			}

		}

		// ������������ ��������� �������
		mWidgetScroll->setScrollPosition(position);

		old_cx = m_cx;
		old_cy = m_cy;
	}

	void List::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel)
	{
		mOffsetTop = (_rel % mHeightLine);

		// �������� � ������������� �������
		int offset = 0 - mOffsetTop;

		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			mWidgetLines[pos]->move(0, offset);
			offset += mHeightLine;
		}

		// ���� ����� ���������, �� �������������� �����
		int top = (_rel / mHeightLine);
		if (top != mTopIndex) {
			mTopIndex = top;
			changeIndex();
		}

//		OUT("scroll : ", mTopIndex, "  ", mOffsetTop);
	}

	void List::changeIndex()
	{
		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			size_t index = pos + (size_t)mTopIndex;
			if (index >= mStringArray.size()) break;
			mWidgetLines[pos]->setCaption(mStringArray[index]);
//			mWidgetLines[pos]->move(0, offset);
//			offset += mHeightLine;
		}
	}

} // namespace MyGUI