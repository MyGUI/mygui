/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_EditText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	struct EditTextStateData : public StateInfo
	{
		Colour colour;
		bool shift;
	};

	const size_t SIMPLETEXT_COUNT_VERTEX = 32 * VERTEX_IN_QUAD;
	const float EDIT_TEXT_WIDTH_CURSOR = 2.0f;
	const int EDIT_MIN_BREAK_WORD_WIDTH = 10;

	struct RollBackSave
	{
		RollBackSave() : rollback(false) {}

		void set(
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

		void reset() { rollback = false; }

		VectorCharInfo::iterator space_rollback;
		Ogre::UTFString::const_iterator space_point;
		bool rollback;
		size_t count;
		float real_lenght;
	};

	// рисковать не будем с инлайнами
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

	#define MYGUI_CONVERT_COLOUR(colour, gl) if (gl) { colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00); }

	EditText::EditText(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		ISubWidgetText(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentColour(0x00FFFFFF), mInverseColour(0x00000000),
		mCurrentAlpha(0xFF000000),
		mCurrentCoord(_info.coord),
		mTextOutDate(false),
		mTextAlign(Align::Default),
		mColour(Colour::White),
		mAlpha(ALPHA_MAX),
		mFontHeight(0),
		mBackgroundNormal(true),
		mStartSelect(0), mEndSelect(0),
		mCursorPosition(0), mShowCursor(false),
		mItemKeeper(nullptr),
		mRenderItem(nullptr),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mShiftText(false),
		mBreakLine(false),
		mOldWidth(0)
	{
		mManager = LayerManager::getInstancePtr();

		// потом перенести
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());

		mCurrentColour = mColour.toColourARGB();
		MYGUI_CONVERT_COLOUR(mCurrentColour, mRenderGL);

		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		mManualView = true;
	}

	EditText::~EditText()
	{
	}

	void EditText::setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::_correctView()
	{
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void EditText::_setAlign(const IntSize& _size, bool _update)
	{

		if (mBreakLine) {
			// передается старая координата всегда
			int width = mCroppedParent->getWidth();
			if (mOldWidth != width) {
				mOldWidth = width;
				mTextOutDate = true;
			}
		}

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isHStretch()) {
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _size.width);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight()) {
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _size.width);
			need_update = true;
		}
		else if (mAlign.isHCenter()) {
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch()) {
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _size.height);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom()) {
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _size.height);
			need_update = true;
		}
		else if (mAlign.isVCenter()) {
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
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

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				//mEmptyView = true;
				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (nullptr != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // мы обрезаны или были обрезаны
			mCurrentCoord.width = _getViewWidth();
			mCurrentCoord.height = _getViewHeight();

		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		//mEmptyView = false;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setCaption(const Ogre::UTFString & _caption)
	{
		mCaption = _caption;
		mTextOutDate = true;

		// если вершин не хватит, делаем реалок, с учетом выделения * 2 и курсора
		size_t need = (mCaption.size() * 2 + 2) * VERTEX_IN_QUAD;
		if (mCountVertex < need) {
			mCountVertex = need + SIMPLETEXT_COUNT_VERTEX;
			if (nullptr != mRenderItem) mRenderItem->reallockDrawItem(this, mCountVertex);
		}
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::UTFString & EditText::getCaption()
	{
		return mCaption;
	}

	void EditText::setTextColour(const Colour& _colour)
	{
		if (mColour == _colour) return;
		mColour = _colour;
		mCurrentColour = mColour.toColourARGB();

		MYGUI_CONVERT_COLOUR(mCurrentColour, mRenderGL);

		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	const Colour& EditText::getTextColour()
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

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
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

		// достаем пробел и табуляцию
		mSpaceGlyphInfo = mpFont->getSpaceGlyphInfo();
		mTabGlyphInfo = mpFont->getTabGlyphInfo();

		// достаем средние точки на текстуре для выделения текста
		Font::GlyphInfo * info = mSpaceGlyphInfo;
		mBackgroundEmpty.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectGlyphInfo();
		mBackgroundFill.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectDeactiveGlyphInfo();
		mBackgroundFillDeactive.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		info = mpFont->getCursorGlyphInfo();
		mCursorTexture.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		// если надо, устанавливаем дефолтный размер шрифта
		if (mpFont->getDefaultHeight() != 0) {
			mFontHeight = mpFont->getDefaultHeight();
		}

		mTextOutDate = true;

		// если мы были приаттаченны, то удаляем себя
		if (nullptr != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = nullptr;
		}

		// если есть текстура, то приаттачиваемся
		if ((false == mpTexture.isNull()) && (nullptr != mItemKeeper)) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	const std::string & EditText::getFontName()
	{
		return mpFont->getName();
	}

	void EditText::setFontHeight(uint _height)
	{
		mFontHeight = _height;
		mTextOutDate = true;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	uint EditText::getFontHeight()
	{
		return mFontHeight;
	}

	void EditText::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mItemKeeper = _keeper;

		// если уже есть текстура, то атачимся, актуально для смены леера
		if (false == mpTexture.isNull()) {
			MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}
	}

	void EditText::_destroyDrawItem()
	{
		if (nullptr != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = nullptr;
		}
		mItemKeeper = nullptr;
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
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	bool EditText::getSelectBackground()
	{
		return mBackgroundNormal;
	}

	void EditText::setSelectBackground(bool _normal)
	{
		if (mBackgroundNormal == _normal) return;
		mBackgroundNormal = _normal;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	bool EditText::isCursorShow()
	{
		return mShowCursor;
	}

	void EditText::setShowCursor(bool _show)
	{
		if (mShowCursor == _show) return;
		mShowCursor = _show;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	size_t EditText::getCursorPosition()
	{
		return mCursorPosition;
	}

	void EditText::setCursorPosition(size_t _pos)
	{
		if (mCursorPosition == _pos) return;
		mCursorPosition = _pos;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setTextAlign(Align _align)
	{
		mTextAlign = _align;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	Align EditText::getTextAlign()
	{
		return mTextAlign;
	}

	IntSize EditText::getTextSize()
	{
		// если нуно обновить, или изменились пропорции экрана
		if (mTextOutDate) updateRawData();
		return IntSize(mContextSize.width, mContextSize.height);
	}

	void EditText::setViewOffset(IntPoint _point)
	{
		mViewOffset = _point;
		mManualView = true;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	IntPoint EditText::getViewOffset()
	{
		return mViewOffset;
	}

	// возвращает положение курсора по произвольному положению
	size_t EditText::getCursorPosition(const IntPoint & _point)
	{
		if ((mpFont.isNull() || nullptr == mRenderItem)) return 0;
		if (mTextOutDate) updateRawData();

		// позиция отображаемого символа
		size_t position = 0;

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		// UNUSED!!!
		// float real_right = real_left + real_width;
		// float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// абсолютные координаты
		float x = mManager->getPixScaleX() * (float)_point.left * 2.0;
		float y = ( 1.0f - (mManager->getPixScaleY() * (float)_point.top * 2.0) );

		// опорное смещение вершин
		float left, right, top, bottom = real_top, left_shift = 0;

		// сдвиг текста
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// для середины нужно четное число
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
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // для середины нужно четное число
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// корректируем координату до нижней строки
		if (y < (bottom - mContextRealSize.height)) y = bottom - mContextRealSize.height;

		// основной цикл
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// пересчет опорных данных
			top = bottom;
			bottom -= real_fontHeight;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// первый всегда длинна строки
			//float len = index->getValueFloat();
			//++index;
			// второй всегда длинна строки в пикселях
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// третий колличество символов
			//size_t count = index->getValueSizeT();
			//++index;

			// следующая строчка
			if (y < bottom) {
				position += info.count;
				continue;
			}

			// пересчет опорных данных
			if ( mTextAlign.isLeft() ) {
				// выравнивание по левой стороне
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				right = real_left - left_shift + (mContextRealSize.width - info.length) + margin_right;
			}
			else {
				// выравнивание по центру
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // для середины нужно четное число
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			if (x <= (1.0 + right)) {
				// в начало строки
				return position;

			} else if (x >= (1.0 + right + info.real_length)) {
				// в конец строки
				position += info.count - 1;
				return position;
			}

			// внутренний цикл строки
			for (;index != end_index; ++index) {

				// проверяем на смену цвета
				if ( index->isColour() ) continue;

				// отображаемый символ
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// пересчет опорных данных
				left = right;
				right += horiz_height;

				// попали в символ, сравниваем с половиной ширины
				if (x < (1.0 + right)) {
					if (!(x < ((1.0 + right) - (horiz_height * 0.5)) )) {
						// в правой половине символа
						position ++;
					}
					return position;
				}

				// следующий символ
				position ++;

			}
			return position;
		}

		return position;
	}

	// возвращает положение курсора в обсолютных координатах
	IntCoord EditText::getCursorCoord(size_t _position)
	{
		if (mpFont.isNull() || (nullptr == mRenderItem)) return IntCoord();

		if (mTextOutDate) updateRawData();

		//FIXME добавленно
		mCursorPosition = _position;

		// позиция отображаемого символа
		size_t position = 0;

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		// UNUSED !!!
		// float real_right = real_left + real_width;
		// float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// опорное смещение вершин
		float left, right, top, bottom = real_top, left_shift = 0;

		// сдвиг текста
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// для середины нужно четное число
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
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // для середины нужно четное число
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// основной цикл
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// пересчет опорных данных
			top = bottom;
			bottom -= real_fontHeight;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// первый всегда длинна строки
			//float len = index->getValueFloat();
			//++index;
			// второй всегда длинна строки в пикселях
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// третий колличество символов
			//size_t count = index->getValueSizeT();
			//++index;

			// пересчет опорных данных
			if ( mTextAlign.isLeft() ) {
				// выравнивание по левой стороне
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				right = real_left - left_shift + (mContextRealSize.width - info.real_length) + margin_right;
			}
			else {
				// выравнивание по центру
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // для середины нужно четное число
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			// текущее положение в строке
			size_t cur = position;

			// внутренний цикл строки
			for (;index != end_index; ++index) {

				// проверяем на смену цвета
				if ( index->isColour() ) continue;

				// отображаемый символ
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// пересчет опорных данных
				left = right;
				right += horiz_height;

				// отрисовка курсора
				if (cur == mCursorPosition) {
					return IntCoord((int)((1.0f + left) / (mManager->getPixScaleX() * 2.0)), (int)((1.0f - top) / (mManager->getPixScaleY() * 2.0)), 2, mFontHeight);

				}

				// следующий символ в строке
				cur ++;
			}

			// отрисовка курсора
			if (cur == mCursorPosition) {
				return IntCoord((int)((1.0f + right) / (mManager->getPixScaleX() * 2.0)), (int)((1.0f - top) / (mManager->getPixScaleY() * 2.0)), 2, mFontHeight);

			}

			// следующая строка
			position += info.count;

		}

		return IntCoord();
	}

	void EditText::setShiftText(bool _shift)
	{
		if (mShiftText == _shift) return;
		mShiftText = _shift;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::setBreakLine(bool _break)
	{
		mBreakLine = _break;
		mTextOutDate = true;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void EditText::_setStateData(StateInfo * _data)
	{
		EditTextStateData * data = (EditTextStateData*)_data;
		if (data->colour != Colour::Zero) setTextColour(data->colour);
		setShiftText(data->shift);
	}

	StateInfo * EditText::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		EditTextStateData * data = new EditTextStateData();
		data->shift = utility::parseBool(_node->findAttribute("shift"));

		std::string colour = _node->findAttribute("colour");

		if (_version >= Version(1, 1)) {
			colour = LanguageManager::getInstance().replaceTags(colour);
		}

		data->colour = Colour::parse(colour);
		return data;
	}

	size_t EditText::_drawItem(Vertex * _vertex, bool _update)
	{

		if (_update) mTextOutDate = true;

		if (mpFont.isNull()) return 0;
		if ((false == mVisible) || (mEmptyView)) return 0;

		if (mTextOutDate) updateRawData();

		// колличество отрисованных вершин
		size_t vertex_count = 0;

		// позиция отображаемого символа
		size_t position = 0;

		// текущие цвета
		uint32 colour_current = mCurrentColour;
		uint32 colour = mCurrentColour;
		uint32 colour_inverse = mInverseColour;

		FloatPoint background(mBackgroundFill);
		if (false == mBackgroundNormal) background = mBackgroundFillDeactive;

		float vertex_z = mManager->getMaximumDepth();

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() + (mShiftText ? 1 : 0)) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// опорное смещение вершин
		float left, right, top, bottom = real_top, left_shift = 0;

		// сдвиг текста
		if (false == mManualView) {
			if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				left_shift = mContextRealSize.width - real_width;
			}
			else if ( mTextAlign.isHCenter() ) {
				// для середины нужно четное число
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
				float tmp = ((mCoord.height - mContextSize.height) >> 1) << 1; // для середины нужно четное число
				bottom += margin_top - (mManager->getPixScaleY() * (float)(tmp));
			}
		}
		else {
			bottom = real_top + margin_top + (mManager->getPixScaleY() * (float)mViewOffset.top * 2.0);
		}

		// данные непосредственно для вывода
		float vertex_top, vertex_bottom, vertex_left, vertex_right;

		// основной цикл
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// пересчет опорных данных
			top = bottom;
			bottom -= real_fontHeight;

			// присваиваем и вершинным
			vertex_top = top;
			vertex_bottom = bottom;

			VectorCharInfo::iterator index = line->second.begin();
			VectorCharInfo::iterator end_index = line->second.end();

			const LineInfo & info = line->first;
			// первый всегда длинна строки
			//float len = index->getValueFloat();
			//++index;
			// второй всегда длинна в пикселях
			//size_t len_pix = index->getValueSizeT();
			//++index;
			// третий колличество символов
			//size_t count = index->getValueSizeT();
			//++index;

			// нуна ли пересчитывать текстурные координаты
			bool texture_crop_height = false;

			if (vertex_top > real_top) {
				// проверка на полный выход
				if (vertex_bottom > real_top) {

					// необходимо парсить теги цветов полюбак
					for (;index != end_index; ++index) {
						// проверяем на смену цвета
						if ( index->isColour() ) {
							colour = index->getColour() | (colour & 0xFF000000);
							colour_inverse = colour ^ 0x00FFFFFF;
						}
					}

					position += info.count;
					continue;
				}
				// обрезаем
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom) {
				// вообще вниз ушли
				if (vertex_top < real_bottom) {
					line = end;
					line --;
					position += info.count;
					continue;
				}
				// обрезаем
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			// пересчет опорных данных
			if ( mTextAlign.isLeft() ) {
				// выравнивание по левой стороне
				right = real_left - left_shift - margin_left;
			}
			else if ( mTextAlign.isRight() ) {
				// выравнивание по правой стороне
				right = real_left - left_shift + (mContextRealSize.width - info.real_length) + margin_right;
			}
			else {
				// выравнивание по центру
				int tmp = ((mContextSize.width - info.length) >> 1) << 1; // для середины нужно четное число
				right = real_left - left_shift + (((mManager->getPixScaleX() * (float)tmp * 2.0)) * 0.5) - margin_left;
			}

			// текущее положение в строке
			size_t cur = position;

			// внутренний цикл строки
			for (;index != end_index; ++index) {

				// проверяем на смену цвета
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					colour_inverse = colour ^ 0x00FFFFFF;
					continue;
				}

				// отображаемый символ
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// пересчет опорных данных
				left = right;
				right += horiz_height;

				// смещение текстуры для фона
				FloatPoint background_current;
				bool select = !( (cur >= mEndSelect) || (cur < mStartSelect) );

				// символ не выделен
				if (false == select) {
					colour_current = colour;
					background_current = mBackgroundEmpty;

				}
				// символ выделен
				else {
					// инверсные цвета
					colour_current = colour_inverse;
					background_current = background;

				}

				// присваиваем и вершинным
				vertex_left = left;
				vertex_right = right;

				// текущие текстурные координаты
				float texture_left = info->uvRect.left;
				float texture_right = info->uvRect.right;
				float texture_top = info->uvRect.top;
				float texture_bottom = info->uvRect.bottom;

				// нуна ли пересчитывать текстурные координаты
				bool texture_crop_width = false;

				if (vertex_left < real_left) {
					// проверка на полный выход
					if (vertex_right < real_left) {
						cur ++;
						continue;
					}
					// обрезаем
					vertex_left = real_left;
					texture_crop_width = true;
				}
				if (vertex_right > real_right) {
					// вообще строку до конца не нуна
					if (vertex_left > real_right) {
						index ++;
						// для того чтобы теги цвета не терялись, нужно пройти до конца строки
						while (index != end_index) {
							// проверяем на смену цвета
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
								colour_inverse = colour ^ 0x00FFFFFF;
							}
							index ++;
						};
						index --; // чтобы при увеличении показывал на end
						continue;
					}
					// обрезаем
					vertex_right = real_right;
					texture_crop_width = true;
				}

				// смещение текстуры по вертикили
				if (texture_crop_height) {
					// прибавляем размер смещения в текстурных координатах
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// отнимаем размер смещения в текстурных координатах
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// смещение текстуры по горизонтали
				if (texture_crop_width) {
					// прибавляем размер смещения в текстурных координатах
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// отнимаем размер смещения в текстурных координатах
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				// если нужно рисуем выделение
				if (select) {
					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
						background_current.left, background_current.top, background_current.left, background_current.top, vertex_count);
				}

				__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
					texture_left, texture_top, texture_right, texture_bottom, vertex_count);

				// отрисовка курсора
				if ((mShowCursor) && (cur == mCursorPosition)) {
					if ((vertex_left == left) && (vertex_right > (left + (mManager->getPixScaleX() * 4)))) {
						vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

						// первая половинка белая
						colour_current |= 0x00FFFFFF;

						__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
							mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

						vertex_left = vertex_right;
						vertex_right += (mManager->getPixScaleX() * 2);

						// вторая половинка черная
						colour_current = colour_current & 0xFF000000;

						__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
							mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

					}
				}

				// следующий символ в строке
				cur ++;
			}

			// отрисовка курсора
			if ((mShowCursor) && (cur == mCursorPosition)) {
				if ((right >= real_left) && ((right + (mManager->getPixScaleX() * 4)) <= real_right)) {
					vertex_left = right;
					vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

					// первая половинка белая
					colour_current |= 0x00FFFFFF;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

					vertex_left = vertex_right;
					vertex_right += (mManager->getPixScaleX() * 2);

					// вторая половинка черная
					colour_current = colour_current & 0xFF000000;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current,
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);
				}
			}

			// следующая строка
			position += info.count;

		}

		// колличество реально отрисованных вершин
		return vertex_count;
	}

	void EditText::updateRawData()
	{
		//??? потом обязательно сделать с резервом для вектора

		if (mpFont.isNull()) return;
		// сбрасывам флаги
		mTextOutDate = false;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// вычисление размера одной единицы в текстурных координатах
		// ??? это нуно пересчитывать только при изменении пропорций экрана или смене шрифта
		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);//???
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mManager->getAspectCoef() * real_fontHeight);

		mLinesInfo.clear();

		// создаем первую строчку
		mLinesInfo.push_back(PairVectorCharInfo());
		float len = 0, width = 0;
		size_t count = 1;

		RollBackSave roll_back;

		Ogre::UTFString::const_iterator end = mCaption.end();

		for (Ogre::UTFString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Char character = *index;

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {

				// длинна строки, кратна пикселю, плюс курсор
				len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

				// запоминаем размер предыдущей строки
				mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// и создаем новую
				mLinesInfo.push_back(PairVectorCharInfo());

				if (character == Font::FONT_CODE_CR) {
					Ogre::UTFString::const_iterator peeki = index;
					peeki++;
					if ((peeki != end) && (*peeki == Font::FONT_CODE_LF)) index = peeki; // skip both as one newline
				}

				// отменяем откат
				roll_back.reset();

				// следующий символ
				continue;

			}
			else if (character == L'#')
			{
				// берем следующий символ
				++ index;
				if (index == end) { --index ;continue; } // это защита

				character = *index;
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != L'#')
				{

					// парсим первый символ
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i=0; i<5; i++)
					{
						++ index;
						if (index == end) {--index ;continue;} // это защита
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F];
					}

					// если нужно, то меняем красный и синий компоненты
					MYGUI_CONVERT_COLOUR(colour, mRenderGL);

					// запоминаем цвет, в верхнем байте единицы
					mLinesInfo.back().second.push_back( EnumCharInfo(colour, true) );
					//count ++;

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

			// перенос строки
			if (mBreakLine
				&& (len + len_char + EDIT_TEXT_WIDTH_CURSOR + 1) > mCoord.width
				&& roll_back.rollback
				&& (mCoord.width > EDIT_MIN_BREAK_WORD_WIDTH)) {

				// откатываем назад до пробела
				len = roll_back.real_lenght;
				count = roll_back.count;
				index = roll_back.space_point;

				mLinesInfo.back().second.erase(mLinesInfo.back().second.begin() + (count-1), mLinesInfo.back().second.end());

				// длинна строки, кратна пикселю, плюс курсор
				len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

				// запоминаем размер предыдущей строки
				mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// и создаем новую
				mLinesInfo.push_back(PairVectorCharInfo());

				// отменяем откат
				roll_back.reset();

				// следующий символ
				continue;
			}

			len += len_char;

			// указатель на инфо о символе
			mLinesInfo.back().second.push_back( EnumCharInfo(info) );
			count ++;

		}

		// длинна строки, кратна пикселю, плюс курсор
		len = (float)((uint)(len + 0.99f)) + EDIT_TEXT_WIDTH_CURSOR;

		// запоминаем размер предыдущей строки
		mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

		if (width < len) width = len;

		// устанавливаем размер текста
		mContextSize.set(int(width), int(mLinesInfo.size() * mFontHeight));
		mContextRealSize.set(mContextSize.width * mManager->getPixScaleX() * 2.0f, mContextSize.height  * mManager->getPixScaleY() * 2.0f);
	}

} // namespace MyGUI
