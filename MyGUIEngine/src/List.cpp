
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

		mCountLine = 10;

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
		int pos = (int)(mHeightLine * mCountLine) - mWidgetClient->height();

		if ( (pos < 1) || (mWidgetScroll->left() <= mWidgetClient->left()) ) {
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

			size_t height = mWidgetLines.size() * mHeightLine;

			// �� ��� ���, ���� �� ��������� ������������� �����������, � ������ �� ���� ������
			while ( ((int)height <= (mWidgetClient->height() + mHeightLine)) && (mWidgetLines.size() < mCountLine) ) {
				// ������� �����
				WidgetPtr line = mWidgetClient->createWidget("Button", mSkinLine, 0, (int)height, mWidgetClient->width(), (int)mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// � ���������
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}

			// ��������� �� ����������� �� ������ ��������� ������
			if (position >= mRangeIndex) {

//				OUT(position, "   ", mRangeIndex);

//				OUT((mCountLine*mHeightLine),"   ", mWidgetClient->height());
				// ������ ���� ���������� � ������
				if ((mCountLine*mHeightLine) <= mWidgetClient->height()) {

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
							offset += (int)mHeightLine;
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

					mTopIndex = mCountLine - count - 1;

//					OUT("calc : ", mTopIndex, "  ", mOffsetTop);

					// �����������
					int offset = 0 - mOffsetTop; // << ���������
					for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
						mWidgetLines[pos]->move(0, offset);
						offset += (int)mHeightLine;
					}

					// ����������� ���������, ������ ���� ������������
					position = mTopIndex * mHeightLine + mOffsetTop;

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
		mTopIndex  = (_rel / (int)mHeightLine);
		mOffsetTop = (_rel % (int)mHeightLine);

		// �������� � ������������� �������
		int offset = 0 - mOffsetTop;

		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			mWidgetLines[pos]->move(0, offset);
			offset += (int)mHeightLine;
		}

//		OUT("scroll : ", mTopIndex, "  ", mOffsetTop);
	}

} // namespace MyGUI