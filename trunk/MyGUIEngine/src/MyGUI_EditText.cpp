/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_EditText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const size_t SIMPLETEXT_COUNT_VERTEX = 32 * VERTEX_IN_QUAD;
	const float EDIT_TEXT_WIDTH_CURSOR = 2.0f;
	const int EDIT_MIN_BREAK_WORD_WIDTH = 10;

	struct RollBackSave
	{
		RollBackSave() : rollback(false) {}

		inline void set(
			VectorCharInfo::iterator & _space_rollback,
			Ogre::UTFString::const_iterator & _space_point,
			size_t _count,
			float _real_length
		)
		{
			space_rollback = _space_rollback;
			space_point = _space_point;
			count = _count;
			real_lenght = _real_length;
			rollback = true;
		}

		inline void reset() { rollback = false; }

		VectorCharInfo::iterator space_rollback;
		Ogre::UTFString::const_iterator space_point;
		bool rollback;
		size_t count;
		float real_lenght;
	};

	// ��������� �� ����� � ���������
	#define __MYGUI_DRAW_QUAD(buf, v_left, v_top, v_rignt, v_bottom, v_z, col, t_left, t_top, t_right, t_bottom, count) \
	{\
		buf[0].x = v_left; buf[0].y = v_top; buf[0].z = v_z; \
		buf[0].colour = col; buf[0].u = t_left; 	buf[0].v = t_top; \
		\
		buf[1].x = v_left; buf[1].y = v_bottom; buf[1].z = v_z; \
		buf[1].colour = col; buf[1].u = t_left; buf[1].v = t_bottom; \
		\
		buf[2].x = v_rignt; buf[2].y = v_top; buf[2].z = v_z; \
		buf[2].colour = col; buf[2].u = t_right; buf[2].v = t_top; \
		\
		buf[3].x = v_rignt; buf[3].y = v_top; buf[3].z = v_z; \
		buf[3].colour = col; buf[3].u = t_right; buf[3].v = t_top; \
		\
		buf[4].x = v_left; buf[4].y = v_bottom; buf[4].z = v_z; \
		buf[4].colour = col; buf[4].u = t_left; buf[4].v = t_bottom; \
		\
		buf[5].x = v_rignt; buf[5].y = v_bottom; buf[5].z = v_z; \
		buf[5].colour = col; buf[5].u = t_right; buf[5].v = t_bottom; \
		\
		buf += VERTEX_IN_QUAD; \
		count += VERTEX_IN_QUAD; \
	}


	EditText::EditText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubWidgetTextInterface(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentCoord(_info.coord),
		mTextOutDate(false),
		mCurrentColour(0x00FFFFFF), mInverseColour(0x00000000),
		mCurrentAlpha(0xFF000000),
		mColour(Ogre::ColourValue::White),
		mAlpha(ALPHA_MAX),
		mFontHeight(0),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mItemKeeper(null),
		mRenderItem(null),
		mBackgroundNormal(true),
		mStartSelect(0), mEndSelect(0),
		mCursorPosition(0), mShowCursor(false),
		mTextAlign(Align::Default),
		mShiftText(false),
		mBreakLine(false),
		mOldWidth(0)
	{
		mManager = LayerManager::getInstancePtr();

		// ����� ���������
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());

		Ogre::Root::getSingleton().convertColourValue(mColour, &mCurrentColour);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		mManualView = true;
	}

	EditText::~EditText()
	{
	}

	void EditText::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void EditText::_setAlign(const IntSize& _size, bool _update)
	{

		if (mBreakLine) {
			if (mOldWidth != _size.width) {
				mOldWidth = _size.width;
				mTextOutDate = true;
			}
		}

		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������
		if (mAlign.isHStretch()) {
			// �����������
			mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
			need_update = true;
			mIsMargin = true; // ��� ��������� �������� ��� �������������
		}
		else if (mAlign.isRight()) {
			// ������� �� ������� ����
			mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
			need_update = true;
		}
		else if (mAlign.isHCenter()) {
			// ������������ �� ����������� ��� ����������
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch()) {
			// �����������
			mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
			need_update = true;
			mIsMargin = true; // ��� ��������� �������� ��� �������������
		}
		else if (mAlign.isBottom()) {
			// ������� �� ������� ����
			mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
			need_update = true;
		}
		else if (mAlign.isVCenter()) {
			// ������������ �� ��������� ��� ����������
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void EditText::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				//mEmptyView = true;
				// ���������� ������� ���������
				mIsMargin = margin;

				// �������� ����� �������
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // �� �������� ��� ���� ��������
			mCurrentCoord.width = getViewWidth();
			mCurrentCoord.height = getViewHeight();

		}

		// ���������� ������� ���������
		mIsMargin = margin;

		// ���� ���� ��� �����, �� �������
		//mEmptyView = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setCaption(const Ogre::UTFString & _caption)
	{
		mCaption = _caption;
		mTextOutDate = true;

		// ���� ������ �� ������, ������ ������, � ������ ��������� * 2 � �������
		size_t need = (mCaption.size() * 2 + 2) * VERTEX_IN_QUAD;
		if (mCountVertex < need) {
			mCountVertex = need + SIMPLETEXT_COUNT_VERTEX;
			if (null != mRenderItem) mRenderItem->reallockDrawItem(this, mCountVertex);
		}
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::UTFString & EditText::getCaption()
	{
		return mCaption;
	}

	void EditText::setColour(const Ogre::ColourValue & _colour)
	{
		if (mColour == _colour) return;
		mColour = _colour;
		Ogre::Root::getSingleton().convertColourValue(_colour, &mCurrentColour);

		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::ColourValue & EditText::getColour()
	{
		return mColour;
	}

	void EditText::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		mCurrentAlpha = ((uint8)(mAlpha*255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	float EditText::getAlpha()
	{
		return mAlpha;
	}

	void EditText::setFontName(const std::string & _font)
	{
		mpFont = FontManager::getInstance().getByName( _font );
		if (mpFont.isNull()) MYGUI_EXCEPT("Could not find font '" << _font << "'");
		mpFont->load();
		mpTexture = mpFont->getTextureFont();

		// ������� ������ � ���������
		mSpaceGlyphInfo = mpFont->getSpaceGlyphInfo();
		mTabGlyphInfo = mpFont->getTabGlyphInfo();

		// ������� ������� ����� �� �������� ��� ��������� ������
		Font::GlyphInfo * info = mSpaceGlyphInfo;
		mBackgroundEmpty.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectGlyphInfo();
		mBackgroundFill.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectDeactiveGlyphInfo();
		mBackgroundFillDeactive.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		info = mpFont->getCursorGlyphInfo();
		mCursorTexture.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		// ���� ����, ������������� ��������� ������ ������
		if (mpFont->getDefaultHeight() != 0) {
			mFontHeight = mpFont->getDefaultHeight();
		}

		mTextOutDate = true;

		// ���� �� ���� ������������, �� ������� ����
		if (null != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = null;
		}

		// ���� ���� ��������, �� ���������������
		if ((false == mpTexture.isNull()) && (null != mItemKeeper)) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const std::string & EditText::getFontName()
	{
		return mpFont->getName();
	}

	void EditText::setFontHeight(uint16 _height)
	{
		mFontHeight = _height;
		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	uint16 EditText::getFontHeight()
	{
		return mFontHeight;
	}

	void EditText::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mItemKeeper = _keeper;

		// ���� ��� ���� ��������, �� ��������, ��������� ��� ����� �����
		if (false == mpTexture.isNull()) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}
	}

	void EditText::_destroyDrawItem()
	{
		if (null != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = null;
		}
		mItemKeeper = null;
	}

	size_t EditText::getSelectStart()
	{
		return mStartSelect;
	}

	size_t EditText::getSelectEnd()
	{
		return mEndSelect;
	}

	void EditText::setTextSelect(size_t _start, size_t _end)
	{
		mStartSelect=_start;
		mEndSelect=_end;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	bool EditText::getSelectBackground()
	{
		return mBackgroundNormal;
	}

	void EditText::setSelectBackground(bool _normal)
	{
		if (mBackgroundNormal == _normal) return;
		mBackgroundNormal = _normal;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	bool EditText::isCursorShow()
	{
		return mShowCursor;
	}

	void EditText::setShowCursor(bool _show)
	{
		if (mShowCursor == _show) return;
		mShowCursor = _show;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	size_t EditText::getCursorPosition()
	{
		return mCursorPosition;
	}

	void EditText::setCursorPosition(size_t _pos)
	{
		if (mCursorPosition == _pos) return;
		mCursorPosition = _pos;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setTextAlign(Align _align)
	{
		mTextAlign = _align;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	Align EditText::getTextAlign()
	{
		return mTextAlign;
	}

	IntSize EditText::getTextSize()
	{
		// ���� ���� ��������, ��� ���������� ��������� ������
		if (mTextOutDate) updateRawData();
		return IntSize(mContextSize.width, mContextSize.height);
	}

	void EditText::setViewOffset(IntPoint _point)
	{
		mViewOffset = _point;
		mManualView = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}
		
	IntPoint EditText::getViewOffset()
	{
		return mViewOffset;
	}

	// ���������� ��������� ������� �� ������������� ���������
	size_t EditText::getCursorPosition(const IntPoint & _point)
	{
		if ((mpFont.isNull() || null == mRenderItem)) return 0;
		if (mTextOutDate) updateRawData();

		// ������� ������������� �������
		size_t position = 0;

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// ���������� ����������
		float x = mManager->getPixScaleX() * (float)_point.left * 2.0;
		float y = ( 1.0f - (mManager->getPixScaleY() * (float)_point.top * 2.0) );

		// ������� �������� ������
		float left, right, top, bottom = real_top, left_shift = 0;

		// ����� ������
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// ��� �������� ����� ������ �����
				float tmp = ((mCoord.width - mContextSize.width) >> 1) << 1;
				left_shift = -(mManager->getPixScaleX() * (float)(tmp));
			}
		}
		else {
			left_shift = (mManager->getPixScaleX() * (float)mViewOffset.left * 2.0);
		}

		if (false == mManualView) {
			if ( mTextAlign.isTop() ) 	{
				bottom += margin_top;
			}
			else if ( mTextAlign.isBottom() ) {
				bottom += mContextRealSize.height - real_height - margin_bottom;
			}
			else if ( mTextAlign.isVCenter() ) {
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // ��� �������� ����� ������ �����
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// ������������ ���������� �� ������ ������
		if (y < (bottom - mContextRealSize.height)) y = bottom - mContextRealSize.height;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= real_fontHeight;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// ������ ������ ������ ������
			//float len = index->getValueFloat();
			//++index;
			// ������ ������ ������ ������ � ��������
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// ������ ����������� ��������
			//size_t count = index->getValueSizeT();
			//++index;

			// ��������� �������
			if (y < bottom) {
				position += info.count;
				continue;
			}

			// �������� ������� ������
			if ( mTextAlign.isLeft() ) {
				// ������������ �� ����� �������
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				right = real_left - left_shift + (mContextRealSize.width - info.length) + margin_right;
			}
			else {
				// ������������ �� ������
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // ��� �������� ����� ������ �����
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			if (x <= (1.0 + right)) {
				// � ������ ������
				return position;

			} else if (x >= (1.0 + right + info.real_length)) {
				// � ����� ������
				position += info.count - 1;
				return position;
			}

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) continue;

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// ������ � ������, ���������� � ��������� ������
				if (x < (1.0 + right)) {
					if (!(x < ((1.0 + right) - (horiz_height * 0.5)) )) {
						// � ������ �������� �������
						position ++;
					}
					return position;
				}

				// ��������� ������
				position ++;

			}
			return position;
		}

		return position;
	}

	// ���������� ��������� ������� � ���������� �����������
	IntCoord EditText::getCursorCoord(size_t _position)
	{
		if (mpFont.isNull() || (null == mRenderItem)) return IntCoord();

		if (mTextOutDate) updateRawData();

		// ������� ������������� �������
		size_t position = 0;

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// ������� �������� ������
		float left, right, top, bottom = real_top, left_shift = 0;

		// ����� ������
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// ��� �������� ����� ������ �����
				float tmp = ((mCoord.width - mContextSize.width) >> 1) << 1;
				left_shift = -(mManager->getPixScaleX() * (float)(tmp));
			}
		}
		else {
			left_shift = (mManager->getPixScaleX() * (float)mViewOffset.left * 2.0);
		}

		if (false == mManualView) {
			if ( mTextAlign.isTop() ) {
				bottom += margin_top;
			}
			else if ( mTextAlign.isBottom() ) {
				bottom += mContextRealSize.height - real_height - margin_bottom;
			}
			else if ( mTextAlign.isVCenter() ) {
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // ��� �������� ����� ������ �����
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= real_fontHeight;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// ������ ������ ������ ������
			//float len = index->getValueFloat();
			//++index;
			// ������ ������ ������ ������ � ��������
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// ������ ����������� ��������
			//size_t count = index->getValueSizeT();
			//++index;

			// �������� ������� ������
			if ( mTextAlign.isLeft() ) {
				// ������������ �� ����� �������
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				right = real_left - left_shift + (mContextRealSize.width - info.real_length) + margin_right;
			}
			else {
				// ������������ �� ������
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // ��� �������� ����� ������ �����
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			// ������� ��������� � ������
			size_t cur = position;

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) continue;

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// ��������� �������
				if (cur == mCursorPosition) {
					return IntCoord((int)((1.0f + left) / (mManager->getPixScaleX() * 2.0)), (int)((1.0f - top) / (mManager->getPixScaleY() * 2.0)), 2, mFontHeight);

				}

				// ��������� ������ � ������
				cur ++;
			}

			// ��������� �������
			if (cur == mCursorPosition) {
				return IntCoord((int)((1.0f + right) / (mManager->getPixScaleX() * 2.0)), (int)((1.0f - top) / (mManager->getPixScaleY() * 2.0)), 2, mFontHeight);

			}

			// ��������� ������
			position += info.count;

		}
		
		return IntCoord();
	}

	size_t EditText::_drawItem(Vertex * _vertex, bool _update)
	{

		if (_update) mTextOutDate = true;

		if (mpFont.isNull()) return 0;
		if ((false == mShow) || (mEmptyView)) return 0;

		if (mTextOutDate) updateRawData();

		// ����������� ������������ ������
		size_t vertex_count = 0;

		// ������� ������������� �������
		size_t position = 0;

		// ������� �����
		uint32 colour_current = mCurrentColour;
		uint32 colour = mCurrentColour;
		uint32 colour_inverse = mInverseColour;

		FloatPoint background(mBackgroundFill);
		if (false == mBackgroundNormal) background = mBackgroundFillDeactive;

		float vertex_z = mManager->getMaximumDepth();

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop() + (mShiftText ? 1 : 0)) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// ������� �������� ������
		float left, right, top, bottom = real_top, left_shift = 0;

		// ����� ������
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// ��� �������� ����� ������ �����
				float tmp = ((mCoord.width - mContextSize.width) >> 1) << 1;
				left_shift = -(mManager->getPixScaleX() * (float)(tmp));
			}
		}
		else {
			left_shift = (mManager->getPixScaleX() * (float)mViewOffset.left * 2.0);
		}

		if (false == mManualView) {
			if ( mTextAlign.isTop() ) 	{
				bottom += margin_top;
			}
			else if ( mTextAlign.isBottom() ) {
				bottom += mContextRealSize.height - real_height - margin_bottom;
			}
			else if ( mTextAlign.isVCenter() ) {
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // ��� �������� ����� ������ �����
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// ������ ��������������� ��� ������
		float vertex_top, vertex_bottom, vertex_left, vertex_right;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= real_fontHeight;

			// ����������� � ���������
			vertex_top = top;
			vertex_bottom = bottom;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// ������ ������ ������ ������
			//float len = index->getValueFloat();
			//++index;
			// ������ ������ ������ � ��������
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// ������ ����������� ��������
			//size_t count = index->getValueSizeT();
			//++index;

			// ���� �� ������������� ���������� ����������
			bool texture_crop_height = false;

			if (vertex_top > real_top) {
				// �������� �� ������ �����
				if (vertex_bottom > real_top) {

					// ���������� ������� ���� ������ �������
					for (;index != end_index; ++index) {
						// ��������� �� ����� �����
						if ( index->isColour() ) {
							colour = index->getColour() | (colour & 0xFF000000);
							colour_inverse = colour ^ 0x00FFFFFF;
						}
					}

					position += info.count;
					continue;
				}
				// ��������
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom) {
				// ������ ���� ����
				if (vertex_top < real_bottom) {
					line = end;
					line --;
					position += info.count;
					continue;
				}
				// ��������
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			// �������� ������� ������
			if ( mTextAlign.isLeft() ) {
				// ������������ �� ����� �������
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// ������������ �� ������ �������
				right = real_left - left_shift + (mContextRealSize.width - info.real_length) + margin_right;
			}
			else {
				// ������������ �� ������
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // ��� �������� ����� ������ �����
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			// ������� ��������� � ������
			size_t cur = position;

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					colour_inverse = colour ^ 0x00FFFFFF;
					continue;
				}

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// �������� �������� ��� ����
				FloatPoint background_current;
				bool select = !( (cur >= mEndSelect) || (cur < mStartSelect) );

				// ������ �� �������
				if (false == select) {
					colour_current = colour;
					background_current = mBackgroundEmpty;

				}
				// ������ �������
				else {
					// ��������� �����
					colour_current = colour_inverse;
					background_current = background;

				}

				// ����������� � ���������
				vertex_left = left;
				vertex_right = right;

				// ������� ���������� ����������
				float texture_left = info->uvRect.left;
				float texture_right = info->uvRect.right;
				float texture_top = info->uvRect.top;
				float texture_bottom = info->uvRect.bottom;

				// ���� �� ������������� ���������� ����������
				bool texture_crop_width = false;

				if (vertex_left < real_left) {
					// �������� �� ������ �����
					if (vertex_right < real_left) {
						cur ++;
						continue;
					}
					// ��������
					vertex_left = real_left;
					texture_crop_width = true;
				}
				if (vertex_right > real_right) {
					// ������ ������ �� ����� �� ����
					if (vertex_left > real_right) {
						index ++;
						// ��� ���� ����� ���� ����� �� ��������, ����� ������ �� ����� ������
						while (index != end_index) {
							// ��������� �� ����� �����
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
								colour_inverse = colour ^ 0x00FFFFFF;
							}
							index ++;
						};
						index --; // ����� ��� ���������� ��������� �� end
						continue;
					}
					// ��������
					vertex_right = real_right;
					texture_crop_width = true;
				}

				// �������� �������� �� ���������
				if (texture_crop_height) {
					// ���������� ������ �������� � ���������� �����������
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// �������� ������ �������� � ���������� �����������
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// �������� �������� �� �����������
				if (texture_crop_width) {
					// ���������� ������ �������� � ���������� �����������
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// �������� ������ �������� � ���������� �����������
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				// ���� ����� ������ ���������
				if (select) {
					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						background_current.left, background_current.top, background_current.left, background_current.top, vertex_count);
				}

				__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
					texture_left, texture_top, texture_right, texture_bottom, vertex_count);

				// ��������� �������
				if ((mShowCursor) && (cur == mCursorPosition)) {
					if ((vertex_left == left) && (vertex_right > (left + (mManager->getPixScaleX() * 4)))) {
						vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

						// ������ ��������� �����
						colour_current |= 0x00FFFFFF;

						__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
							mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

						vertex_left = vertex_right;
						vertex_right += (mManager->getPixScaleX() * 2);

						// ������ ��������� ������
						colour_current = colour_current & 0xFF000000;

						__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
							mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

					}
				}

				// ��������� ������ � ������
				cur ++;
			}

			// ��������� �������
			if ((mShowCursor) && (cur == mCursorPosition)) {
				if ((right >= real_left) && ((right + (mManager->getPixScaleX() * 4)) <= real_right)) {
					vertex_left = right;
					vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

					// ������ ��������� �����
					colour_current |= 0x00FFFFFF;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

					vertex_left = vertex_right;
					vertex_right += (mManager->getPixScaleX() * 2);

					// ������ ��������� ������
					colour_current = colour_current & 0xFF000000;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);
				}
			}

			// ��������� ������
			position += info.count;

		}
		
		// ����������� ������� ������������ ������
		return vertex_count;
	}

	void EditText::setShiftText(bool _shift)
	{
		if (mShiftText == _shift) return;
		mShiftText = _shift;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setBreakLine(bool _break)
	{
		mBreakLine = _break;
		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::updateRawData()
	{
		//??? ����� ����������� ������� � �������� ��� �������

		if (mpFont.isNull()) return;
		// ��������� �����
		mTextOutDate = false;

		// ������ ��� ������� ����������� ������
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// ���������� ������� ����� ������� � ���������� �����������
		// ??? ��� ���� ������������� ������ ��� ��������� ��������� ������ ��� ����� ������
		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);//???
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mManager->getAspectCoef() * real_fontHeight);

		mLinesInfo.clear();

		// ������� ������ �������
		mLinesInfo.push_back(PairVectorCharInfo());
		float len = 0, width = 0;
		size_t count = 1;

		RollBackSave roll_back;

		Ogre::UTFString::const_iterator end = mCaption.end();

		for (Ogre::UTFString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Font::CodePoint character = *index;

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {

				// ������ ������, ������ �������, ���� ������
				len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

				// ���������� ������ ���������� ������
				mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// � ������� �����
				mLinesInfo.push_back(PairVectorCharInfo());

				if (character == Font::FONT_CODE_CR) {
					Ogre::UTFString::const_iterator peeki = index;
					peeki++;
					if ((peeki != end) && (*peeki == Font::FONT_CODE_LF)) index = peeki; // skip both as one newline
				}

				// �������� �����
				roll_back.reset();

				// ��������� ������
				continue;

			}
			else if (character == L'#') {
				// ����� ��������� ������
				++ index;
				if (index == end) {--index ;continue;} // ��� ������

				character = *index;
				// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
				if (character != L'#') {

					// ������ ������ ������
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// � ��� ���� �������� ����� �����
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // ��� ������
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F];
					}

					// ���� �����, �� ������ ������� � ����� ����������
					if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);

					// ���������� ����, � ������� ����� �������
					mLinesInfo.back().second.push_back( EnumCharInfo(colour, true) );

					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) {
				VectorCharInfo::iterator iter = mLinesInfo.back().second.end();
				if (mBreakLine) roll_back.set(iter, index, count, len);
				info = mpFont->getSpaceGlyphInfo();
			}
			else if (Font::FONT_CODE_TAB == character) {
				VectorCharInfo::iterator iter = mLinesInfo.back().second.end();
				if (mBreakLine) roll_back.set(iter, index, count, len);
				info = mpFont->getTabGlyphInfo();
			}
			else {
				info = mpFont->getGlyphInfo(character);
			}

			float len_char = info->aspectRatio * (float)mFontHeight;

			// ������� ������
			if (mBreakLine
				&& (len + len_char + EDIT_TEXT_WIDTH_CURSOR + 1) > mCoord.width
				&& roll_back.rollback
				&& (mCoord.width > EDIT_MIN_BREAK_WORD_WIDTH)) {

				// ���������� ����� �� �������
				len = roll_back.real_lenght;
				count = roll_back.count;
				index = roll_back.space_point;

				mLinesInfo.back().second.erase(mLinesInfo.back().second.begin() + (count-1), mLinesInfo.back().second.end());

				// ������ ������, ������ �������, ���� ������
				len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

				// ���������� ������ ���������� ������
				mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// � ������� �����
				mLinesInfo.push_back(PairVectorCharInfo());

				// �������� �����
				roll_back.reset();

				// ��������� ������
				continue;
			}

			len += len_char;

			// ��������� �� ���� � �������
			mLinesInfo.back().second.push_back( EnumCharInfo(info) );
			count ++;

		}

		// ������ ������, ������ �������, ���� ������
		len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

		// ���������� ������ ���������� ������
		mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

		if (width < len) width = len;

		// ������������� ������ ������
		mContextSize.set(width, (float)mLinesInfo.size() * mFontHeight);
		mContextRealSize.set(mContextSize.width * mManager->getPixScaleX() * 2.0f, mContextSize.height  * mManager->getPixScaleY() * 2.0f);
	}

} // namespace MyGUI
