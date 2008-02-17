/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_FontManager.h"

namespace MyGUI
{

	const size_t SIMPLETEXT_COUNT_VERTEX = 16 * VERTEX_IN_QUAD;

	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubWidgetTextInterface(_info.coord, _info.align, _parent),
		mTransparent(false),
		mCurrentCoord(_info.coord),
		mTextOutDate(false),
		mCurrentColour(0x00FFFFFF),
		mCurrentAlpha(0xFF000000),
		mColour(Ogre::ColourValue::White),
		mAlpha(ALPHA_MAX),
		mFontHeight(16),
		mAspectCoef(0),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mItemKeeper(null),
		mRenderItem(null)
	{
		// ����� ���������
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void SimpleText::_setAlign(const IntSize& _size, bool _update)
	{

		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// �����������
				mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_update = true;
				mIsMargin = true; // ��� ��������� �������� ��� �������������
			}
			else {
				// ������� �� ������� ����
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		}
		else if (false == IS_ALIGN_LEFT(mAlign)) {
			// ������������ �� ����������� ��� ����������
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// �����������
				mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_update = true;
				mIsMargin = true; // ��� ��������� �������� ��� �������������
			}
			else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		}
		else if (false == IS_ALIGN_TOP(mAlign)) {
			// ������������ �� ��������� ��� ����������
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void SimpleText::_updateView()
	{
		bool margin = _checkMargin();

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				if (false == mTransparent) mTransparent = true;
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
		if (mTransparent) mTransparent = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setTextAlign(Align _align)
	{
		mTextAlign = _align;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setCaption(const Ogre::DisplayString & _caption)
	{
		mCaption = _caption;
		mTextOutDate = true;

		if (null != mRenderItem) {
			// ���� ������ �� ������, ������ ������
			if (mCountVertex < mCaption.size()) {
				mCountVertex = (mCaption.size() * VERTEX_IN_QUAD) + SIMPLETEXT_COUNT_VERTEX;
				mRenderItem->reallockDrawItem(this, mCountVertex);
			}
			mRenderItem->outOfDate();
		}
	}

	const Ogre::DisplayString & SimpleText::getCaption()
	{
		return mCaption;
	}

	void SimpleText::setColour(const Ogre::ColourValue & _colour)
	{
		mColour = _colour;
		uint32 colour;
		Ogre::Root::getSingleton().convertColourValue(_colour, &colour);
		mCurrentColour = (colour & 0x00FFFFFF) | mCurrentAlpha;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::ColourValue & SimpleText::getColour()
	{
		return mColour;
	}

	void SimpleText::setAlpha(float _alpha)
	{
		mAlpha = _alpha;
		mCurrentAlpha = ((uint8)(mAlpha*255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	float SimpleText::getAlpha()
	{
		return mAlpha;
	}

	void SimpleText::setFontName(const std::string & _font)
	{
		mpFont = FontManager::getInstance().getByName( _font );
		if (mpFont.isNull()) MYGUI_EXCEPT("Could not find font " << _font);
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

	const std::string & SimpleText::getFontName()
	{
		return mpFont->getName();
	}

	void SimpleText::setFontHeight(uint16 _height)
	{
		mFontHeight = _height;
		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	uint16 SimpleText::getFontHeight()
	{
		return mFontHeight;
	}

	IntSize SimpleText::getTextSize()
	{
		return IntSize(); //???
	}

	IntSize SimpleText::getTextSize(const Ogre::DisplayString& _text)
	{
		return IntSize(); //???
	}

	void SimpleText::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mItemKeeper = _keeper;

		// ���� ��� ���� ��������, �� ��������, ��������� ��� ����� �����
		if (false == mpTexture.isNull()) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}
	}

	void SimpleText::_destroyDrawItem()
	{
		if (null != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = null;
		}
		mItemKeeper = null;
	}

	void SimpleText::updateRawData()
	{
		if (mpFont.isNull()) return;
		// ��������� �����
		mAspectCoef = mRenderItem->getAspectCoef();
		mTextOutDate = false;

		// ������ ��� ������� ����������� ������
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// ���������� ������� ����� ������� � ���������� �����������
		float real_fontHeight = (mRenderItem->getPixScaleY() * (float)mFontHeight * 2.0f);
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mAspectCoef * real_fontHeight);

		mLinesInfo.clear();

		// ������� ������ �������
		mLinesInfo.push_back(VectorCharInfo());
		mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � �������� �����������
		mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������
		float len = 0, width = 0;
		size_t count = 1;

		Ogre::DisplayString::const_iterator end = mCaption.end();
		for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Font::CodePoint character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
				// ���������� ������ ���������� ������
				mLinesInfo.back()[0] = EnumCharInfo(len);
				mLinesInfo.back()[1] = EnumCharInfo(count);
				if (width < len) width = len;
				count = 1;
				len = 0;

				// � ������� �����
				mLinesInfo.push_back(VectorCharInfo());
				mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � ��������
				mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������

				if (character == Font::FONT_CODE_CR) {
					Ogre::DisplayString::const_iterator peeki = index;
					peeki++;
					if (peeki != end && MYGUI_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
				}
				// ��������� ������
				continue;

			} else if (character == _T('#')) {
				// ����� ��������� ������
				++ index;
				if (index == end) {--index ;continue;} // ��� ������

				character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);
				// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
				if (character != _T('#')) {

					// ������ ������ ������
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// � ��� ���� �������� ����� �����
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // ��� ������
						colour <<= 4;
						colour += convert_colour[ (MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index)-48) & 0x3F];
					}

					// ���� �����, �� ������ ������� � ����� ����������
					if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);

					// ���������� ����, � ������� ����� �������
					mLinesInfo.back().push_back( EnumCharInfo(colour, true) );

					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
			else info = mpFont->getGlyphInfo(character);

			len += info->aspectRatio * real_fontHeight * mAspectCoef;

			// ��������� �� ���� � �������
			mLinesInfo.back().push_back( EnumCharInfo(info) );
			count ++;

		}

		// ���������� ������ ���������� ������
		mLinesInfo.back()[0] = EnumCharInfo(len);
		mLinesInfo.back()[1] = EnumCharInfo(count);
		if (width < len) width = len;


		// ������������� ������ ������
		mContextRealSize.set(width, (float)mLinesInfo.size() * real_fontHeight);

	}



	size_t SimpleText::_drawItem(Vertex * _vertex)
	{
		if (mpFont.isNull()) return 0;
		if ((false == mShow) || (mTransparent)) return 0;
		if ((0 >= mCurrentCoord.width) || (0 >= mCurrentCoord.height)) return 0;

		if ((mAspectCoef != mRenderItem->getAspectCoef()) || mTextOutDate) updateRawData();

		size_t vertex_count = 0;

		// ������� �����
		Ogre::RGBA colour = mCurrentColour;

		float vertex_z = mRenderItem->getMaximumDepth();

		float real_fontHeight = (mRenderItem->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mRenderItem->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mRenderItem->getHOffset()) * 2) - 1;
		float real_top = -(((mRenderItem->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mRenderItem->getVOffset()) * 2) - 1);
		float real_width = (mRenderItem->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mRenderItem->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mRenderItem->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mRenderItem->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mRenderItem->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mRenderItem->getPixScaleY() * 2);

		// ������� �������� ������
		float left, right, top, bottom = real_top, left_shift = 0;

		mTextAlign = ALIGN_HCENTER | ALIGN_VCENTER;

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		if ( IS_ALIGN_RIGHT(mTextAlign) ) left_shift = mContextRealSize.width - real_width; // ������������ �� ������ �������
		else if ( IS_ALIGN_HCENTER(mTextAlign) ) left_shift = (mContextRealSize.width - real_width) * 0.5; // ������������ �� ������

		if ( IS_ALIGN_TOP(mTextAlign) ) 	bottom += margin_top;
		else if ( IS_ALIGN_BOTTOM(mTextAlign) ) bottom += mContextRealSize.height - real_height - margin_bottom;
		else if ( IS_ALIGN_VCENTER(mTextAlign) ) bottom += (margin_top - margin_bottom + mContextRealSize.height - real_height) * 0.5;

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

			VectorCharInfo::iterator index = line->begin();
			VectorCharInfo::iterator end_index = line->end();
			// ������ ������ ������ ������
			float len = index->getValueFloat();
			++index;
			// ������ ����������� ��������
			++index;

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
						}
					}

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
					continue;
				}
				// ��������
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			// �������� ������� ������
			if ( IS_ALIGN_LEFT(mTextAlign) ) right = real_left - left_shift - margin_left; // ������������ �� ����� �������
			else if ( IS_ALIGN_RIGHT(mTextAlign) ) right = real_left - left_shift + (mContextRealSize.width - len) + margin_right; // ������������ �� ������ �������
			else right = real_left - left_shift + (((mContextRealSize.width - len) - margin_left + margin_right) * 0.5); // ������������ �� ������

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					continue;
				}

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mAspectCoef;

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// ���� ������ ��� ��������� �� �� ������
				if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) continue;

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
					if (vertex_right < real_left) continue;
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

				// first triangle - left top
				_vertex[0].x = vertex_left;
				_vertex[0].y = vertex_top;
				_vertex[0].z = vertex_z;
				_vertex[0].colour = colour;
				_vertex[0].u = texture_left;
				_vertex[0].v = texture_top;

				// first triangle - left bottom
				_vertex[1].x = vertex_left;
				_vertex[1].y = vertex_bottom;
				_vertex[1].z = vertex_z;
				_vertex[1].colour = colour;
				_vertex[1].u = texture_left;
				_vertex[1].v = texture_bottom;

				// first triangle - right top
				_vertex[2].x = vertex_right;
				_vertex[2].y = vertex_top;
				_vertex[2].z = vertex_z;
				_vertex[2].colour = colour;
				_vertex[2].u = texture_right;
				_vertex[2].v = texture_top;

				// second triangle - right top
				_vertex[3].x = vertex_right;
				_vertex[3].y = vertex_top;
				_vertex[3].z = vertex_z;
				_vertex[3].colour = colour;
				_vertex[3].u = texture_right;
				_vertex[3].v = texture_top;

				// second triangle = left bottom
				_vertex[4].x = vertex_left;
				_vertex[4].y = vertex_bottom;
				_vertex[4].z = vertex_z;
				_vertex[4].colour = colour;
				_vertex[4].u = texture_left;
				_vertex[4].v = texture_bottom;

				// second triangle - right botton
				_vertex[5].x = vertex_right;
				_vertex[5].y = vertex_bottom;
				_vertex[5].z = vertex_z;
				_vertex[5].colour = colour;
				_vertex[5].u = texture_right;
				_vertex[5].v = texture_bottom;

				// ������ ��������� � ����������� ������������ ������
				_vertex += VERTEX_IN_QUAD;
				vertex_count += VERTEX_IN_QUAD;

			}
		}

		return vertex_count;
	}

} // namespace MyGUI
